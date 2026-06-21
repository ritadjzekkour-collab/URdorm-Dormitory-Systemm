#include "LoginDialog.h"
#include "MainWindow.h"
#include "DataManager.h"
#include "ThemeManager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QLinearGradient>
#include <QTimer>
#include <QFrame>

// ── GradientPanel: left panel with bisexual flag gradient ──────────
class GradientPanel : public QWidget {
public:
    explicit GradientPanel(QWidget* p = nullptr) : QWidget(p) {}
protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QLinearGradient grad(0, 0, width(), height());
        grad.setColorAt(0.0, QColor("#D60270")); // hot pink
        grad.setColorAt(0.5, QColor("#9B4F96")); // purple
        grad.setColorAt(1.0, QColor("#0038A8")); // bold blue
        painter.fillRect(rect(), grad);

        // Decorative circles
        painter.setOpacity(0.15);
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPoint(-30, height()/3), 160, 160);
        painter.drawEllipse(QPoint(width()+30, 2*height()/3), 140, 140);
        painter.setOpacity(0.08);
        painter.drawEllipse(QPoint(width()/2, height()+20), 220, 220);
    }
};

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent, Qt::Window), bgMovie(nullptr)
{
    setFixedSize(900, 600);
    setWindowTitle("URdorm");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setupUi();
    applyTheme();
}

void LoginDialog::setupUi() {
    auto* root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Left panel ──────────────────────────────────────────────
    auto* leftPanel = new QWidget(this);
    leftPanel->setFixedWidth(450);
    leftPanel->setMinimumHeight(600);

    // Gradient background
    auto* gradPanel = new GradientPanel(leftPanel);
    gradPanel->setGeometry(0, 0, 450, 600);

    // GIF label on top of gradient (for anime themes)
    bgLabel = new QLabel(leftPanel);
    bgLabel->setGeometry(0, 0, 450, 600);
    bgLabel->setScaledContents(true);
    bgLabel->hide();

    // Branding on left
    auto* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setAlignment(Qt::AlignCenter);
    leftLayout->setSpacing(16);
    leftLayout->setContentsMargins(40, 40, 40, 40);

    auto* brandLogo = new QLabel(leftPanel);
    QPixmap lp(":/assets/images/logo.png");
    if (!lp.isNull())
        brandLogo->setPixmap(
            lp.scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        brandLogo->setText("🏠");
    brandLogo->setAlignment(Qt::AlignCenter);
    brandLogo->setStyleSheet("font-size: 60px; background: transparent;");

    auto* brandName = new QLabel("URdorm", leftPanel);
    QFont bf;
    bf.setFamily("Arial Rounded MT Bold");
    bf.setPointSize(36);
    bf.setBold(true);
    brandName->setFont(bf);
    brandName->setStyleSheet(
        "color: white;"
        "letter-spacing: 3px;"
        "background: transparent;"
        );
    brandName->setAlignment(Qt::AlignCenter);

    auto* brandSub = new QLabel("University Residence\nManagement System", leftPanel);
    brandSub->setStyleSheet(
        "color: rgba(255,255,255,0.80);"
        "font-size: 15px;"
        "background: transparent;"
        "line-height: 1.6;"
        );
    brandSub->setAlignment(Qt::AlignCenter);

    // Developer credit
    auto* devLabel = new QLabel("by Zekkour Ritadj", leftPanel);
    devLabel->setStyleSheet(
        "color: rgba(255,255,255,0.50);"
        "font-size: 12px;"
        "background: transparent;"
        "font-style: italic;"
        );
    devLabel->setAlignment(Qt::AlignCenter);

    leftLayout->addStretch();
    leftLayout->addWidget(brandLogo,  0, Qt::AlignCenter);
    leftLayout->addWidget(brandName,  0, Qt::AlignCenter);
    leftLayout->addWidget(brandSub,   0, Qt::AlignCenter);
    leftLayout->addStretch();
    leftLayout->addWidget(devLabel,   0, Qt::AlignCenter);

    root->addWidget(leftPanel);

    // ── Right panel ─────────────────────────────────────────────
    auto* rightPanel = new QWidget(this);
    rightPanel->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #F8E6F0, stop:0.5 #EDD6F0, stop:1 #D6E4F8);"
        );

    auto* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setAlignment(Qt::AlignCenter);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // Card
    auto* card = new QWidget(rightPanel);
    card->setFixedWidth(380);
    card->setStyleSheet(
        "background: rgba(255,255,255,0.85);"
        "border-radius: 20px;"
        "border: 1px solid rgba(255,255,255,0.9);"
        );

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(14);
    cardLayout->setContentsMargins(32, 32, 32, 32);

    // Logo
    logoLabel = new QLabel(card);
    QPixmap lp2(":/assets/images/logo.png");
    if (!lp2.isNull())
        logoLabel->setPixmap(
            lp2.scaled(65, 65, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        logoLabel->setText("🏠");
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setStyleSheet("background: transparent; font-size: 40px;");

    // Title
    titleLabel = new QLabel("Welcome to URdorm", card);
    QFont tf;
    tf.setFamily("Arial Rounded MT Bold");
    tf.setPointSize(20);
    tf.setBold(true);
    titleLabel->setFont(tf);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        QString("color: %1; background: transparent;")
            .arg(ThemeManager::instance().primary().name())
        );

    // Subtitle
    auto* subLabel = new QLabel("Sign in to your account", card);
    subLabel->setStyleSheet(
        "color: #7B7B7B;"
        "font-size: 14px;"
        "background: transparent;"
        );
    subLabel->setAlignment(Qt::AlignCenter);

    // Divider
    auto* divider = new QFrame(card);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet(
        "background: rgba(128,128,128,0.25);"
        "border: none;"
        "max-height: 1px;"
        "margin: 4px 0;"
        );

    // Fields
    idField = new QLineEdit(card);
    idField->setPlaceholderText("Registration Number (e.g. 20251234)");
    idField->setFixedHeight(50);
    idField->setStyleSheet(
        "QLineEdit {"
        "  background: white;"
        "  color: #1A1A2E;"
        "  border: 2px solid rgba(150,100,200,0.3);"
        "  border-radius: 12px;"
        "  padding: 10px 16px;"
        "  font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid #D60270;"
        "}"
        );

    passField = new QLineEdit(card);
    passField->setPlaceholderText("🔒  Password");
    passField->setEchoMode(QLineEdit::Password);
    passField->setFixedHeight(50);
    passField->setStyleSheet(idField->styleSheet());

    roleBox = new QComboBox(card);
    roleBox->addItems({"Student", "Administration", "Inspection Team", "Pavilion Chief"});
    roleBox->setFixedHeight(50);
    roleBox->setStyleSheet(
        "QComboBox {"
        "  background: white;"
        "  color: #1A1A2E;"
        "  border: 2px solid rgba(150,100,200,0.3);"
        "  border-radius: 12px;"
        "  padding: 10px 16px;"
        "  font-size: 14px;"
        "}"
        "QComboBox:focus { border: 2px solid #D60270; }"
        "QComboBox::drop-down { border: none; width: 28px; }"
        "QComboBox QAbstractItemView {"
        "  background: white; color: #1A1A2E;"
        "  border-radius: 8px;"
        "  selection-background-color: #D60270;"
        "  selection-color: white;"
        "}"
        );

    // Login button
    loginBtn = new QPushButton("  Login", card);
    loginBtn->setFixedHeight(52);
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setStyleSheet(QString(
        "QPushButton {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "    stop:0 #D60270, stop:0.5 #9B4F96, stop:1 #0038A8);"
        "  color: white;"
        "  border-radius: 12px;"
        "  font-size: 16px;"
        "  font-weight: 700;"
        "  letter-spacing: 1px;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "    stop:0 #B0005A, stop:0.5 #7A3A7A, stop:1 #002880);"
        "}"
        ));

    connect(loginBtn, &QPushButton::clicked,
            this, &LoginDialog::onLoginClicked);

    // Error label
    errorLabel = new QLabel(card);
    errorLabel->setStyleSheet(
        "color: #D32F2F;"
        "font-size: 13px;"
        "font-weight: 600;"
        "background: transparent;"
        );
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setWordWrap(true);
    errorLabel->hide();

    cardLayout->addWidget(logoLabel,  0, Qt::AlignCenter);
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subLabel);
    cardLayout->addWidget(divider);
    cardLayout->addWidget(idField);
    cardLayout->addWidget(passField);
    cardLayout->addWidget(roleBox);
    cardLayout->addSpacing(4);
    cardLayout->addWidget(loginBtn);
    cardLayout->addWidget(errorLabel);

    rightLayout->addWidget(card, 0, Qt::AlignCenter);
    root->addWidget(rightPanel, 1);

    // Enter key triggers login
    connect(idField,  &QLineEdit::returnPressed,
            this, &LoginDialog::onLoginClicked);
    connect(passField, &QLineEdit::returnPressed,
            this, &LoginDialog::onLoginClicked);
}

void LoginDialog::applyTheme() {
    int    theme = ThemeManager::instance().currentTheme();
    QString gif  = ThemeManager::instance().gifPath();

    if (theme != 0 && !gif.isEmpty()) {
        if (!bgMovie) {
            bgMovie = new QMovie(gif, QByteArray(), this);
        } else {
            bgMovie->stop();
            bgMovie->setFileName(gif);
        }
        bgLabel->setMovie(bgMovie);
        auto* eff = new QGraphicsOpacityEffect(bgLabel);
        eff->setOpacity(0.25); // 25% — readable
        bgLabel->setGraphicsEffect(eff);
        bgMovie->start();
        bgLabel->show();
    } else {
        if (bgMovie) bgMovie->stop();
        bgLabel->hide();
    }
}

void LoginDialog::resizeEvent(QResizeEvent* event) {
    QDialog::resizeEvent(event);
    bgLabel->setGeometry(0, 0, 450, height());
}
void LoginDialog::animateBounce() {
    QRect orig = loginBtn->geometry();
    int dw = orig.width()  / 20;
    int dh = orig.height() / 20;
    QRect shrunk(orig.x() + dw, orig.y() + dh,
                 orig.width() - 2*dw, orig.height() - 2*dh);

    auto* anim = new QPropertyAnimation(loginBtn, "geometry", this);
    anim->setDuration(150);
    anim->setKeyValueAt(0.0, orig);
    anim->setKeyValueAt(0.4, shrunk);
    anim->setKeyValueAt(0.7, QRect(orig.x()-dw/2, orig.y()-dh/2,
                                   orig.width()+dw, orig.height()+dh));
    anim->setKeyValueAt(1.0, orig);
    anim->setEasingCurve(QEasingCurve::OutBounce);

    // ← FIXED: use lambda + QueuedConnection to prevent crash
    connect(anim, &QPropertyAnimation::finished,
            this, [this]() { tryLogin(); },
            Qt::QueuedConnection);

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}


void LoginDialog::onLoginClicked() {
    errorLabel->hide();
    loginBtn->setEnabled(false); // prevent double click crash
    animateBounce();
}

void LoginDialog::tryLogin() {
    loginBtn->setEnabled(true);

    QString selectedRole = roleBox->currentText();

    QString roleKey;
    if      (selectedRole == "Student")          roleKey = "student";
    else if (selectedRole == "Administration")   roleKey = "administration";
    else if (selectedRole == "Inspection Team")  roleKey = "inspection";
    else if (selectedRole == "Pavilion Chief")   roleKey = "chief";

    auto& dm = DataManager::instance();
    for (auto& s : dm.students) {
        if (s.role == roleKey) {
            dm.currentUser = &s;
            MainWindow* w = new MainWindow();
            w->show();

            this->deleteLater();
            return;
        }
    }
}