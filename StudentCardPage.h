#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

class FlipCard : public QWidget {
    Q_OBJECT
public:
    explicit FlipCard(QWidget* parent = nullptr);
    void setupFrontFace(const QString& photoPath, const QString& name,
                        const QString& id, const QString& major,
                        const QString& room, const QString& pavilion);
    void setupBackFace(const QString& id);
    QLabel* makeRectPhoto(const QString& path, int w, int h, QWidget* parent);
    QLabel* makeCircularPhoto(const QString& path, int size, QWidget* parent);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent*) override;

private:
    QStackedWidget* cardStack   = nullptr;
    QWidget*        frontWidget = nullptr;
    QWidget*        backWidget  = nullptr;
    bool            isFlipped   = false;
    bool            animating   = false;
    void flipCard();
};

class StudentCardPage : public QWidget {
    Q_OBJECT
public:
    explicit StudentCardPage(QWidget* parent = nullptr);
private:
    void setupStudentView();
    void setupAdminView();
    void setupChiefView();
    void setupInspectionView();
};