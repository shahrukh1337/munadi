#include <QApplication>
#include "qtquick2applicationviewer.h"

#include <QQmlContext>
#include "munadiengine.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Munadi Project");
    app.setOrganizationDomain("munadi.org");
    app.setApplicationName("Muandi");

    QtQuick2ApplicationViewer window;
    MunadiEngine engine(&window);

    window.rootContext()->setContextProperty("engine", &engine);
    window.rootContext()->setContextProperty("settingsCache", engine.settingsCache);
    window.setSource(QUrl("qrc:/qml/main.qml"));
    window.installEventFilter(&engine);

    if(argc >= 2 && strcmp(argv[1], "-startup") == 0)
        return app.exec();
    else
    {
        window.showExpanded();
        return app.exec();
    }
}
