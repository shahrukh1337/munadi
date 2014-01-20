#include <QTranslator>
#include "qtquick2applicationviewer.h"

#include <QQmlContext>
#include "munadiengine.h"

#ifdef DESKTOP
#include <QApplication>
#else
#include <QGuiApplication>
#endif

int main(int argc, char *argv[])
{
#ifdef DESKTOP
    QApplication app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif

#ifdef ARABIC
    QTranslator t;
    t.load(QLocale::Arabic, ":/data/ar.qm");
    app.installTranslator(&t);
#endif

    app.setOrganizationName("Munadi Project");
    app.setOrganizationDomain("munadi.org");
    app.setApplicationName("Muandi");

    QtQuick2ApplicationViewer window;
    MunadiEngine engine(&window);

    window.rootContext()->setContextProperty("engine", &engine);
    window.rootContext()->setContextProperty("settingsCache", engine.settingsCache);
    window.setSource(QUrl("qrc:/qml/main.qml"));
    window.setIcon(QIcon(":/img/munadi.png"));

    window.installEventFilter(&engine);

    if(argc >= 2 && strcmp(argv[1], "-startup") == 0)
        return app.exec();
    else
    {
        window.showExpanded();
        return app.exec();
    }
}
