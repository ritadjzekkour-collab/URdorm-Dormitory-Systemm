#include "MainWindow.h"
#include "StudentCardPage.h"
#include "ServicesPage.h"
#include "NotificationsPage.h"
#include "WaterStatusPage.h"
#include "InfoPage.h"
#include "DataManager.h"
#include "ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QResizeEvent>
#include <functional>

// ══════════════════════════════════════════════════════════════════
//  TabButton
// ══════════════════════════════════════════════════════════════════
TabButton::TabButton(const QString& iconPath, QWidget* parent)
    : QWidget(parent)
{
    setCursor(Qt::PointingHandCursor);
    setFixedSize(72, 72);
    m_icon = QPixmap(iconPath);
}

void TabButton::setActive(bool active) {
    m_active = active;
    m_size   = active ? 78 : 68;
    update();
}

void TabButton::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) emit clicked();
    QWidget::mousePressEvent(e);
}

void TabButton::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    int cx = width()  / 2;
    int cy = height() / 2;

    if (m_active) {
        QColor glow = ThemeManager::instance().primary();
        glow.setAlpha(35);
        p.setPen(Qt::NoPen);
        p.setBrush(glow);
        p.drawEllipse(QPoint(cx, cy), 33, 33);

        glow.setAlpha(55);
        p.setBrush(glow);
        p.drawEllipse(QPoint(cx, cy), 24, 24);
    }

    if (!m_icon.isNull()) {
        int half = m_size / 2;
        QRect iconRect(cx - half, cy - half, m_size, m_size);
        if (!m_active) p.setOpacity(0.40);
        p.drawPixmap(iconRect,
                     m_icon.scaled(m_size, m_size,
                                   Qt::KeepAspectRatio, Qt::SmoothTransformation));
        p.setOpacity(1.0);
    }
}

// ══════════════════════════════════════════════════════════════════
//  ThemeSwatch
// ══════════════════════════════════════════════════════════════════
class ThemeSwatch : public QFrame {
public:
    std::function<void(int)> onSelected;

    ThemeSwatch(int idx, const QString& name,
                const QColor& color, QWidget* parent = nullptr)
        : QFrame(parent), m_index(idx)
    {
        setFixedSize(90, 90);
        setCursor(Qt::PointingHandCursor);
        auto* l = new QVBoxLayout(this);
        l->setAlignment(Qt::AlignCenter);
        l->setSpacing(6);

        auto* dot = new QLabel(this);
        dot->setFixedSize(40, 40);
        dot->setStyleSheet(QString(
                               "background: %1; border-radius: 20px;").arg(color.name()));

        auto* lbl = new QLabel(name, this);
        lbl->setAlignment(Qt::AlignCenter);
        QFont f; f.setPixelSize(17); f.setBold(true);
        lbl->setFont(f);
        lbl->setStyleSheet("background: transparent; color: #FFFFFF;");

        l->addWidget(dot, 0, Qt::AlignCenter);
        l->addWidget(lbl, 0, Qt::AlignCenter);

        bool isActive = (idx == ThemeManager::instance().currentTheme());
        setStyleSheet(isActive
                          ? QString("QFrame { border: 2px solid %1; border-radius: 12px;"
                                    "background: rgba(255,255,255,0.15); }")
                                .arg(ThemeManager::instance().primary().name())
                          : "QFrame { border: 1px solid rgba(255,255,255,0.2);"
                            "border-radius: 12px; }");
    }

protected:
    void mousePressEvent(QMouseEvent* e) override {
        QFrame::mousePressEvent(e);
        if (onSelected) onSelected(m_index);
    }
    void enterEvent(QEnterEvent* e) override {
        QFrame::enterEvent(e);
        setStyleSheet(
            "QFrame { border: 1px solid rgba(255,255,255,0.5);"
            "border-radius: 12px;"
            "background: rgba(255,255,255,0.12); }");
    }
    void leaveEvent(QEvent* e) override {
        QFrame::leaveEvent(e);
        setStyleSheet(
            "QFrame { border: 1px solid rgba(255,255,255,0.2);"
            "border-radius: 12px; }");
    }
private:
    int m_index;
};

// ══════════════════════════════════════════════════════════════════
//  MainWindow
// ══════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), bgMovie(nullptr)
{
    setMinimumSize(1100, 700);
    setWindowTitle("URdorm");

    auto* central = new QWidget(this);
    setCentralWidget(central);
    central->setLayout(new QVBoxLayout());
    central->layout()->setContentsMargins(0, 0, 0, 0);
    central->layout()->setSpacing(0);

    bgLabel = new QLabel(central);
    bgLabel->setScaledContents(true);
    bgLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    bgLabel->setGeometry(0, 0, 1100, 700);
    bgLabel->hide();

    stack = new QStackedWidget(central);
    central->layout()->addWidget(stack);

    // Top bar
    topBar = new QWidget(central);
    topBar->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    topBar->setFixedHeight(130);
    topBar->setStyleSheet("background: transparent;");
    setupTopBar();

    floatingNav = new QWidget(central);
    floatingNav->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    floatingNav->setStyleSheet("background: transparent;");
    setupFloatingNav();

    setupPages();
    applyTheme();

    topBar->raise();
    floatingNav->raise();
    bgLabel->lower();

    if (!tabs.empty()) tabs[0]->setActive(true);

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, [this](int) { applyTheme(); });
}

void MainWindow::setupTopBar() {
    auto* layout = new QHBoxLayout(topBar);
    layout->setContentsMargins(24, 8, 24, 8);
    layout->setSpacing(14);

    auto* logoLbl = new QLabel(topBar);
    QPixmap logoPx(":/assets/images/logo.png");
    if (!logoPx.isNull())
        logoLbl->setPixmap(
            logoPx.scaled(150, 150, Qt::KeepAspectRatio,
                          Qt::SmoothTransformation));
    else
        logoLbl->setText("◈");
    logoLbl->setFixedSize(150, 150);
    logoLbl->setAlignment(Qt::AlignCenter);
    logoLbl->setStyleSheet(
        "background: transparent; color: white; font-size: 70px;");

    // ── Center: page title ───────────────────────────────────────
    pageTitleLabel = new QLabel("MY CARD", topBar);
    pageTitleLabel->setAlignment(Qt::AlignCenter);
    QFont ptf;
    ptf.setFamily("Georgia");
    ptf.setPixelSize(26);
    ptf.setBold(true);
    ptf.setLetterSpacing(QFont::AbsoluteSpacing, 5);
    pageTitleLabel->setFont(ptf);
    pageTitleLabel->setStyleSheet(
        "color: #FFFFFF; background: transparent;");

    // ── Right: bell + avatar ─────────────────────────────────────
    auto* student = DataManager::instance().currentUser;

    auto* bellContainer = new QWidget(topBar);
    bellContainer->setFixedSize(44, 44);
    bellContainer->setStyleSheet("background: transparent;");

    auto* bellBtnLocal = new QPushButton("🔔", bellContainer);
    bellBtnLocal->setFixedSize(36, 36);
    bellBtnLocal->setStyleSheet(
        "QPushButton { background: transparent;"
        "border: none; font-size: 22px; }");
    bellBtnLocal->move(0, 4);
    connect(bellBtnLocal, &QPushButton::clicked,
            this, [this]() { switchToPage(2); });

    bellBadge = new QLabel("0", bellContainer);
    bellBadge->setFixedSize(18, 18);
    bellBadge->setAlignment(Qt::AlignCenter);
    bellBadge->setStyleSheet(
        "background: #F44336; color: white;"
        "border-radius: 9px; font-size: 10px; font-weight: 700;");
    bellBadge->move(22, 0);

    int unread = 0;
    for (auto& n : DataManager::instance().notifications)
        if (!n.isRead) unread++;
    bellBadge->setText(QString::number(unread));
    bellBadge->setVisible(unread > 0);

    auto* avatar = new QLabel(topBar);
    avatar->setFixedSize(46, 46);
    if (student && !student->photoPath.isEmpty()) {
        QPixmap px(student->photoPath);
        if (!px.isNull()) {
            QPixmap sc = px.scaled(
                46, 46,
                Qt::KeepAspectRatioByExpanding,
                Qt::SmoothTransformation);
            QPixmap circle(46, 46);
            circle.fill(Qt::transparent);
            QPainter painter(&circle);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setBrush(QBrush(sc));
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(0, 0, 46, 46);
            avatar->setPixmap(circle);
        }
    }
    avatar->setStyleSheet(QString(
                              "border-radius: 23px; background: %1;")
                              .arg(ThemeManager::instance().primary().name()));

    layout->addWidget(logoLbl);
    layout->addStretch(1);
    layout->addWidget(pageTitleLabel);
    layout->addStretch(1);
    layout->addWidget(bellContainer);
    layout->addWidget(avatar);
}

void MainWindow::setupFloatingNav() {
    auto* layout = new QHBoxLayout(floatingNav);
    layout->setContentsMargins(50, 6, 50, 12);
    layout->setSpacing(0);

    QStringList iconPaths = {
        ":/assets/images/card.jpg",
        ":/assets/images/services.jpg",
        ":/assets/images/alerts.jpg",
        ":/assets/images/water.jpg",
        ":/assets/images/info.jpg",
        ":/assets/images/themes.jpg"
    };

    for (int i = 0; i < 6; ++i) {
        auto* tab = new TabButton(iconPaths[i], floatingNav);
        tabs.push_back(tab);
        layout->addWidget(tab, 1, Qt::AlignCenter);

        if (i < 5) {
            connect(tab, &TabButton::clicked,
                    this, [this, i]() { switchToPage(i); });
        } else {
            connect(tab, &TabButton::clicked,
                    this, &MainWindow::openThemeSwitcher);
        }
    }
}

void MainWindow::setupPages() {
    cardPage     = new StudentCardPage(this);
    servicesPage = new ServicesPage(this);
    notifPage    = new NotificationsPage(this);
    waterPage    = new WaterStatusPage(this);
    infoPage     = new InfoPage(this);

    stack->addWidget(cardPage);
    stack->addWidget(servicesPage);
    stack->addWidget(notifPage);
    stack->addWidget(waterPage);
    stack->addWidget(infoPage);
    stack->setCurrentIndex(0);

    connect(servicesPage, &ServicesPage::notificationAdded,
            this, [this]() {
                int unread = 0;
                for (auto& n : DataManager::instance().notifications)
                    if (!n.isRead) unread++;
                updateBellBadge(unread);
            });
    connect(notifPage, &NotificationsPage::unreadCountChanged,
            this, &MainWindow::updateBellBadge);
}

void MainWindow::repositionNav() {
    topBar->setGeometry(0, 0, width(), 130);
    floatingNav->setGeometry(0, height() - 88, width(), 88);
}

void MainWindow::switchToPage(int index) {
    if (index == currentPageIndex) return;
    fadeToPage(index);
}

void MainWindow::fadeToPage(int index) {
    auto* current = stack->currentWidget();
    auto* outEff  = new QGraphicsOpacityEffect(current);
    current->setGraphicsEffect(outEff);

    auto* outAnim = new QPropertyAnimation(outEff, "opacity", this);
    outAnim->setDuration(120);
    outAnim->setStartValue(1.0);
    outAnim->setEndValue(0.0);

    connect(outAnim, &QPropertyAnimation::finished, this,
            [this, index, current]() {
                current->setGraphicsEffect(nullptr);

                for (int i = 0; i < (int)tabs.size(); ++i)
                    tabs[i]->setActive(i == index);

                QStringList upper = pageNames;
                for (auto& s : upper) s = s.toUpper();
                pageTitleLabel->setText(upper[index]);

                stack->setCurrentIndex(index);
                currentPageIndex = index;

                auto* newW  = stack->currentWidget();
                auto* inEff = new QGraphicsOpacityEffect(newW);
                newW->setGraphicsEffect(inEff);
                inEff->setOpacity(0.0);

                auto* inAnim = new QPropertyAnimation(
                    inEff, "opacity", newW);
                inAnim->setDuration(150);
                inAnim->setStartValue(0.0);
                inAnim->setEndValue(1.0);
                connect(inAnim, &QPropertyAnimation::finished,
                        newW, [newW]() {
                            newW->setGraphicsEffect(nullptr);
                        });
                inAnim->start(QAbstractAnimation::DeleteWhenStopped);

                if (index == 2) notifPage->refreshNotifications();
            });
    outAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::applyTheme() {
    setStyleSheet(ThemeManager::instance().getStyleSheet());

    int     theme = ThemeManager::instance().currentTheme();
    QString gif   = ThemeManager::instance().gifPath();

    if (theme != 0 && !gif.isEmpty()) {
        if (!bgMovie) {
            bgMovie = new QMovie(gif, QByteArray(), this);
        } else {
            bgMovie->stop();
            bgMovie->setFileName(gif);
        }
        bgLabel->setMovie(bgMovie);
        bgLabel->setGeometry(0, 0, width(), height());
        auto* eff = new QGraphicsOpacityEffect(bgLabel);
        eff->setOpacity(0.25);
        bgLabel->setGraphicsEffect(eff);
        bgMovie->start();
        bgLabel->show();
        bgLabel->lower();
    } else {
        if (bgMovie) bgMovie->stop();
        bgLabel->hide();
    }

    for (auto* t : tabs) t->setActive(false);
    if (currentPageIndex < (int)tabs.size())
        tabs[currentPageIndex]->setActive(true);

    topBar->raise();
    floatingNav->raise();
}

void MainWindow::updateBellBadge(int count) {
    bellBadge->setText(QString::number(count));
    bellBadge->setVisible(count > 0);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    repositionNav();
    if (bgLabel)
        bgLabel->setGeometry(0, 0, width(), height());
    topBar->raise();
    floatingNav->raise();
}

void MainWindow::openThemeSwitcher() {
    auto* dlg = new QDialog(this, Qt::Popup);
    dlg->setWindowTitle("Theme");
    dlg->setFixedSize(340, 180);
    dlg->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #1A1A2E, stop:1 #2D1B4E);"
        "border-radius: 16px;");

    auto* l = new QVBoxLayout(dlg);
    l->setContentsMargins(16, 16, 16, 16);
    l->setSpacing(12);

    auto* title = new QLabel("Choose Theme", dlg);
    QFont tf; tf.setPixelSize(22); tf.setBold(true);
    title->setFont(tf);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #FFFFFF; background: transparent;");
    l->addWidget(title);

    auto* row = new QHBoxLayout();
    row->setSpacing(12);
    row->setAlignment(Qt::AlignCenter);

    struct ThemeInfo { QString name; QColor color; };
    QList<ThemeInfo> themes = {
        {"Default", QColor("#D60270")},
        {"Denji",   QColor("#FF3D00")},
        {"Reze",    QColor("#29B6F6")}
    };

    for (int i = 0; i < 3; ++i) {
        auto* swatch = new ThemeSwatch(
            i, themes[i].name, themes[i].color, dlg);
        swatch->onSelected = [this, dlg](int idx) {
            ThemeManager::instance().setTheme(idx);
            applyTheme();
            dlg->close();
        };
        row->addWidget(swatch);
    }
    l->addLayout(row);
    dlg->exec();
}