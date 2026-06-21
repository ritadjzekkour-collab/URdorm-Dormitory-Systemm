#pragma once
#include <vector>
#include <QString>
#include "Student.h"

class DataManager {
public:
    static DataManager& instance();

    std::vector<Student> students;
    bool waterAvailable = true;
    QString waterLastUpdated = "Today 08:00";
    std::vector<QString> waterHistory;

    struct Notification {
        QString title;
        QString description;
        QString timestamp;
        QString type; // "hosting", "water", "announcement"
        bool isRead = false;
    };
    std::vector<Notification> notifications;

    Student* findStudent(const QString& id);
    Student* currentUser = nullptr;

private:
    DataManager();
};