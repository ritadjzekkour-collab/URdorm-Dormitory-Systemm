cmake_minimum_required(VERSION 3.16)
project(URdorm VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

qt_standard_project_setup()

qt_add_executable(URdorm
    main.cpp
    DataManager.cpp
    ThemeManager.cpp
    SplashScreen.cpp
    LoginDialog.cpp
    MainWindow.cpp
    StudentCardPage.cpp
    ServicesPage.cpp
    NotificationsPage.cpp
    WaterStatusPage.cpp
    InfoPage.cpp
    DataManager.h InfoPage.h LoginDialog.h MainWindow.h NotificationsPage.h ServicesPage.h SplashScreen.h Student.h StudentCardPage.h ThemeManager.h WaterStatusPage.h
    assets/images/students/student1.jpg assets/images/students/student2.jpg assets/images/alerts.jpg assets/images/card.jpg assets/images/change.jpg assets/images/chief.jpg assets/images/contact.jpg assets/images/denji.gif assets/images/info.jpg assets/images/logo.png assets/images/photo.jpg assets/images/qr_placeholder.png assets/images/report.jpg assets/images/reze.gif assets/images/services.jpg assets/images/swap.jpg assets/images/water.jpg assets/sounds/notif_denji.wav assets/sounds/notif_reze.wav
    assets/images/students/student1.jpg assets/images/students/student2.jpg assets/images/alerts.jpg assets/images/card.jpg assets/images/change.jpg assets/images/chief.jpg assets/images/contact.jpg assets/images/denji.gif assets/images/info.jpg assets/images/logo.png assets/images/notif_announce.jpg assets/images/notif_hosting.jpg assets/images/notif_water.jpg assets/images/photo.jpg assets/images/qr_placeholder.png assets/images/report.jpg assets/images/reze.gif assets/images/services.jpg assets/images/swap.jpg assets/images/water.jpg assets/sounds/notif_denji.wav assets/sounds/notif_reze.wav
    assets/images/students/student1.jpg assets/images/students/student2.jpg assets/images/alerts.jpg assets/images/card.jpg assets/images/change.jpg assets/images/chief.jpg assets/images/contact.jpg assets/images/denji.gif assets/images/info.jpg assets/images/logo.png assets/images/notif_announce.jpg assets/images/notif_hosting.jpg assets/images/notif_water.jpg assets/images/photo.jpg assets/images/qr_placeholder.png assets/images/report.jpg assets/images/reze.gif assets/images/services.jpg assets/images/swap.jpg assets/images/water.jpg assets/sounds/notif_denji.wav assets/sounds/notif_reze.wav
    assets/images/students/student1.jpg assets/images/students/student2.jpg assets/images/alerts.jpg assets/images/card.jpg assets/images/change.jpg assets/images/chief.jpg assets/images/contact.jpg assets/images/denji.gif assets/images/info.jpg assets/images/logo.png assets/images/notif_announce.jpg assets/images/notif_hosting.jpg assets/images/notif_water.jpg assets/images/photo.jpg assets/images/qr_placeholder.png assets/images/report.jpg assets/images/reze.gif assets/images/services.jpg assets/images/swap.jpg assets/images/themes.jpg assets/images/water.jpg assets/sounds/notif_denji.wav assets/sounds/notif_reze.wav
    assets/images/students/student1.jpg assets/images/students/student2.jpg assets/images/alerts.jpg assets/images/card.jpg assets/images/change.jpg assets/images/chief.jpg assets/images/contact.jpg assets/images/denji.gif assets/images/info.jpg assets/images/logo.png assets/images/notif_announce.jpg assets/images/notif_hosting.jpg assets/images/notif_water.jpg assets/images/photo.jpg assets/images/qr_placeholder.png assets/images/report.jpg assets/images/reze.gif assets/images/services.jpg assets/images/swap.jpg assets/images/themes.jpg assets/images/water.jpg assets/sounds/notif_denji.wav assets/sounds/notif_reze.wav
    assets/images/students/student1.jpg assets/images/students/student2.jpg assets/images/alerts.jpg assets/images/card.jpg assets/images/change.jpg assets/images/chief.jpg assets/images/contact.jpg "assets/images/denji (1).gif" "assets/images/denji (2).gif" assets/images/info.jpg assets/images/logo.png assets/images/notif_announce.jpg assets/images/notif_hosting.jpg assets/images/notif_water.jpg assets/images/photo.jpg assets/images/qr_placeholder.png assets/images/report.jpg assets/images/services.jpg assets/images/swap.jpg assets/images/themes.jpg assets/images/water.jpg assets/sounds/notif_denji.wav assets/sounds/notif_reze.wav
)

qt_add_resources(URdorm "assets"
    PREFIX "/"
    FILES
        assets/images/denji.gif
        assets/images/reze.gif
        assets/images/logo.png
        assets/images/qr_placeholder.png
        assets/images/students/student1.jpg
        assets/images/students/student2.jpg
        assets/images/card.jpg
        assets/images/services.jpg
       assets/images/alerts.jpg
         assets/images/water.jpg
         assets/images/swap.jpg
         assets/images/change.jpg
         assets/images/photo.jpg
         assets/images/report.jpg
         assets/images/chief.jpg
         assets/images/contact.jpg
        assets/images/info.jpg
        assets/sounds/notif_denji.wav
        assets/sounds/notif_reze.wav
        assets/images/notif_hosting.jpg
        assets/images/notif_water.jpg
        assets/images/notif_announce.jpg
        assets/images/themes.jpg
)

target_link_libraries(URdorm PRIVATE Qt6::Widgets Qt6::Multimedia)