#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"

#include <QQmlContext>
#include "munadiengine.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Munadi Project");
    app.setOrganizationDomain("munadi.org");
    app.setApplicationName("Muandi");

    QtQuick2ApplicationViewer window;
    MunadiEngine engine(&window);

    window.rootContext()->setContextProperty("engine", &engine);
    window.setSource(QUrl("qrc:/qml/main.qml"));
    window.installEventFilter(&engine);
    window.showExpanded();

    return app.exec();
}
