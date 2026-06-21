#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QMovie>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QFrame>
#include <QMouseEvent>
#include <vector>

class StudentCardPage;
class ServicesPage;
class NotificationsPage;
class WaterStatusPage;
class InfoPage;

// ── Floating icon button ─────────────────────────────────────────
class TabButton : public QWidget {
    Q_OBJECT
public:
    TabButton(const QString& iconPath, QWidget* parent = nullptr);
    void setActive(bool active);
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;
private:
    QPixmap m_icon;
    bool    m_active = false;
    int     m_size   = 46;
};

// ── Main window ──────────────────────────────────────────────────
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    void applyTheme();
    void updateBellBadge(int count);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void switchToPage(int index);
    void openThemeSwitcher();

private:
    void setupTopBar();
    void setupFloatingNav();
    void setupPages();
    void fadeToPage(int index);
    void repositionNav();

    // Top bar — logo only, centered title
    QWidget*  topBar         = nullptr;
    QLabel*   pageTitleLabel = nullptr;
    QLabel*   bellBadge      = nullptr;

    // GIF background
    QLabel*   bgLabel        = nullptr;
    QMovie*   bgMovie        = nullptr;

    // Floating nav icons (6 now — last one opens theme switcher)
    QWidget*  floatingNav    = nullptr;
    std::vector<TabButton*> tabs;

    // Pages
    QStackedWidget*    stack        = nullptr;
    StudentCardPage*   cardPage     = nullptr;
    ServicesPage*      servicesPage = nullptr;
    NotificationsPage* notifPage    = nullptr;
    WaterStatusPage*   waterPage    = nullptr;
    InfoPage*          infoPage     = nullptr;

    int currentPageIndex = 0;

    const QStringList pageNames = {
        "My Card", "Services", "Alerts", "Water Status", "Info & Rules"
    };
};