#pragma once
#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class SplashScreen : public QWidget {
    Q_OBJECT
public:
    explicit SplashScreen(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    QLabel* logoLabel;
    QLabel* nameLabel;
    QLabel* subtitleLabel;
    QGraphicsOpacityEffect* logoOpacity;
    QGraphicsOpacityEffect* nameOpacity;

    void startAnimations();
};