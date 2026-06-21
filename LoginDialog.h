#pragma once
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMovie>
class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void tryLogin();
    void onLoginClicked();

private:
    QLabel*     bgLabel;
    QLabel*     logoLabel;
    QLabel*     titleLabel;
    QLineEdit*  idField;
    QLineEdit*  passField;
    QComboBox*  roleBox;
    QPushButton* loginBtn;
    QLabel*     errorLabel;
    QMovie*     bgMovie;

    void setupUi();
    void applyTheme();
    void animateBounce();
};