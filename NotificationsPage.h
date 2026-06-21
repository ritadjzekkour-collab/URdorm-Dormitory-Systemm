#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
class NotificationCard : public QWidget {
    Q_OBJECT
public:
    explicit NotificationCard(int notifIndex, QWidget* parent = nullptr);
signals:
    void readStateChanged();
protected:
    void mousePressEvent(QMouseEvent* event) override;
private:
    int m_index;
};

class NotificationsPage : public QWidget {
    Q_OBJECT
public:
    explicit NotificationsPage(QWidget* parent = nullptr);
    void refreshNotifications();
signals:
    void unreadCountChanged(int count);
protected:
    void showEvent(QShowEvent* event) override;
private:
    QVBoxLayout* listLayout;
    QWidget*     listContainer;
    QScrollArea* scrollArea;
    QLabel*      unreadBadgeLabel;
    void buildList();
    int countUnread() const;
};