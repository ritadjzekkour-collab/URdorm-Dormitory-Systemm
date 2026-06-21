#include "InfoPage.h"
#include "DataManager.h"
#include "ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QTabWidget>
#include <QGraphicsDropShadowEffect>

InfoPage::InfoPage(QWidget* parent) : QWidget(parent) {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 16, 20, 16);
    mainLayout->setSpacing(12);

    // ── Title — centered at top ───────────────────────────────────
    auto* title = new QLabel("INFORMATION", this);
    QFont tf;
    tf.setFamily("Georgia");
    tf.setPixelSize(38);
    tf.setBold(true);
    tf.setLetterSpacing(QFont::AbsoluteSpacing, 6);
    title->setFont(tf);
    title->setAlignment(Qt::AlignCenter);   // ← centered
    title->setObjectName("pageTitle");
    title->setStyleSheet("color: #FFFFFF; background: transparent;");
    mainLayout->addWidget(title);

    auto* tabs = new QTabWidget(this);
    tabs->setDocumentMode(true);
    QFont tabF; tabF.setPixelSize(20);
    tabs->setFont(tabF);
    tabs->addTab(makePavilionTab(), "Pavilion Info");
    tabs->addTab(makeRulesTab(),    "Rules");
    mainLayout->addWidget(tabs, 1);
}

QWidget* InfoPage::makePavilionTab() {
    auto* page = new QWidget();
    auto* scroll = new QScrollArea(page);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    auto* pageLayout = new QVBoxLayout(page);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scroll);

    auto* content = new QWidget(scroll);
    auto* layout = new QVBoxLayout(content);
    layout->setContentsMargins(4, 12, 4, 12);
    layout->setSpacing(16);
    scroll->setWidget(content);

    // ── Chief card ──────────────────────────────────────────────
    auto* chiefCard = new QWidget(content);
    chiefCard->setStyleSheet(QString(
        "background: white; border-radius: 14px; border: 1px solid rgba(0,0,0,0.06);"
        ));
    auto* shadow = new QGraphicsDropShadowEffect(chiefCard);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 30));
    chiefCard->setGraphicsEffect(shadow);

    auto* chiefLayout = new QHBoxLayout(chiefCard);
    chiefLayout->setContentsMargins(16, 16, 16, 16);
    chiefLayout->setSpacing(16);

    auto* avatarLabel = new QLabel(chiefCard);
    avatarLabel->setFixedSize(64, 64);
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setStyleSheet(QString(
                                   "background: %1; border-radius: 32px; color: white; font-size: 26px;"
                                   ).arg(ThemeManager::instance().primary().name()));
    avatarLabel->setText("👤");

    auto* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(4);

    Student* chief = nullptr;
    for (auto& s : DataManager::instance().students) {
        if (s.role == "chief") { chief = &s; break; }
    }

    QString chiefName = chief ? chief->name : "Fatima Oukaci";
    QString chiefRoom = chief ? chief->room : "101";

    auto* nameLbl = new QLabel(chiefName, chiefCard);
    QFont nf; nf.setPixelSize(22); nf.setBold(true); nameLbl->setFont(nf);
    nameLbl->setStyleSheet(QString("color: %1; background: transparent;")
                               .arg(ThemeManager::instance().primary().name()));

    auto* roleLbl = new QLabel("Pavilion Chief — Pavilion A", chiefCard);
    QFont roleF; roleF.setPixelSize(18);
    roleLbl->setFont(roleF);
    roleLbl->setStyleSheet("color: #9E9E9E; background: transparent;");

    auto* roomLbl  = new QLabel("Room " + chiefRoom, chiefCard);
    auto* phoneLbl = new QLabel("0550 123 456",       chiefCard);
    QFont rowF; rowF.setPixelSize(19);
    roomLbl->setFont(rowF);
    phoneLbl->setFont(rowF);
    roomLbl->setStyleSheet("background: transparent;");
    phoneLbl->setStyleSheet("background: transparent;");

    infoLayout->addWidget(nameLbl);
    infoLayout->addWidget(roleLbl);
    infoLayout->addWidget(roomLbl);
    infoLayout->addWidget(phoneLbl);

    chiefLayout->addWidget(avatarLabel, 0, Qt::AlignTop);
    chiefLayout->addLayout(infoLayout, 1);
    layout->addWidget(chiefCard);

    auto* div = new QFrame(content);
    div->setFrameShape(QFrame::HLine);
    div->setStyleSheet("background: rgba(0,0,0,0.1); border: none; max-height: 1px;");
    layout->addWidget(div);

    auto* adminTitle = new QLabel("Administration Contacts", content);
    QFont at; at.setPixelSize(22); at.setBold(true); adminTitle->setFont(at);
    layout->addWidget(adminTitle);

    struct Contact { QString line; };
    QList<Contact> contacts = {
        {"Ext. 101 — Main Office"},
        {"Ext. 102 — Maintenance"},
        {"Ext. 103 — Security"},
        {"Ext. 104 — Medical Room"}
    };

    for (auto& c : contacts) {
        auto* row = new QWidget(content);
        row->setStyleSheet("background: rgba(0,0,0,0.03); border-radius: 8px;");
        auto* rl = new QHBoxLayout(row);
        rl->setContentsMargins(14, 10, 14, 10);

        auto* lbl = new QLabel(c.line, row);
        QFont lf; lf.setPixelSize(20);
        lbl->setFont(lf);
        lbl->setStyleSheet("background: transparent;");
        rl->addWidget(lbl);
        layout->addWidget(row);
    }

    auto* usefulTitle = new QLabel("Useful Information", content);
    QFont ut; ut.setPixelSize(22); ut.setBold(true); usefulTitle->setFont(ut);
    layout->addWidget(usefulTitle);

    QList<QPair<QString,QString>> infos = {
        {"🕐", "Office hours: Sunday–Thursday, 8AM–4PM"},
        {"🚿", "Laundry room hours: 7AM–8PM daily"},
        {"🏥", "Medical room: Ground floor, Building B"},
        {"🅿️", "Parking available: Gate 2 (permit required)"}
    };

    for (auto& info : infos) {
        auto* row = new QWidget(content);
        row->setStyleSheet("background: rgba(0,0,0,0.03); border-radius: 8px;");
        auto* rl = new QHBoxLayout(row);
        rl->setContentsMargins(14, 10, 14, 10);
        rl->setSpacing(10);

        auto* iconLbl = new QLabel(info.first, row);
        iconLbl->setFixedWidth(28);
        iconLbl->setAlignment(Qt::AlignCenter);
        QFont iconF; iconF.setPixelSize(22);
        iconLbl->setFont(iconF);
        iconLbl->setStyleSheet("background: transparent;");

        auto* textLbl = new QLabel(info.second, row);
        QFont textF; textF.setPixelSize(19);
        textLbl->setFont(textF);
        textLbl->setStyleSheet("background: transparent;");

        rl->addWidget(iconLbl);
        rl->addWidget(textLbl, 1);
        layout->addWidget(row);
    }

    layout->addStretch();
    return page;
}

QWidget* InfoPage::makeRulesTab() {
    auto* page = new QWidget();
    auto* pageLayout = new QVBoxLayout(page);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    auto* scroll = new QScrollArea(page);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    pageLayout->addWidget(scroll);

    auto* content = new QWidget(scroll);
    auto* layout  = new QVBoxLayout(content);
    layout->setContentsMargins(4, 12, 4, 24);
    scroll->setWidget(content);

    auto* rulesLabel = new QLabel(content);
    rulesLabel->setWordWrap(true);
    rulesLabel->setTextFormat(Qt::RichText);
    rulesLabel->setOpenExternalLinks(false);

    QString pri = ThemeManager::instance().primary().name();
    rulesLabel->setText(QString(R"(
<style>
  h3 { color: %1; margin-top: 16px; margin-bottom: 6px; font-size: 17pt; }
  ol { margin-left: 20px; }
  li { margin-bottom: 8px; font-size: 14pt; line-height: 1.5; }
</style>
<h3>General Rules</h3>
<ol>
<li>Curfew is at <b>10:00 PM</b> on weekdays and <b>11:00 PM</b> on weekends.</li>
<li>Visitors of the opposite gender are <b>not permitted</b> inside the rooms.</li>
<li>Noise must be kept to a minimum after <b>9:00 PM</b>.</li>
<li>Students are responsible for the <b>cleanliness</b> of their rooms.</li>
</ol>

<h3>Room Inspection</h3>
<ol start="5">
<li>Rooms will be inspected at the end of each <b>academic year</b>.</li>
<li>Any damage found will be <b>reported to administration</b>.</li>
<li>Students must submit a <b>room photo</b> before the end of year deadline via the Services page.</li>
</ol>

<h3>Facilities</h3>
<ol start="8">
<li>Laundry rooms are available from <b>7:00 AM to 8:00 PM</b> only.</li>
<li>Common areas must be left <b>clean</b> after use.</li>
<li>Report any maintenance issues through the app <b>Services</b> page.</li>
</ol>

<h3>Conduct</h3>
<ol start="11">
<li>Smoking is strictly <b>prohibited</b> inside all buildings.</li>
<li>Cooking in rooms is <b>not permitted</b>. Use the shared kitchen on each floor.</li>
<li>Respect the <b>privacy</b> and personal space of fellow students at all times.</li>
</ol>

<br><p style="color: #9E9E9E; font-size: 12pt; font-style: italic;">
Last updated: September 2024 — Administration of URdorm
</p>
    )").arg(pri));

    layout->addWidget(rulesLabel);
    layout->addStretch();
    return page;
}