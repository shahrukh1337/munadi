#include "updater.h"
#include "munadiengine.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QUrl>
#include <QSettings>

#include <QtWidgets/QMessageBox>

Updater::Updater(QObject *parent) :
    QObject(parent)
{
    QUrlQuery jadeedParams;
#if defined Q_OS_WIN
    jadeedParams.addQueryItem("os","win");
#elif defined Q_OS_OSX
    jadeedParams.addQueryItem("os","osx");
#elif defined Q_OS_LINUX
    jadeedParams.addQueryItem("os","linux");
#endif

#if defined ARABIC
    jadeedParams.addQueryItem("lang","ar");
#else
    jadeedParams.addQueryItem("lang","en");
#endif

    QUrl jadeed(QSettings().value("General/updateURL", "http://munadi.org/jadeed.php").toString());
    jadeed.setQuery(jadeedParams);

    QNetworkRequest req;
    req.setUrl(jadeed);

    QNetworkAccessManager *am = new QNetworkAccessManager(this);
    connect(am, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    am->get(req);
}

void Updater::replyFinished(QNetworkReply *reply)
{
    QStringList replyStrings = QString(reply->readAll()).split(',');

    if(replyStrings.size() == 2)
    {
        QString version = replyStrings[0];
        QString details = replyStrings[1];

        if(version > APP_VERSION)
        {
            QMessageBox::information(0, tr("Munadi Update Available"), details);
        }
    }
    reply->deleteLater();
}
