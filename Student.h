#pragma once
#include <QString>

struct Student {
    QString id;
    QString password;
    QString name;
    QString major;
    QString room;
    QString pavilion;
    QString photoPath;
    QString role; // "student" "administration" "inspection" "chief"
    bool roomSwapRequested = false;
    QString swapTargetId = "";
    QString roomPhotoPath = "";
    bool roomPhotoValidated = false;
};