#include <QApplication>
#include "SplashScreen.h"
#include "ThemeManager.h"
#include "DataManager.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("URdorm");
    app.setApplicationDisplayName("URdorm — University Residence Management");
    app.setApplicationVersion("1.0");

    // Initialize singletons
    DataManager::instance();
    ThemeManager::instance().setTheme(0); // Default theme

    // Apply global stylesheet
    app.setStyleSheet(ThemeManager::instance().getStyleSheet());

    // Start with splash screen → login → main window
    SplashScreen* splash = new SplashScreen();
    splash->show();

    return app.exec();
}