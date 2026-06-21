#include "WaterStatusPage.h"
#include "DataManager.h"
#include "ThemeManager.h"

#include <QPainter>
#include <QPaintEvent>
#include <QScrollArea>
#include <QFrame>
#include <QHBoxLayout>

// ══════════════════════════════════════════════════════════════════
//  WaterCircle
// ══════════════════════════════════════════════════════════════════
WaterCircle::WaterCircle(QWidget* parent) : QWidget(parent) {
    setFixedSize(220, 220);
    m_available = DataManager::instance().waterAvailable;
    startPulse();
}

void WaterCircle::updateState(bool available) {
    m_available = available;
    update();
}

void WaterCircle::startPulse() {
    m_pulseAnim = new QPropertyAnimation(this, "pulseScale", this);
    m_pulseAnim->setDuration(1500);
    m_pulseAnim->setKeyValueAt(0.0,  1.00f);
    m_pulseAnim->setKeyValueAt(0.5,  1.08f);
    m_pulseAnim->setKeyValueAt(1.0,  1.00f);
    m_pulseAnim->setEasingCurve(QEasingCurve::SineCurve);
    m_pulseAnim->setLoopCount(-1); // Infinite
    m_pulseAnim->start();
}

void WaterCircle::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPointF center(width() / 2.0, height() / 2.0);
    QColor mainColor = m_available ? QColor("#1565C0") : QColor("#C62828");
    QColor glowColor = m_available ? QColor("#42A5F5") : QColor("#EF5350");

    // Outer glow rings (affected by pulseScale)
    float ps = m_pulseScale;
    auto drawGlow = [&](float baseRadius, float opacity) {
        painter.setOpacity(opacity);
        painter.setBrush(glowColor);
        painter.setPen(Qt::NoPen);
        float r = baseRadius * ps;
        painter.drawEllipse(center, r, r);
    };

    drawGlow(110, 0.08f);
    drawGlow(100, 0.13f);
    drawGlow(92,  0.08f);

    // Main circle
    painter.setOpacity(1.0);
    painter.setBrush(mainColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, 82.0, 82.0);

    // Inner shine
    QRadialGradient shine(center.x() - 20, center.y() - 25, 50);
    shine.setColorAt(0, QColor(255, 255, 255, 55));
    shine.setColorAt(1, Qt::transparent);
    painter.setBrush(shine);
    painter.drawEllipse(center, 82.0, 82.0);

    // Text
    painter.setOpacity(1.0);
    painter.setPen(Qt::white);
    QFont f;
    f.setPointSize(13);
    f.setBold(true);
    painter.setFont(f);

    QString line1 = m_available ? "✓" : "✗";
    QString line2 = m_available ? "Available" : "Unavailable";

    QRect topHalf(0, int(center.y()) - 35, width(), 30);
    QRect botHalf(0, int(center.y()) + 5,  width(), 30);

    QFont bigFont; bigFont.setPointSize(22); bigFont.setBold(true);
    painter.setFont(bigFont);
    painter.drawText(topHalf, Qt::AlignCenter, line1);

    QFont smallFont; smallFont.setPointSize(11); smallFont.setBold(true);
    painter.setFont(smallFont);
    painter.drawText(botHalf, Qt::AlignCenter, line2);
}

// ══════════════════════════════════════════════════════════════════
//  WaterStatusPage
// ══════════════════════════════════════════════════════════════════
WaterStatusPage::WaterStatusPage(QWidget* parent) : QWidget(parent) {
    auto* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    auto* content = new QWidget(scroll);
    auto* mainLayout = new QVBoxLayout(content);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(16);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->addWidget(scroll);
    scroll->setWidget(content);

    // Title
    auto* title = new QLabel("Water Status", this);
    QFont tf; tf.setPointSize(22); tf.setBold(true); title->setFont(tf);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("pageTitle");
    mainLayout->addWidget(title);

    // Water circle
    waterCircle = new WaterCircle(content);
    mainLayout->addWidget(waterCircle, 0, Qt::AlignCenter);

    // Last updated
    lastUpdatedLabel = new QLabel("Last updated: " + DataManager::instance().waterLastUpdated, content);
    lastUpdatedLabel->setAlignment(Qt::AlignCenter);
    lastUpdatedLabel->setStyleSheet("color: #9E9E9E; font-size: 12px;");
    mainLayout->addWidget(lastUpdatedLabel);

    // Toggle button (admin/chief only)
    auto* user = DataManager::instance().currentUser;
    bool canToggle = user && (user->role == "administration" || user->role == "chief");

    toggleBtn = new QPushButton("🔄  Toggle Water Status", content);
    toggleBtn->setFixedHeight(42);
    toggleBtn->setCursor(Qt::PointingHandCursor);
    toggleBtn->setStyleSheet(QString(
                                 "QPushButton { background: %1; color: white; border-radius: 10px;"
                                 "font-size: 13px; font-weight: 700; padding: 0 24px; }"
                                 "QPushButton:hover { background: %2; }"
                                 ).arg(ThemeManager::instance().primary().name(),
                                      ThemeManager::instance().secondary().name()));
    toggleBtn->setVisible(canToggle);

    connect(toggleBtn, &QPushButton::clicked, this, [this]() {
        auto& dm = DataManager::instance();
        dm.waterAvailable = !dm.waterAvailable;

        // Prepend history entry
        QString newEntry = dm.waterAvailable
                               ? "Just now — Restored"
                               : "Just now — Unavailable";
        dm.waterHistory.insert(dm.waterHistory.begin(), newEntry);
        dm.waterLastUpdated = "Just now";

        // Add notification
        DataManager::Notification notif;
        notif.title = dm.waterAvailable ? "Water Restored" : "Water Unavailable";
        notif.description = dm.waterAvailable
                                ? "Water supply has been restored."
                                : "Water supply has been cut temporarily.";
        notif.timestamp = "Just now";
        notif.type = "water";
        notif.isRead = false;
        dm.notifications.insert(dm.notifications.begin(), notif);

        waterCircle->updateState(dm.waterAvailable);
        lastUpdatedLabel->setText("Last updated: " + dm.waterLastUpdated);
        buildHistory();
    });

    mainLayout->addWidget(toggleBtn, 0, Qt::AlignCenter);

    // Divider
    auto* div = new QFrame(content);
    div->setFrameShape(QFrame::HLine);
    div->setStyleSheet("background: rgba(0,0,0,0.1); border: none; max-height: 1px;");
    mainLayout->addWidget(div);

    // History section
    auto* histTitle = new QLabel("Recent History", content);
    QFont ht; ht.setPointSize(13); ht.setBold(true); histTitle->setFont(ht);
    mainLayout->addWidget(histTitle);

    auto* histContainer = new QWidget(content);
    historyLayout = new QVBoxLayout(histContainer);
    historyLayout->setSpacing(6);
    historyLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(histContainer);

    buildHistory();
    mainLayout->addStretch();
}

void WaterStatusPage::buildHistory() {
    while (QLayoutItem* item = historyLayout->takeAt(0)) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    for (auto& entry : DataManager::instance().waterHistory) {
        auto* row = new QWidget();
        row->setStyleSheet("background: rgba(0,0,0,0.03); border-radius: 8px;");
        auto* rl = new QHBoxLayout(row);
        rl->setContentsMargins(12, 8, 12, 8);

        // Dot
        auto* dot = new QLabel(row);
        dot->setFixedSize(8, 8);
        bool isAvail = entry.contains("Available") || entry.contains("Restored");
        dot->setStyleSheet(QString("background: %1; border-radius: 4px;")
                               .arg(isAvail ? "#4CAF50" : "#F44336"));

        auto* lbl = new QLabel(entry, row);
        lbl->setStyleSheet("font-size: 12px; background: transparent;");

        rl->addWidget(dot);
        rl->addWidget(lbl, 1);

        historyLayout->addWidget(row);
    }
}

void WaterStatusPage::refreshHistory() {
    buildHistory();
    waterCircle->updateState(DataManager::instance().waterAvailable);
    lastUpdatedLabel->setText("Last updated: " + DataManager::instance().waterLastUpdated);
}