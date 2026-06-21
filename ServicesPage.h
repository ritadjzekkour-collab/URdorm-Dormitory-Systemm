#pragma once
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
class ServiceCard : public QWidget {
    Q_OBJECT
public:
    ServiceCard(const QString& iconPath,
                const QString& name,
                int index,
                QWidget* parent = nullptr);

signals:
    void clicked(int index);

protected:

    void mousePressEvent(QMouseEvent* e) override;

private:
    int m_index;

};

class ToastWidget : public QLabel {
    Q_OBJECT
public:
    explicit ToastWidget(QWidget* parent = nullptr);
    void showToast(const QString& message);
};

class ServicesPage : public QWidget {
    Q_OBJECT
public:
    explicit ServicesPage(QWidget* parent = nullptr);

signals:
    void notificationAdded();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void openDrawer(int serviceIndex);
    void closeDrawer();
    void handleSubmit();

private:
    void setupGrid();
    void setupDrawer();
    QWidget* makeRoomSwapForm();
    QWidget* makeChangeRequestForm();
    QWidget* makeRoomPhotoForm();
    QWidget* makeReportProblemForm();
    QWidget* makePavilionChiefInfo();
    QWidget* makeAdminContactInfo();

    QWidget*        drawer;
    QLabel*         drawerTitle;
    QStackedWidget* drawerStack;
    QPushButton*    submitBtn;
    ToastWidget*    toast;
    bool            drawerOpen = false;
    int             currentService = 0;

    // Form widgets we need to reference
    QLineEdit* swapTargetIdField = nullptr;
    QTextEdit* changeReasonEdit = nullptr;
    QLineEdit* changeRoomField = nullptr;
    QLabel*    photoPreviewLabel = nullptr;
    QComboBox* problemCategoryBox = nullptr;
    QTextEdit* problemDescEdit = nullptr;
};