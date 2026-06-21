#include "NotificationsPage.h"
#include "DataManager.h"
#include "ThemeManager.h"

#include <QHBoxLayout>
#include <QFrame>
#include <QMouseEvent>
#include <QShowEvent>

// ══════════════════════════════════════════════════════════════════
//  NotificationCard
// ══════════════════════════════════════════════════════════════════
NotificationCard::NotificationCard(int notifIndex, QWidget* parent)
    : QWidget(parent), m_index(notifIndex)
{
    auto& notif = DataManager::instance().notifications[notifIndex];

    setFixedHeight(92);
    setCursor(Qt::PointingHandCursor);

    auto getBg = [&]() -> QString {
        if (notif.isRead) return "#FAFAFA";
        if (notif.type == "hosting")  return "#FFF3E0";
        if (notif.type == "water") {
            if (notif.title.contains("Unavailable", Qt::CaseInsensitive)) return "#FFEBEE";
            return "#E3F2FD";
        }
        return QString("#F3E5F5");
    };

    setStyleSheet(QString("QWidget { background: %1; border-radius: 10px; }"
                          "QWidget:hover { background: %2; }")
                      .arg(getBg(), notif.isRead ? "#F0F0F0" : getBg()));

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 10, 12, 10);
    layout->setSpacing(12);

    auto* iconLbl = new QLabel(this);
    iconLbl->setFixedSize(55, 55);
    iconLbl->setAlignment(Qt::AlignCenter);

    QString iconPath;
    if (notif.type == "hosting") {
        iconPath = ":/assets/images/notif_hosting.jpg";
    } else if (notif.type == "water") {
        iconPath = ":/assets/images/notif_water.jpg";
    } else {
        iconPath = ":/assets/images/notif_announce.jpg";
    }

    QPixmap px(iconPath);
    if (!px.isNull()) {
        iconLbl->setPixmap(px.scaled(
            55, 55, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        iconLbl->setText("•");
    }
    iconLbl->setStyleSheet("background: transparent;");

    auto* centerLayout = new QVBoxLayout();
    centerLayout->setSpacing(3);

    auto* titleLbl = new QLabel(notif.title, this);
    QFont tf; tf.setPixelSize(30); tf.setBold(true); titleLbl->setFont(tf);
    titleLbl->setStyleSheet("color: #212121; background: transparent;");

    auto* descLbl = new QLabel(notif.description, this);
    QFont df; df.setPixelSize(25); descLbl->setFont(df);
    descLbl->setStyleSheet("color: #0D47A1; background: transparent;");
    descLbl->setWordWrap(true);

    centerLayout->addWidget(titleLbl);
    centerLayout->addWidget(descLbl);

    auto* rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);
    rightLayout->setSpacing(6);

    auto* timeLbl = new QLabel(notif.timestamp, this);
    QFont timeF; timeF.setPixelSize(20);
    timeLbl->setFont(timeF);
    timeLbl->setStyleSheet("color: #9E9E9E; background: transparent;");
    timeLbl->setAlignment(Qt::AlignRight);

    auto* dot = new QLabel(this);
    dot->setFixedSize(10, 10);
    dot->setStyleSheet("background: #2196F3; border-radius: 5px;");
    dot->setVisible(!notif.isRead);

    rightLayout->addWidget(timeLbl);
    rightLayout->addWidget(dot, 0, Qt::AlignRight);
    rightLayout->addStretch();

    layout->addWidget(iconLbl, 0, Qt::AlignVCenter);
    layout->addLayout(centerLayout, 1);
    layout->addLayout(rightLayout);
}

void NotificationCard::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        auto& notif = DataManager::instance().notifications[m_index];
        if (!notif.isRead) {
            notif.isRead = true;
            setStyleSheet("QWidget { background: #FAFAFA; border-radius: 10px; }");
            emit readStateChanged();
        }
    }
    QWidget::mousePressEvent(e);
}

// ══════════════════════════════════════════════════════════════════
//  NotificationsPage
// ══════════════════════════════════════════════════════════════════
NotificationsPage::NotificationsPage(QWidget* parent) : QWidget(parent) {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 16, 20, 16);
    mainLayout->setSpacing(12);

    auto* title = new QLabel("NOTIFICATIONS", this);
    QFont tf;
    tf.setFamily("Georgia");
    tf.setPixelSize(38);
    tf.setBold(true);
    tf.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    title->setFont(tf);
    title->setAlignment(Qt::AlignCenter);   // ← centered
    title->setObjectName("pageTitle");
    title->setStyleSheet("color: #FFFFFF; background: transparent;");
    mainLayout->addWidget(title);

    // Unread badge row — kept separate from the title so it doesn't
    // sit beside/overlap it
    auto* badgeRow = new QHBoxLayout();
    unreadBadgeLabel = new QLabel("0 unread", this);
    QFont ubF; ubF.setPixelSize(18); ubF.setBold(true);
    unreadBadgeLabel->setFont(ubF);
    unreadBadgeLabel->setStyleSheet(QString(
                                        "background: %1; color: white; border-radius: 10px; padding: 4px 12px;"
                                        ).arg(ThemeManager::instance().primary().name()));
    badgeRow->addStretch();
    badgeRow->addWidget(unreadBadgeLabel);
    badgeRow->addStretch();
    mainLayout->addLayout(badgeRow);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    listContainer = new QWidget(scrollArea);
    listLayout = new QVBoxLayout(listContainer);
    listLayout->setSpacing(8);
    listLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea->setWidget(listContainer);
    mainLayout->addWidget(scrollArea, 1);

    buildList();
}

void NotificationsPage::buildList() {
    while (QLayoutItem* item = listLayout->takeAt(0)) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    auto& notifs = DataManager::instance().notifications;
    for (int i = 0; i < (int)notifs.size(); ++i) {
        auto* card = new NotificationCard(i, listContainer);
        connect(card, &NotificationCard::readStateChanged, this, [this]() {
            int unread = countUnread();
            emit unreadCountChanged(unread);
            unreadBadgeLabel->setText(QString("%1 unread").arg(unread));
        });
        listLayout->addWidget(card);
    }
    listLayout->addStretch();

    int unread = countUnread();
    unreadBadgeLabel->setText(QString("%1 unread").arg(unread));
    unreadBadgeLabel->setVisible(unread > 0);
}

void NotificationsPage::refreshNotifications() {
    buildList();
    emit unreadCountChanged(countUnread());
}

int NotificationsPage::countUnread() const {
    int count = 0;
    for (auto& n : DataManager::instance().notifications)
        if (!n.isRead) count++;
    return count;
}

void NotificationsPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    buildList();
    emit unreadCountChanged(countUnread());
}