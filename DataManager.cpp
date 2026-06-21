#include "DataManager.h"

DataManager& DataManager::instance() {
    static DataManager inst;
    return inst;
}

DataManager::DataManager() {
    students = {
        {"20230101", "pass123", "Amina Bouali",   "Computer Science", "204", "Pavilion A", ":/assets/images/students/student1.jpg", "student"},
        {"20230102", "pass123", "Sara Meziane",   "Mathematics",      "205", "Pavilion A", ":/assets/images/students/student2.jpg", "student"},
        {"20230103", "pass123", "Lina Khelil",    "Physics",          "301", "Pavilion B", ":/assets/images/students/student1.jpg", "student"},
        {"20230104", "pass123", "Nour Benali",    "Biology",          "110", "Pavilion A", ":/assets/images/students/student2.jpg", "student"},
        {"20230105", "pass123", "Dina Hamidi",    "Architecture",     "412", "Pavilion C", ":/assets/images/students/student1.jpg", "student"},
        {"admin01",  "admin123","Admin User",     "",                 "",    "",            "", "administration"},
        {"chief01",  "chief123","Fatima Oukaci",  "Pavilion Chief",   "101", "Pavilion A", "", "chief"},
        {"inspect01","insp123", "Inspection",     "",                 "",    "",            "", "inspection"}
    };

    notifications = {
        {"Room Hosting Notice",  "Your room will be used for hosting on Friday.", "Today 09:00",      "hosting",      false},
        {"Water Unavailable",    "Water supply is cut until 6PM.",                "Yesterday 14:00",  "water",        false},
        {"Water Restored",       "Water supply is back to normal.",               "Yesterday 18:30",  "water",        true},
        {"General Announcement", "Pavilion meeting this Sunday at 10AM.",         "2 days ago 11:00", "announcement", true}
    };

    waterHistory = {
        "Today 08:00 — Available",
        "Yesterday 18:30 — Restored",
        "Yesterday 14:00 — Unavailable",
        "3 days ago 07:00 — Available",
        "Last week — Unavailable"
    };
}

Student* DataManager::findStudent(const QString& id) {
    for (auto& s : students) {
        if (s.id == id) return &s;
    }
    return nullptr;
}