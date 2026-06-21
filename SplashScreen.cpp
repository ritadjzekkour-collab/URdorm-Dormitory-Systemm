#include "SplashScreen.h"
#include "LoginDialog.h"
#include "ThemeManager.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QPainter>
#include <QLinearGradient>
#include <QScreen>
#include <QApplication>
#include <QPoint>

SplashScreen::SplashScreen(QWidget* parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    setAttribute(Qt::WA_TranslucentBackground, false);

    QScreen* screen = QApplication::primaryScreen();
    setGeometry(screen->geometry());

    // Central widget layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(16);

    // Logo
    logoLabel = new QLabel(this);
    QPixmap logoPix(":/assets/images/logo.png");
    if (!logoPix.isNull()) {
        logoLabel->setPixmap(logoPix.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel->setText("🏠");
        logoLabel->setStyleSheet("font-size: 80px;");
    }
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setFixedSize(120, 120);

    // Name label
    nameLabel = new QLabel("URdorm", this);
    nameLabel->setAlignment(Qt::AlignCenter);
    QFont nameFont;
    nameFont.setPointSize(48);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);
    nameLabel->setStyleSheet(QString("color: %1; letter-spacing: 2px;")
                                 .arg(ThemeManager::instance().primary().name()));

    // Subtitle
    subtitleLabel = new QLabel("University Residence Management", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subFont;
    subFont.setPointSize(16);
    subtitleLabel->setFont(subFont);
    subtitleLabel->setStyleSheet("color: #9E9E9E; letter-spacing: 1px;");

    mainLayout->addStretch();
    mainLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(nameLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(subtitleLabel, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // Opacity effects
    logoOpacity = new QGraphicsOpacityEffect(this);
    logoOpacity->setOpacity(0.0);
    logoLabel->setGraphicsEffect(logoOpacity);

    nameOpacity = new QGraphicsOpacityEffect(this);
    nameOpacity->setOpacity(0.0);
    nameLabel->setGraphicsEffect(nameOpacity);
}

void SplashScreen::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QLinearGradient grad(0, 0, width(), height());
    grad.setColorAt(0, QColor("#F3E5F5"));
    grad.setColorAt(1, QColor("#E8EAF6"));
    painter.fillRect(rect(), grad);
}

void SplashScreen::showEvent(QShowEvent* event) {
    Q_UNUSED(event);
    QTimer::singleShot(50, this, &SplashScreen::startAnimations);
}

void SplashScreen::startAnimations() {
    // Logo fade in 0→1 over 1200ms
    QPropertyAnimation* logoFade = new QPropertyAnimation(logoOpacity, "opacity", this);
    logoFade->setDuration(1200);
    logoFade->setStartValue(0.0);
    logoFade->setEndValue(1.0);
    logoFade->setEasingCurve(QEasingCurve::InOutQuad);
    logoFade->start(QAbstractAnimation::DeleteWhenStopped);


        QPropertyAnimation* nameFade = new QPropertyAnimation(nameOpacity, "opacity", this);
        nameFade->setDuration(800);
        nameFade->setStartValue(0.0);
        nameFade->setEndValue(1.0);
        nameFade->setEasingCurve(QEasingCurve::OutCubic);
        nameFade->start(QAbstractAnimation::DeleteWhenStopped);


        QPoint startPos = nameLabel->pos() + QPoint(0, 40);
        QPoint endPos   = nameLabel->pos();
        QPropertyAnimation* nameSlide = new QPropertyAnimation(nameLabel, "pos", this);
        nameSlide->setDuration(800);
        nameSlide->setStartValue(startPos);
        nameSlide->setEndValue(endPos);
        nameSlide->setEasingCurve(QEasingCurve::OutCubic);
        nameSlide->start(QAbstractAnimation::DeleteWhenStopped);
    });

    QTimer::singleShot(2500, this, [this]() {
        LoginDialog* login = new LoginDialog();
        login->show();
        this->hide();
        this->deleteLater();
    });
}