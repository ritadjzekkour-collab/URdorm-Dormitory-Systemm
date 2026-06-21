#include "ThemeManager.h"

// URdorm — Developed by Zekkour Ritadj

ThemeManager& ThemeManager::instance() {
    static ThemeManager inst;
    return inst;
}

ThemeManager::ThemeManager() : QObject(nullptr), m_currentTheme(0) {}

void ThemeManager::setTheme(int index) {
    m_currentTheme = index;
    emit themeChanged(index);
}

QColor ThemeManager::primary() const {
    switch (m_currentTheme) {
    case 1: return QColor("#FF3D00");  // Denji red-orange
    case 2: return QColor("#29B6F6");  // Reze icy blue
    default: return QColor("#D60270"); // Bisexual pink
    }
}

QColor ThemeManager::secondary() const {
    switch (m_currentTheme) {
    case 1: return QColor("#FF6D00");  // Denji dark orange
    case 2: return QColor("#0D47A1");  // Reze deep blue
    default: return QColor("#0038A8"); // Bisexual blue
    }
}

QColor ThemeManager::bgColor() const {
    switch (m_currentTheme) {
    case 1: return QColor("#1A1A1A");  // Denji near black
    case 2: return QColor("#0A0E27");  // Reze dark navy
    default: return QColor("#9B4F96"); // Bisexual purple mid
    }
}

QColor ThemeManager::cardColor() const {
    switch (m_currentTheme) {
    case 1: return QColor("#2A2A2A");
    case 2: return QColor("#0D1333");
    default: return QColor("rgba(255,255,255,0.15)");
    }
}

QColor ThemeManager::textColor() const {
    // Always white — works on all bold/dark backgrounds
    return QColor("#FFFFFF");
}

QString ThemeManager::gifPath() const {
    switch (m_currentTheme) {
    case 1: return ":/assets/images/denji.gif";
    case 2: return ":/assets/images/reze.gif";
    default: return "";
    }
}

QString ThemeManager::soundPath() const {
    switch (m_currentTheme) {
    case 1: return ":/assets/sounds/notif_denji.wav";
    case 2: return ":/assets/sounds/notif_reze.wav";
    default: return "";
    }
}

float ThemeManager::gifOpacity() const {
    return 0.25f; // 25% opacity — GIF visible but text stays clear
}

QString ThemeManager::getStyleSheet() const {
    QString pri  = primary().name();
    QString sec  = secondary().name();
    QString card = cardColor().name();

    QString bgGradient;
    if (m_currentTheme == 0) {
        // Bold bisexual flag gradient: hot pink → purple → bold blue
        bgGradient = "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
                     "stop:0 #D60270, stop:0.5 #9B4F96, stop:1 #0038A8);";
    } else if (m_currentTheme == 1) {
        // Denji: dark with orange hint
        bgGradient = "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
                     "stop:0 #1A1A1A, stop:1 #2A1500);";
    } else {
        // Reze: dark navy with blue hint
        bgGradient = "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
                     "stop:0 #0A0E27, stop:1 #0D1F3C);";
    }

    return QString(R"(
/* ── URdorm Global Stylesheet — Zekkour Ritadj ── */

QMainWindow, QDialog {
    %1
    color: #FFFFFF;
}

QWidget {
    color: #FFFFFF;
    font-family: "Segoe UI", "Helvetica Neue", Arial, sans-serif;
    font-size: 14px;
}

/* ── Scroll ── */
QScrollArea {
    border: none;
    background: transparent;
}
QScrollArea > QWidget > QWidget {
    background: transparent;
}
QScrollBar:vertical {
    background: rgba(255,255,255,0.08);
    width: 8px;
    border-radius: 4px;
}
QScrollBar::handle:vertical {
    background: %2;
    border-radius: 4px;
    min-height: 20px;
}
QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical { height: 0px; }

/* ── Inputs ── */
QLineEdit {
    background: rgba(255,255,255,0.15);
    color: #FFFFFF;
    border: 2px solid rgba(255,255,255,0.35);
    border-radius: 10px;
    padding: 10px 14px;
    font-size: 14px;
    font-weight: 500;
}
QLineEdit:focus {
    border: 2px solid %2;
    background: rgba(255,255,255,0.22);
}
QLineEdit::placeholder {
    color: rgba(255,255,255,0.55);
}

QTextEdit {
    background: rgba(255,255,255,0.15);
    color: #FFFFFF;
    border: 2px solid rgba(255,255,255,0.35);
    border-radius: 10px;
    padding: 10px 14px;
    font-size: 14px;
    font-weight: 500;
}
QTextEdit:focus {
    border: 2px solid %2;
    background: rgba(255,255,255,0.22);
}

QComboBox {
    background: rgba(255,255,255,0.15);
    color: #FFFFFF;
    border: 2px solid rgba(255,255,255,0.35);
    border-radius: 10px;
    padding: 10px 14px;
    font-size: 14px;
    font-weight: 500;
    min-height: 40px;
}
QComboBox:focus {
    border: 2px solid %2;
}
QComboBox::drop-down {
    border: none;
    width: 28px;
}
QComboBox QAbstractItemView {
    background: #2A2A2A;
    color: #FFFFFF;
    border: 1px solid rgba(255,255,255,0.2);
    border-radius: 10px;
    selection-background-color: %2;
    selection-color: #FFFFFF;
    font-size: 14px;
    padding: 4px;
}

/* ── Buttons ── */
QPushButton {
    background-color: %2;
    color: #FFFFFF;
    border: none;
    border-radius: 10px;
    padding: 10px 22px;
    font-size: 15px;
    font-weight: 700;
    letter-spacing: 0.5px;
}
QPushButton:hover {
    background-color: %3;
}
QPushButton:pressed {
    background-color: %2;
}

/* ── Tabs ── */
QTabWidget::pane {
    border: none;
    background: transparent;
}
QTabBar::tab {
    background: transparent;
    color: rgba(255,255,255,0.6);
    padding: 12px 26px;
    border-bottom: 3px solid transparent;
    font-size: 14px;
    font-weight: 500;
}
QTabBar::tab:selected {
    color: #FFFFFF;
    border-bottom: 3px solid %2;
    font-weight: 700;
}
QTabBar::tab:hover {
    color: #FFFFFF;
}

/* ── Lists ── */
QListWidget {
    background: rgba(255,255,255,0.10);
    color: #FFFFFF;
    border: 1px solid rgba(255,255,255,0.15);
    border-radius: 10px;
    padding: 6px;
    font-size: 14px;
}
QListWidget::item {
    padding: 10px 14px;
    border-radius: 8px;
    color: #FFFFFF;
}
QListWidget::item:selected {
    background: %2;
    color: #FFFFFF;
}
QListWidget::item:hover {
    background: rgba(255,255,255,0.12);
}

/* ── Labels ── */
QLabel {
    font-size: 14px;
    font-weight: 500;
    color: #FFFFFF;
    background: transparent;
}
QLabel#pageTitle {
    font-size: 24pt;
    font-weight: 800;
    color: #FFFFFF;
}
QLabel#sectionTitle {
    font-size: 18pt;
    font-weight: 700;
    color: #FFFFFF;
}
QLabel#subtitleLabel {
    font-size: 13pt;
    font-weight: 400;
    color: rgba(255,255,255,0.80);
}
QLabel#errorLabel {
    font-size: 13px;
    color: #FF6B6B;
    font-weight: 600;
}

/* ── Cards ── */
QWidget#card {
    background: rgba(255,255,255,0.12);
    border-radius: 16px;
    border: 1px solid rgba(255,255,255,0.20);
}

/* ── Top bar ── */
QWidget#topBar {
    background: rgba(0,0,0,0.25);
    border-bottom: 1px solid rgba(255,255,255,0.10);
}

/* ── Bottom bar ── */
QWidget#bottomBar {
    background: rgba(0,0,0,0.30);
    border-top: 1px solid rgba(255,255,255,0.10);
}

    )").arg(bgGradient, pri, sec);
}