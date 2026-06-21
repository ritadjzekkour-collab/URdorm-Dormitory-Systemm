#pragma once
#include <QObject>
#include <QColor>
#include <QString>

class ThemeManager : public QObject {
    Q_OBJECT
public:
    static ThemeManager& instance();

    void setTheme(int index); // 0=default, 1=denji, 2=rez
    int currentTheme() const { return m_currentTheme; }

    QString getStyleSheet() const;
    QColor primary() const;
    QColor secondary() const;
    QColor bgColor() const;
    QColor cardColor() const;
    QColor textColor() const;
    QString gifPath() const;
    QString soundPath() const;
    float gifOpacity() const;
signals:
    void themeChanged(int index);

private:
    ThemeManager();
    int m_currentTheme = 0;
};