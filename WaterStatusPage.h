#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPropertyAnimation>

class WaterCircle : public QWidget {
    Q_OBJECT
    Q_PROPERTY(float pulseScale READ pulseScale WRITE setPulseScale)

public:
    explicit WaterCircle(QWidget* parent = nullptr);
    void updateState(bool available);

    float pulseScale() const { return m_pulseScale; }
    void setPulseScale(float v) { m_pulseScale = v; update(); }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    bool m_available = true;
    float m_pulseScale = 1.0f;
    QPropertyAnimation* m_pulseAnim = nullptr;

    void startPulse();
};

class WaterStatusPage : public QWidget {
    Q_OBJECT
public:
    explicit WaterStatusPage(QWidget* parent = nullptr);
    void refreshHistory();

private:
    WaterCircle* waterCircle;
    QLabel*      lastUpdatedLabel;
    QVBoxLayout* historyLayout;
    QPushButton* toggleBtn;

    void buildHistory();
};