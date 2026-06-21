#include "StudentCardPage.h"
#include "DataManager.h"
#include "ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QScrollArea>
#include <QPainter>
#include <QPainterPath>
#include <QListWidget>
#include <QPushButton>
#include <QLinearGradient>

// ══════════════════════════════════════════════════════════════════
//  FlipCard
// ══════════════════════════════════════════════════════════════════
FlipCard::FlipCard(QWidget* parent) : QWidget(parent) {
    setFixedSize(580, 370);
    setCursor(Qt::PointingHandCursor);

    cardStack = new QStackedWidget(this);
    cardStack->setGeometry(0, 0, 580, 370);

    frontWidget = new QWidget(cardStack);
    frontWidget->setFixedSize(580, 370);

    backWidget = new QWidget(cardStack);
    backWidget->setFixedSize(580, 370);

    cardStack->addWidget(frontWidget);
    cardStack->addWidget(backWidget);
    cardStack->setCurrentIndex(0);

    setStyleSheet("QWidget { background: transparent; }");

    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(50);
    shadow->setOffset(0, 16);
    shadow->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(shadow);
}

void FlipCard::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QColor pri = ThemeManager::instance().primary();
    QColor sec = ThemeManager::instance().secondary();

    QLinearGradient grad(0, 0, width(), height());
    grad.setColorAt(0.0, pri.lighter(190));
    grad.setColorAt(0.5, sec.lighter(200));
    grad.setColorAt(1.0, pri.lighter(175));

    QPainterPath path;
    path.addRoundedRect(rect(), 18, 18);
    p.fillPath(path, grad);

    // Decorative circles pattern
    p.setPen(QPen(QColor(255, 255, 255, 60), 0.8));
    p.setBrush(Qt::NoBrush);
    for (int i = 0; i < 12; ++i) {
        int r = 40 + i * 22;
        p.drawEllipse(QPoint(width() / 3, height() + 20), r, r);
    }

    // Border
    p.setPen(QPen(QColor(255, 255, 255, 120), 1.5));
    p.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 18, 18);
}

QLabel* FlipCard::makeRectPhoto(const QString& path,
                                int w, int h, QWidget* parent) {
    auto* lbl = new QLabel(parent);
    lbl->setFixedSize(w, h);
    lbl->setAlignment(Qt::AlignCenter);

    QPixmap px(path);
    if (!px.isNull()) {
        lbl->setPixmap(px.scaled(
            w, h, Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation));
        lbl->setStyleSheet(
            "border: 2px solid white;"
            "border-radius: 4px;"
            "background: transparent;"
            );
    } else {
        lbl->setStyleSheet(QString(
                               "background: %1;"
                               "border: 2px solid white;"
                               "border-radius: 4px;"
                               "color: white;"
                               "font-size: 18px;")
                               .arg(ThemeManager::instance().primary().name()));
        lbl->setText("PHOTO");
    }
    return lbl;
}

QLabel* FlipCard::makeCircularPhoto(const QString& path,
                                    int size, QWidget* parent) {
    return makeRectPhoto(path, size, size, parent);
}

void FlipCard::setupFrontFace(const QString& photoPath,
                              const QString& name,
                              const QString& id,
                              const QString& major,
                              const QString& room,
                              const QString& pavilion)
{
    frontWidget->setStyleSheet("background: transparent;");

    // ── "STUDENT"
    auto* studentLbl = new QLabel("STUDENT", frontWidget);
    QFont sf;
    sf.setFamily("Georgia");
    sf.setPixelSize(28);
    sf.setBold(true);
    sf.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    studentLbl->setFont(sf);
    studentLbl->setStyleSheet(
        "color: #1A1A1A; background: transparent;");
    studentLbl->setAlignment(Qt::AlignCenter);
    studentLbl->setGeometry(0, 18, 380, 36);

    auto* line1 = new QFrame(frontWidget);
    line1->setFrameShape(QFrame::HLine);
    line1->setGeometry(30, 60, 320, 1);
    line1->setStyleSheet("background: #9B4F96; border: none;");


    auto* idCardLbl = new QLabel("IDENTITY  CARD", frontWidget);
    QFont icf;
    icf.setFamily("Arial");
    icf.setPixelSize(13);
    icf.setLetterSpacing(QFont::AbsoluteSpacing, 3);
    idCardLbl->setFont(icf);
    idCardLbl->setStyleSheet(
        "color: #9B4F96; background: transparent;");
    idCardLbl->setAlignment(Qt::AlignCenter);
    idCardLbl->setGeometry(0, 66, 380, 20);

    // ── Info
    int leftX  = 30;
    int startY = 100;
    int rowGap = 62;

    struct InfoRow { QString label; QString value; };
    QList<InfoRow> rows = {
        {"Studies at", pavilion.isEmpty() ? "URdorm University" : pavilion},
        {"Name",       name},
        {"Major",      major},
        {"Room",       room}
    };

    for (int i = 0; i < rows.size(); ++i) {
        int y = startY + i * rowGap;


        auto* labelLbl = new QLabel(rows[i].label, frontWidget);
        QFont lf;
        lf.setFamily("Arial");
        lf.setPixelSize(25);
        lf.setBold(true);
        labelLbl->setFont(lf);
        labelLbl->setStyleSheet(
            "color: #9B4F96; background: transparent;");
        labelLbl->setGeometry(leftX, y, 320, 30);

        auto* valueLbl = new QLabel(
            rows[i].value.toUpper(), frontWidget);
        QFont vf;
        vf.setFamily("Arial");
        vf.setPixelSize(20);
        vf.setBold(true);
        valueLbl->setFont(vf);
        valueLbl->setStyleSheet(
            "color: #1A1A1A; background: transparent;");
        valueLbl->setGeometry(leftX, y + 32, 320, 24);
    }

    auto* idNumLbl = new QLabel(id, frontWidget);
    QFont idf;
    idf.setFamily("Arial");
    idf.setPixelSize(13);
    idf.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    idNumLbl->setFont(idf);
    idNumLbl->setStyleSheet(
        "color: #9B4F96; background: transparent;");
    idNumLbl->setGeometry(leftX, 342, 320, 20);
    idNumLbl->setAlignment(Qt::AlignLeft);

    auto* photo = makeRectPhoto(photoPath, 120, 160, frontWidget);
    photo->move(440, 90);
}

void FlipCard::setupBackFace(const QString& /*id*/) {
    backWidget->setStyleSheet("background: transparent;");

    auto* layout = new QVBoxLayout(backWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(0);

    auto* qrLarge = new QLabel(backWidget);
    QPixmap qrPx(":/assets/images/qr_placeholder.png");
    if (!qrPx.isNull())
        qrLarge->setPixmap(
            qrPx.scaled(260, 260,
                        Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        qrLarge->setText("QR");
    qrLarge->setAlignment(Qt::AlignCenter);
    qrLarge->setStyleSheet(
        "background: white;"
        "border-radius: 16px;"
        "padding: 14px;"
        );
    qrLarge->setFixedSize(288, 288);

    layout->addWidget(qrLarge, 0, Qt::AlignCenter);
}

void FlipCard::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton && !animating)
        flipCard();
    QWidget::mousePressEvent(e);
}

void FlipCard::flipCard() {
    animating   = true;
    int centerX = x() + width() / 2;
    QRect orig  = geometry();

    auto* phase1 = new QPropertyAnimation(this, "geometry", this);
    phase1->setDuration(150);
    phase1->setStartValue(orig);
    phase1->setEndValue(QRect(centerX, orig.y(), 0, orig.height()));
    phase1->setEasingCurve(QEasingCurve::InQuad);

    connect(phase1, &QPropertyAnimation::finished, this,
            [this, orig, centerX]() {
                cardStack->setCurrentIndex(
                    1 - cardStack->currentIndex());
                isFlipped = !isFlipped;
                update();

                auto* phase2 = new QPropertyAnimation(
                    this, "geometry", this);
                phase2->setDuration(150);
                phase2->setStartValue(
                    QRect(centerX, orig.y(), 0, orig.height()));
                phase2->setEndValue(orig);
                phase2->setEasingCurve(QEasingCurve::OutQuad);
                connect(phase2, &QPropertyAnimation::finished,
                        this, [this]() { animating = false; });
                phase2->start(QAbstractAnimation::DeleteWhenStopped);
            });
    phase1->start(QAbstractAnimation::DeleteWhenStopped);
}

// ══════════════════════════════════════════════════════════════════
//  StudentCardPage
// ══════════════════════════════════════════════════════════════════
StudentCardPage::StudentCardPage(QWidget* parent) : QWidget(parent) {
    auto* user   = DataManager::instance().currentUser;
    QString role = user ? user->role : "student";

    if      (role == "student")        setupStudentView();
    else if (role == "administration") setupAdminView();
    else if (role == "chief")          setupChiefView();
    else if (role == "inspection")     setupInspectionView();
}

void StudentCardPage::setupStudentView() {
    auto* user   = DataManager::instance().currentUser;
    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);
    layout->setContentsMargins(24, 80, 24, 100);

    auto* title = new QLabel("MY  CARD", this);
    QFont tf;
    tf.setFamily("Georgia");
    tf.setPixelSize(38);
    tf.setBold(true);
    tf.setLetterSpacing(QFont::AbsoluteSpacing, 8);
    title->setFont(tf);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("pageTitle");
    title->setStyleSheet("color: #FFFFFF; background: transparent;");
    layout->addWidget(title);

    auto* card = new FlipCard(this);
    if (user) {
        card->setupFrontFace(
            user->photoPath,
            user->name,
            user->id,
            user->major,
            user->room,
            user->pavilion
            );
        card->setupBackFace(user->id);
    }
    layout->addWidget(card, 0, Qt::AlignCenter);
    layout->addStretch();
}

void StudentCardPage::setupAdminView() {
    auto& dm     = DataManager::instance();
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 80, 24, 100);
    layout->setSpacing(14);

    auto* title = new QLabel("ADMIN  PANEL", this);
    QFont tf;
    tf.setFamily("Georgia");
    tf.setPixelSize(38);
    tf.setBold(true);
    tf.setLetterSpacing(QFont::AbsoluteSpacing, 8);
    title->setFont(tf);
    title->setObjectName("pageTitle");
    title->setStyleSheet("color: #FFFFFF; background: transparent;");
    layout->addWidget(title);

    auto* subtitle = new QLabel(
        "Student overview and room requests", this);
    QFont sf; sf.setPixelSize(22);
    subtitle->setFont(sf);
    subtitle->setStyleSheet(
        "color: rgba(255,255,255,0.70); background: transparent;");
    layout->addWidget(subtitle);

    auto* statsRow = new QHBoxLayout();
    statsRow->setSpacing(14);

    auto mkStat = [this](const QString& val,
                         const QString& lbl,
                         const QString& color) -> QWidget* {
        auto* w = new QWidget(this);
        w->setStyleSheet(QString(
                             "background: %1; border-radius: 16px;").arg(color));
        auto* l = new QVBoxLayout(w);
        l->setAlignment(Qt::AlignCenter);
        auto* vl = new QLabel(val, w);
        QFont vf; vf.setPixelSize(30); vf.setBold(true);
        vl->setFont(vf);
        vl->setStyleSheet("color: white; background: transparent;");
        vl->setAlignment(Qt::AlignCenter);
        auto* ll = new QLabel(lbl, w);
        QFont lf; lf.setPixelSize(19);
        ll->setFont(lf);
        ll->setStyleSheet(
            "color: rgba(255,255,255,0.85); background: transparent;");
        ll->setAlignment(Qt::AlignCenter);
        l->addWidget(vl); l->addWidget(ll);
        w->setMinimumHeight(100);
        return w;
    };

    int studentCount = 0, swapCount = 0, photoCount = 0;
    for (auto& s : dm.students) {
        if (s.role == "student") {
            studentCount++;
            if (s.roomSwapRequested) swapCount++;
            if (!s.roomPhotoPath.isEmpty()) photoCount++;
        }
    }

    statsRow->addWidget(mkStat(
        QString::number(studentCount), "Students",        "#9C27B0"));
    statsRow->addWidget(mkStat(
        QString::number(swapCount),    "Swap Requests",   "#FF9800"));
    statsRow->addWidget(mkStat(
        QString::number(photoCount),   "Photos Submitted","#4CAF50"));
    layout->addLayout(statsRow);

    auto* listTitle = new QLabel("ALL STUDENTS", this);
    QFont lt;
    lt.setFamily("Georgia");
    lt.setPixelSize(22);
    lt.setBold(true);
    lt.setLetterSpacing(QFont::AbsoluteSpacing, 4);
    listTitle->setFont(lt);
    listTitle->setStyleSheet(
        "color: #FFFFFF; background: transparent;");
    layout->addWidget(listTitle);

    auto* list = new QListWidget(this);
    QFont lf; lf.setPixelSize(20);
    list->setFont(lf);
    for (auto& s : dm.students) {
        if (s.role != "student") continue;
        list->addItem(new QListWidgetItem(
            QString("  %1      Room %2      %3")
                .arg(s.name).arg(s.room)
                .arg(s.roomPhotoPath.isEmpty()
                         ? "No photo" : "Photo submitted")));
    }
    layout->addWidget(list, 1);
}

void StudentCardPage::setupChiefView() {
    auto* user   = DataManager::instance().currentUser;
    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(16);
    layout->setContentsMargins(24, 80, 24, 100);

    auto* title = new QLabel("MY  CARD", this);
    QFont tf;
    tf.setFamily("Georgia");
    tf.setPixelSize(38);
    tf.setBold(true);
    tf.setLetterSpacing(QFont::AbsoluteSpacing, 8);
    title->setFont(tf);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("pageTitle");
    title->setStyleSheet("color: #FFFFFF; background: transparent;");
    layout->addWidget(title);

    auto* badge = new QLabel("Pavilion Chief", this);
    QFont bf;
    bf.setFamily("Georgia");
    bf.setPixelSize(22);
    badge->setFont(bf);
    badge->setAlignment(Qt::AlignCenter);
    badge->setStyleSheet(QString(
                             "color: %1; background: transparent;")
                             .arg(ThemeManager::instance().primary().name()));
    layout->addWidget(badge);

    auto* card = new FlipCard(this);
    if (user) {
        card->setupFrontFace(
            user->photoPath, user->name, user->id,
            "Pavilion Chief", user->room, user->pavilion);
        card->setupBackFace(user->id);
    }
    layout->addWidget(card, 0, Qt::AlignCenter);
    layout->addStretch();
}

void StudentCardPage::setupInspectionView() {
    auto& dm     = DataManager::instance();
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 80, 24, 100);
    layout->setSpacing(14);

    auto* title = new QLabel("ROOM  INSPECTION", this);
    QFont tf;
    tf.setFamily("Georgia");
    tf.setPixelSize(38);
    tf.setBold(true);
    tf.setLetterSpacing(QFont::AbsoluteSpacing, 8);
    title->setFont(tf);
    title->setObjectName("pageTitle");
    title->setStyleSheet("color: #FFFFFF; background: transparent;");
    layout->addWidget(title);

    auto* sub = new QLabel(
        "Review and validate student room photos", this);
    QFont sf; sf.setPixelSize(22);
    sub->setFont(sf);
    sub->setStyleSheet(
        "color: rgba(255,255,255,0.70); background: transparent;");
    layout->addWidget(sub);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto* container = new QWidget(scrollArea);
    auto* vLayout   = new QVBoxLayout(container);
    vLayout->setSpacing(10);
    vLayout->setContentsMargins(0, 0, 0, 0);

    for (auto& s : dm.students) {
        if (s.role != "student") continue;

        auto* row = new QWidget(container);
        row->setStyleSheet(
            "background: rgba(255,255,255,0.12);"
            "border-radius: 12px;"
            "border: 1px solid rgba(255,255,255,0.18);");
        auto* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(16, 12, 16, 12);
        rowLayout->setSpacing(14);

        auto* nameLbl = new QLabel(s.name, row);
        QFont nf; nf.setPixelSize(20); nf.setBold(true);
        nameLbl->setFont(nf);
        nameLbl->setStyleSheet(
            "color: #FFFFFF; background: transparent;");

        auto* roomLbl = new QLabel("Room " + s.room, row);
        QFont rf; rf.setPixelSize(19);
        roomLbl->setFont(rf);
        roomLbl->setStyleSheet(
            "color: rgba(255,255,255,0.70); background: transparent;");

        QString photoStatus, statusColor;
        if (s.roomPhotoPath.isEmpty()) {
            photoStatus = "No Photo";
            statusColor = "#9E9E9E";
        } else if (s.roomPhotoValidated) {
            photoStatus = "Validated";
            statusColor = "#4CAF50";
        } else {
            photoStatus = "Pending";
            statusColor = "#FF9800";
        }

        auto* statusLbl = new QLabel(photoStatus, row);
        QFont stf; stf.setPixelSize(19); stf.setBold(true);
        statusLbl->setFont(stf);
        statusLbl->setStyleSheet(QString(
                                     "color: %1; background: transparent;").arg(statusColor));

        auto* validateBtn = new QPushButton("Validate", row);
        validateBtn->setFixedSize(90, 34);
        validateBtn->setEnabled(
            !s.roomPhotoPath.isEmpty() && !s.roomPhotoValidated);

        Student* sPtr = &s;
        connect(validateBtn, &QPushButton::clicked, this,
                [sPtr, statusLbl, validateBtn]() {
                    sPtr->roomPhotoValidated = true;
                    statusLbl->setText("Validated");
                    statusLbl->setStyleSheet(
                        "color: #4CAF50; font-size: 15px;"
                        "font-weight: bold; background: transparent;");
                    validateBtn->setEnabled(false);
                });

        rowLayout->addWidget(nameLbl,    2);
        rowLayout->addWidget(roomLbl,    1);
        rowLayout->addWidget(statusLbl,  1);
        rowLayout->addWidget(validateBtn);
        vLayout->addWidget(row);
    }
    vLayout->addStretch();

    scrollArea->setWidget(container);
    layout->addWidget(scrollArea, 1);
}