#include "ServicesPage.h"
#include "DataManager.h"
#include "ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>
#include <QTimer>
#include <QFileDialog>

// ══════════════════════════════════════════════════════════════════
//  ServiceCard
// ══════════════════════════════════════════════════════════════════
ServiceCard::ServiceCard(const QString& iconPath,
                         const QString& name,
                         int index,
                         QWidget* parent)
    : QWidget(parent), m_index(index)
{
    setFixedSize(300, 165);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("background: transparent;");

    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(10);
    layout->setContentsMargins(16, 16, 16, 16);

    auto* iconLbl = new QLabel(this);
    iconLbl->setAlignment(Qt::AlignCenter);
    iconLbl->setStyleSheet("background: transparent;");
    QPixmap px(iconPath);
    if (!px.isNull()) {
        iconLbl->setPixmap(px.scaled(
            60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        iconLbl->setText("•");
    }

    auto* nameLbl = new QLabel(name, this);
    nameLbl->setAlignment(Qt::AlignCenter);
    QFont nf;
    nf.setFamily("Arial Rounded MT Bold");
    nf.setPixelSize(40); // raised into 14-20px range
    nf.setBold(true);
    nameLbl->setFont(nf);
    nameLbl->setStyleSheet("color: #1A1A2E; background: transparent;");
    nameLbl->setWordWrap(true);

    layout->addWidget(iconLbl, 0, Qt::AlignCenter);
    layout->addWidget(nameLbl, 0, Qt::AlignCenter);
}

void ServiceCard::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) emit clicked(m_index);
    QWidget::mousePressEvent(e);
}

// ══════════════════════════════════════════════════════════════════
//  ToastWidget
// ══════════════════════════════════════════════════════════════════
ToastWidget::ToastWidget(QWidget* parent) : QLabel(parent) {
    setStyleSheet(
        "background: rgba(30,30,30,0.92);"
        "color: white;"
        "border-radius: 12px;"
        "padding: 14px 24px;"
        "font-size: 16px;"
        "font-weight: 600;"
        );
    setAlignment(Qt::AlignCenter);
    hide();
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void ToastWidget::showToast(const QString& message) {
    setText(message);
    adjustSize();

    if (parentWidget()) {
        int px = (parentWidget()->width()  - width())  / 2;
        int py =  parentWidget()->height() - height()  - 100;
        move(px, py);
    }

    auto* eff = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(eff);
    eff->setOpacity(0.0);
    show();
    raise();

    auto* fadeIn = new QPropertyAnimation(eff, "opacity", this);
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(2200, this, [this]() {
        auto* eff2 = new QGraphicsOpacityEffect(this);
        setGraphicsEffect(eff2);
        eff2->setOpacity(1.0);
        auto* fadeOut = new QPropertyAnimation(eff2, "opacity", this);
        fadeOut->setDuration(300);
        fadeOut->setStartValue(1.0);
        fadeOut->setEndValue(0.0);
        connect(fadeOut, &QPropertyAnimation::finished,
                this, [this]() { hide(); });
        fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    });
}

// ══════════════════════════════════════════════════════════════════
//  ServicesPage
// ══════════════════════════════════════════════════════════════════
ServicesPage::ServicesPage(QWidget* parent) : QWidget(parent) {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 16);
    mainLayout->setSpacing(10);

    // ── Title — centered at top, same size as before ─────────────
    auto* title = new QLabel("SERVICES", this);
    QFont tf;
    tf.setFamily("Georgia");
    tf.setPixelSize(38);
    tf.setBold(true);
    tf.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    title->setFont(tf);
    title->setAlignment(Qt::AlignCenter);   // ← centered
    title->setObjectName("pageTitle");
    title->setStyleSheet(
        "color: #FFFFFF;"
        "background: transparent;"
        "letter-spacing: 6px;"
        );
    mainLayout->addWidget(title);

    auto* accent = new QFrame(this);
    accent->setFixedHeight(2);
    accent->setFixedWidth(180);
    accent->setStyleSheet(QString(
                              "background: %1; border: none;")
                              .arg(ThemeManager::instance().primary().name()));
    auto* accentRow = new QHBoxLayout();
    accentRow->addStretch();
    accentRow->addWidget(accent);
    accentRow->addStretch();
    mainLayout->addLayout(accentRow);

    auto* subtitle = new QLabel(
        "Select a service to submit your request", this);
    subtitle->setAlignment(Qt::AlignCenter);
    QFont sf; sf.setPixelSize(25); // raised
    subtitle->setFont(sf);
    subtitle->setStyleSheet(
        "color: rgba(255,255,255,0.65); background: transparent;");
    mainLayout->addWidget(subtitle);

    mainLayout->addSpacing(8);
    setupGrid();
    mainLayout->addStretch();

    setupDrawer();
    toast = new ToastWidget(this);
}

void ServicesPage::setupGrid() {
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("background: transparent;");

    auto* container = new QWidget(scrollArea);
    container->setStyleSheet("background: transparent;");
    auto* grid = new QGridLayout(container);
    grid->setSpacing(20);
    grid->setContentsMargins(4, 4, 4, 4);

    struct SvcInfo { QString iconPath; QString name; };
    QList<SvcInfo> services = {
        {":/assets/images/swap.jpg",    "Room Swap"},
        {":/assets/images/change.jpg",  "Change Request"},
        {":/assets/images/photo.jpg",   "Room Photo"},
        {":/assets/images/report.jpg",  "Report Problem"},
        {":/assets/images/chief.jpg",   "Pavilion Chief"},
        {":/assets/images/contact.jpg", "Admin Contact"}
    };

    for (int i = 0; i < services.size(); ++i) {
        auto* card = new ServiceCard(
            services[i].iconPath, services[i].name, i, container);
        connect(card, &ServiceCard::clicked,
                this, &ServicesPage::openDrawer);
        grid->addWidget(card, i / 2, i % 2);
    }

    scrollArea->setWidget(container);

    auto* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) mainLayout->addWidget(scrollArea, 1);
}

void ServicesPage::setupDrawer() {
    drawer = new QWidget(this);
    drawer->setFixedHeight(340);
    drawer->move(0, height());
    drawer->hide();

    drawer->setStyleSheet(
        "background: rgba(20,20,30,0.97);"
        "border-top-left-radius: 24px;"
        "border-top-right-radius: 24px;"
        "border-top: 1px solid rgba(255,255,255,0.12);"
        );

    auto* drawerShadow = new QGraphicsDropShadowEffect(drawer);
    drawerShadow->setBlurRadius(30);
    drawerShadow->setOffset(0, -6);
    drawerShadow->setColor(QColor(0, 0, 0, 80));
    drawer->setGraphicsEffect(drawerShadow);

    auto* drawerLayout = new QVBoxLayout(drawer);
    drawerLayout->setContentsMargins(24, 12, 24, 20);
    drawerLayout->setSpacing(12);

    auto* handle = new QFrame(drawer);
    handle->setFixedSize(48, 4);
    handle->setStyleSheet(
        "background: rgba(255,255,255,0.25); border-radius: 2px;");
    auto* handleRow = new QHBoxLayout();
    handleRow->addStretch();
    handleRow->addWidget(handle);
    handleRow->addStretch();
    drawerLayout->addLayout(handleRow);

    auto* titleRow = new QHBoxLayout();
    drawerTitle = new QLabel("Service", drawer);
    QFont dtf;
    dtf.setFamily("Arial Rounded MT Bold");
    dtf.setPixelSize(24);
    dtf.setBold(true);
    drawerTitle->setFont(dtf);
    drawerTitle->setStyleSheet("color: #FFFFFF; background: transparent;");

    auto* closeBtn = new QPushButton("✕", drawer);
    closeBtn->setFixedSize(34, 34);
    closeBtn->setStyleSheet(
        "QPushButton { background: rgba(255,255,255,0.12);"
        "border-radius: 17px; font-size: 16px; color: #FFFFFF; }"
        "QPushButton:hover { background: rgba(255,255,255,0.22); }"
        );
    connect(closeBtn, &QPushButton::clicked,
            this, &ServicesPage::closeDrawer);

    titleRow->addWidget(drawerTitle);
    titleRow->addStretch();
    titleRow->addWidget(closeBtn);
    drawerLayout->addLayout(titleRow);

    submitBtn = new QPushButton("Submit Request", drawer);
    submitBtn->setFixedHeight(46);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setStyleSheet(QString(
                                 "QPushButton {"
                                 "  background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
                                 "    stop:0 %1, stop:1 %2);"
                                 "  color: white;"
                                 "  border-radius: 14px;"
                                 "  font-size: 17px;"
                                 "  font-weight: 700;"
                                 "  letter-spacing: 1px;"
                                 "}"
                                 "QPushButton:hover { opacity: 0.85; }"
                                 ).arg(ThemeManager::instance().primary().name(),
                                      ThemeManager::instance().secondary().name()));
    connect(submitBtn, &QPushButton::clicked,
            this, &ServicesPage::handleSubmit);

    drawerStack = new QStackedWidget(drawer);
    drawerStack->addWidget(makeRoomSwapForm());
    drawerStack->addWidget(makeChangeRequestForm());
    drawerStack->addWidget(makeRoomPhotoForm());
    drawerStack->addWidget(makeReportProblemForm());
    drawerStack->addWidget(makePavilionChiefInfo());
    drawerStack->addWidget(makeAdminContactInfo());
    drawerLayout->addWidget(drawerStack, 1);
    drawerLayout->addWidget(submitBtn);
}

QWidget* ServicesPage::makeRoomSwapForm() {
    auto* w = new QWidget();
    w->setStyleSheet("background: transparent;");
    auto* l = new QVBoxLayout(w);
    l->setSpacing(10);
    l->setContentsMargins(0, 0, 0, 0);

    auto* lbl = new QLabel(
        "Enter the ID of the student you want to swap with:", w);
    QFont lf; lf.setPixelSize(20);
    lbl->setFont(lf);
    lbl->setStyleSheet("color: #FFFFFF; background: transparent;");
    lbl->setWordWrap(true);
    l->addWidget(lbl);

    swapTargetIdField = new QLineEdit(w);
    swapTargetIdField->setPlaceholderText("Target Student ID (e.g. 20230102)");
    swapTargetIdField->setFixedHeight(44);
    QFont fldF; fldF.setPixelSize(19);
    swapTargetIdField->setFont(fldF);
    l->addWidget(swapTargetIdField);

    auto* note = new QLabel(
        "The other student must also submit a request with your ID.", w);
    QFont nf; nf.setPixelSize(18); nf.setItalic(true);
    note->setFont(nf);
    note->setWordWrap(true);
    note->setStyleSheet(
        "color: rgba(255,255,255,0.55); background: transparent;");
    l->addWidget(note);
    l->addStretch();
    return w;
}

QWidget* ServicesPage::makeChangeRequestForm() {
    auto* w = new QWidget();
    w->setStyleSheet("background: transparent;");
    auto* l = new QVBoxLayout(w);
    l->setSpacing(10);
    l->setContentsMargins(0, 0, 0, 0);

    auto mkLbl = [&](const QString& t) {
        auto* lb = new QLabel(t, w);
        QFont f; f.setPixelSize(20);
        lb->setFont(f);
        lb->setStyleSheet("color: #FFFFFF; background: transparent;");
        return lb;
    };

    l->addWidget(mkLbl("Reason for room change:"));
    changeReasonEdit = new QTextEdit(w);
    changeReasonEdit->setPlaceholderText("Describe your reason...");
    changeReasonEdit->setFixedHeight(70);
    QFont teF; teF.setPixelSize(19);
    changeReasonEdit->setFont(teF);
    l->addWidget(changeReasonEdit);

    l->addWidget(mkLbl("Preferred room / floor:"));
    changeRoomField = new QLineEdit(w);
    changeRoomField->setPlaceholderText("e.g. Room 210, Floor 2");
    changeRoomField->setFixedHeight(40);
    changeRoomField->setFont(teF);
    l->addWidget(changeRoomField);

    l->addStretch();
    return w;
}

QWidget* ServicesPage::makeRoomPhotoForm() {
    auto* w = new QWidget();
    w->setStyleSheet("background: transparent;");
    auto* l = new QVBoxLayout(w);
    l->setSpacing(10);
    l->setContentsMargins(0, 0, 0, 0);
    l->setAlignment(Qt::AlignTop);

    auto* lbl = new QLabel("Upload a photo of your room:", w);
    QFont lf; lf.setPixelSize(20);
    lbl->setFont(lf);
    lbl->setStyleSheet("color: #FFFFFF; background: transparent;");
    l->addWidget(lbl);

    photoPreviewLabel = new QLabel(w);
    photoPreviewLabel->setFixedSize(160, 100);
    photoPreviewLabel->setAlignment(Qt::AlignCenter);
    QFont ppF; ppF.setPixelSize(18);
    photoPreviewLabel->setFont(ppF);
    photoPreviewLabel->setStyleSheet(
        "background: rgba(255,255,255,0.08);"
        "border-radius: 10px;"
        "border: 2px dashed rgba(255,255,255,0.25);"
        "color: rgba(255,255,255,0.55);"
        );

    auto* user = DataManager::instance().currentUser;
    if (user && !user->roomPhotoPath.isEmpty()) {
        QPixmap px(user->roomPhotoPath);
        if (!px.isNull())
            photoPreviewLabel->setPixmap(
                px.scaled(160, 100, Qt::KeepAspectRatio,
                          Qt::SmoothTransformation));
        else
            photoPreviewLabel->setText("Photo submitted");
    } else {
        photoPreviewLabel->setText("No photo yet");
    }

    auto* chooseBtn = new QPushButton("Choose Photo", w);
    chooseBtn->setFixedHeight(40);
    QFont btnF; btnF.setPixelSize(18);
    chooseBtn->setFont(btnF);
    connect(chooseBtn, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getOpenFileName(
            this, "Select Room Photo", "",
            "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!path.isEmpty()) {
            QPixmap px(path);
            if (!px.isNull())
                photoPreviewLabel->setPixmap(
                    px.scaled(160, 100, Qt::KeepAspectRatio,
                              Qt::SmoothTransformation));
            auto* u = DataManager::instance().currentUser;
            if (u) u->roomPhotoPath = path;
        }
    });

    l->addWidget(photoPreviewLabel, 0, Qt::AlignLeft);
    l->addWidget(chooseBtn);
    l->addStretch();
    return w;
}

QWidget* ServicesPage::makeReportProblemForm() {
    auto* w = new QWidget();
    w->setStyleSheet("background: transparent;");
    auto* l = new QVBoxLayout(w);
    l->setSpacing(10);
    l->setContentsMargins(0, 0, 0, 0);

    auto mkLbl = [&](const QString& t) {
        auto* lb = new QLabel(t, w);
        QFont f; f.setPixelSize(20);
        lb->setFont(f);
        lb->setStyleSheet("color: #FFFFFF; background: transparent;");
        return lb;
    };

    l->addWidget(mkLbl("Problem category:"));
    problemCategoryBox = new QComboBox(w);
    problemCategoryBox->addItems(
        {"Plumbing", "Electricity", "Furniture", "Other"});
    problemCategoryBox->setFixedHeight(40);
    QFont cbF; cbF.setPixelSize(19);
    problemCategoryBox->setFont(cbF);
    l->addWidget(problemCategoryBox);

    l->addWidget(mkLbl("Description:"));
    problemDescEdit = new QTextEdit(w);
    problemDescEdit->setPlaceholderText("Describe the problem...");
    problemDescEdit->setFixedHeight(70);
    problemDescEdit->setFont(cbF);
    l->addWidget(problemDescEdit);

    l->addStretch();
    return w;
}

QWidget* ServicesPage::makePavilionChiefInfo() {
    auto* w = new QWidget();
    w->setStyleSheet("background: transparent;");
    auto* l = new QVBoxLayout(w);
    l->setSpacing(10);
    l->setContentsMargins(0, 0, 0, 0);
    l->setAlignment(Qt::AlignTop);

    Student* chief = nullptr;
    for (auto& s : DataManager::instance().students)
        if (s.role == "chief") { chief = &s; break; }

    QString chiefName = chief ? chief->name : "Fatima Oukaci";
    QString chiefRoom = chief ? chief->room : "101";

    auto* card = new QWidget(w);
    card->setStyleSheet(
        "background: rgba(255,255,255,0.10);"
        "border-radius: 14px;"
        "border: 1px solid rgba(255,255,255,0.15);"
        );
    auto* cl = new QVBoxLayout(card);
    cl->setContentsMargins(18, 16, 18, 16);
    cl->setSpacing(8);

    auto* nameLbl = new QLabel(chiefName, card);
    QFont nf; nf.setPixelSize(24); nf.setBold(true);
    nameLbl->setFont(nf);
    nameLbl->setStyleSheet(QString(
                               "color: %1; background: transparent;")
                               .arg(ThemeManager::instance().primary().name()));
    cl->addWidget(nameLbl);

    auto mkRow = [&](const QString& text) {
        auto* lb = new QLabel(text, card);
        QFont f; f.setPixelSize(20);
        lb->setFont(f);
        lb->setStyleSheet("color: #FFFFFF; background: transparent;");
        cl->addWidget(lb);
    };
    mkRow("Room " + chiefRoom);
    mkRow("0550 123 456");
    mkRow("chief@urdorm.dz");
    mkRow("Available: 8:00 AM – 5:00 PM");

    l->addWidget(card);
    l->addStretch();
    return w;
}

QWidget* ServicesPage::makeAdminContactInfo() {
    auto* w = new QWidget();
    w->setStyleSheet("background: transparent;");
    auto* l = new QVBoxLayout(w);
    l->setSpacing(10);
    l->setContentsMargins(0, 0, 0, 0);
    l->setAlignment(Qt::AlignTop);

    auto mkContact = [&](const QString& ext, const QString& dept) {
        auto* row = new QWidget(w);
        row->setStyleSheet(
            "background: rgba(255,255,255,0.08);"
            "border-radius: 10px;"
            );
        auto* rl = new QHBoxLayout(row);
        rl->setContentsMargins(14, 10, 14, 10);

        auto* extLbl = new QLabel(ext, row);
        QFont ef; ef.setPixelSize(21); ef.setBold(true);
        extLbl->setFont(ef);
        extLbl->setStyleSheet(QString(
                                  "color: %1; background: transparent;")
                                  .arg(ThemeManager::instance().primary().name()));

        auto* deptLbl = new QLabel(dept, row);
        QFont df; df.setPixelSize(20);
        deptLbl->setFont(df);
        deptLbl->setStyleSheet("color: #FFFFFF; background: transparent;");

        rl->addWidget(extLbl);
        rl->addWidget(deptLbl, 1);
        l->addWidget(row);
    };

    mkContact("Ext. 101", "Main Office");
    mkContact("Ext. 102", "Maintenance");
    mkContact("Ext. 103", "Security");
    mkContact("Ext. 104", "Medical Room");

    l->addStretch();
    return w;
}

void ServicesPage::openDrawer(int serviceIndex) {
    currentService = serviceIndex;

    QStringList titles = {
        "ROOM SWAP", "CHANGE REQUEST", "ROOM PHOTO",
        "REPORT PROBLEM", "PAVILION CHIEF", "ADMIN CONTACT"
    };
    drawerTitle->setText(titles[serviceIndex]);
    drawerStack->setCurrentIndex(serviceIndex);

    bool showSubmit = (serviceIndex != 4 && serviceIndex != 5);
    submitBtn->setVisible(showSubmit);

    drawer->setFixedWidth(width());
    drawer->move(0, height());
    drawer->show();
    drawer->raise();

    auto* anim = new QPropertyAnimation(drawer, "geometry", this);
    anim->setDuration(300);
    anim->setStartValue(QRect(0, height(), width(), 340));
    anim->setEndValue(QRect(0, height() - 340, width(), 340));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    drawerOpen = true;
}

void ServicesPage::closeDrawer() {
    auto* anim = new QPropertyAnimation(drawer, "geometry", this);
    anim->setDuration(250);
    anim->setStartValue(drawer->geometry());
    anim->setEndValue(QRect(0, height(), width(), 340));
    anim->setEasingCurve(QEasingCurve::InCubic);
    connect(anim, &QPropertyAnimation::finished,
            drawer, &QWidget::hide);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    drawerOpen = false;
}

void ServicesPage::handleSubmit() {
    auto* user = DataManager::instance().currentUser;
    if (user && currentService == 0 && swapTargetIdField) {
        user->roomSwapRequested = true;
        user->swapTargetId = swapTargetIdField->text().trimmed();
    }

    DataManager::Notification notif;
    QStringList names = {
        "Room Swap", "Change Request", "Room Photo",
        "Report Problem", "", ""
    };
    notif.title       = names[currentService] + " Submitted";
    notif.description = "Your request has been received and is under review.";
    notif.timestamp   = "Just now";
    notif.type        = "announcement";
    notif.isRead      = false;
    DataManager::instance().notifications.insert(
        DataManager::instance().notifications.begin(), notif);

    emit notificationAdded();
    closeDrawer();

    QTimer::singleShot(350, this, [this]() {
        toast->showToast("Request submitted successfully");
    });
}

void ServicesPage::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    drawer->setFixedWidth(width());
    if (drawerOpen)
        drawer->move(0, height() - 340);
    else
        drawer->move(0, height());
}

void ServicesPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    if (!drawerOpen)
        drawer->move(0, height());
}