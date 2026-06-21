#pragma once
#include <QWidget>
#include <QTabWidget>

class InfoPage : public QWidget {
    Q_OBJECT
public:
    explicit InfoPage(QWidget* parent = nullptr);

private:
    QWidget* makePavilionTab();
    QWidget* makeRulesTab();
};