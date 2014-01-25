#include "munadiengine.h"
#include "settingscache.h"

#include <QTime>
#include <QSettings>
#include <QQuickView>

#ifdef DESKTOP

#include "updater.h"
#include <QtWidgets/QMessageBox>
#include <QApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#endif

MunadiEngine::MunadiEngine(QQuickView *parent)
    : QObject(0)
    , parent(parent)
    , athanObject(0)
    , settingsCache(new SettingsCache())
    , timer(new QTimer(this))
#ifdef DESKTOP
    , updater(0)
#endif
{
    init();
}
MunadiEngine::~MunadiEngine()
{
    stopAthan();
#ifdef DESKTOP
    QSettings().setValue("Window/geometry", ((QQuickView *) parent)->geometry());
    QSettings().setValue("Window/state", ((QQuickView *) parent)->windowState());
    if(updater)         delete updater;
#endif
    if(athanObject)     delete athanObject;
    if(settingsCache)   delete settingsCache;
}
void MunadiEngine::calculatePrayer()
{
    Date date;  // Different from QDate
    date.day = QDate::currentDate().day();
    date.month = QDate::currentDate().month();
    date.year = QDate::currentDate().year();

    getPrayerTimes(&settingsCache->currentCity.location,
                   &settingsCache->currentCity.method,
                   &date,
                   settingsCache->currentCity.ptList);
}

void MunadiEngine::showMainWindow()
{
    parent->setWindowState( Qt::WindowState(parent->windowState() & ~Qt::WindowMinimized) );
    parent->show();
    parent->showNormal();
    parent->raise();  // for MacOS
}

void MunadiEngine::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch(status)
    {
    case QMediaPlayer::EndOfMedia:
        QTimer::singleShot(0, this, SIGNAL(athanStopped()) );    // Needed so events get through, for player to update state
        break;
    }
}
#ifdef DESKTOP
void MunadiEngine::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Trigger :
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::MiddleClick:

        if(parent->isVisible())
        {
            parent->hide();
        }
        else
        {
            showMainWindow();
        }
        break;
    }
}
void MunadiEngine::toggleView()
{
    if(parent->isVisible())
        parent->hide();
    else
        showMainWindow();
}
#endif
QTime MunadiEngine::getFajr()
{
    return QTime(settingsCache->currentCity.ptList[Fajr].hour,
                 settingsCache->currentCity.ptList[Fajr].minute).addSecs(settingsCache->fajrAdjustment * 60);
}
QTime MunadiEngine::getDuhr()
{
    return QTime(settingsCache->currentCity.ptList[Duhr].hour,
                 settingsCache->currentCity.ptList[Duhr].minute).addSecs(settingsCache->duhrAdjustment * 60);
}
QTime MunadiEngine::getAsr()
{
    return QTime(settingsCache->currentCity.ptList[Asr].hour,
                 settingsCache->currentCity.ptList[Asr].minute).addSecs(settingsCache->asrAdjustment * 60);
}
QTime MunadiEngine::getMagrib()
{
    return QTime(settingsCache->currentCity.ptList[Magrib].hour,
                 settingsCache->currentCity.ptList[Magrib].minute).addSecs(settingsCache->magribAdjustment * 60);
}
QTime MunadiEngine::getIsha()
{
    return QTime(settingsCache->currentCity.ptList[Isha].hour,
                 settingsCache->currentCity.ptList[Isha].minute).addSecs(settingsCache->ishaAdjustment * 60);
}
QTime MunadiEngine::getSunrise()
{
    return QTime(settingsCache->currentCity.ptList[Sunrise].hour,
                 settingsCache->currentCity.ptList[Sunrise].minute);
}

bool MunadiEngine::isAthanPlaying()
{
    dout << "isAthanPlaying :" << athanObject->state();

    if(athanObject == 0)    return false;

    if(athanObject->state() == QMediaPlayer::PlayingState)
        return true;

    return false;
}

void MunadiEngine::playAthan()
{
    if(isAthanPlaying())
    {
        dout << "Athan playing, will return.";
        return;
    }

    currentPrayerLabel = nextPrayerLabel;

    athanObject->setVolume(QSettings().value("Athan/volume", 80).toInt());
    athanObject->setMuted(QSettings().value("Athan/muted", false).toBool());
    athanObject->play();

    if(QSettings().value("Window/showOnAthan", true).toBool())
        showMainWindow();

    emit athanStarted();
}
void MunadiEngine::stopAthan()
{
    athanObject->stop();
    emit athanStopped();
}

QString MunadiEngine::getLocationName()
{
    return settingsCache->currentCity.name;
}

double MunadiEngine::getQibla()
{
    double q = getNorthQibla(&settingsCache->currentCity.location);
    return (q > -90 && q < 90) ? q : -q;
}

QString MunadiEngine::getTimeDifference()
{
    int currentMins = QTime::currentTime().hour() * 60;
    currentMins += QTime::currentTime().minute();

    QTime nextPrayerTime = getNextPrayer();

    int nextPrayerMins = nextPrayerTime.hour() * 60;
    nextPrayerMins += nextPrayerTime.minute();

    QTime time(0,0);
    time = time.addSecs(60 * (nextPrayerMins - currentMins));

    QString format;

#ifdef ARABIC
    if(time.hour() == 1)
        format += tr("'sa3a'");
    else if(time.hour() == 2)
        format += tr("'sa3atain'");
    else if(time.hour() < 11 && time.hour() > 0)
        format += tr("H 'sa3at'");
    else if(time.hour() > 0)
        format += tr("H 'sa3a'");

    if(time.hour() != 0 && time.minute() != 0)    format += tr(" wa ");

    if(time.minute() == 1)
        format += tr("'dakika'");
    else if(time.minute() == 2)
        format += tr("'dakikatan'");
    else if(time.minute() < 11 && time.minute() > 0)
        format += tr("m 'dakayek'");
    else if(time.minute() > 0)
        format += tr("m 'dakika'");
#else
    if(time.hour() == 1)
        format += ("'an hour'");
    else if(time.hour() > 1)
        format += ("H 'hours'");

    if(time.hour() != 0 && time.minute() != 0)    format += (" 'and' ");

    if(time.minute() == 1)
        format += ("'a minute'");
    else if(time.minute() > 1)
        format += ("m 'minutes'");
#endif

    return time.toString(format);
}

QTime MunadiEngine::getNextPrayer()
{
    QTime currentTime(QTime::currentTime().hour(),QTime::currentTime().minute(), QTime::currentTime().second());

    if(currentTime <= getFajr() && settingsCache->fajrEnabled)
    {
        //dout<< "HIT FAJR";

        nextPrayerLabel = tr("Fajr");
        return getFajr();
    }
    else if(currentTime <= getDuhr() && settingsCache->duhrEnabled)
    {
        //dout<< "HIT THUHR";

        nextPrayerLabel = tr("Duhr");
        return getDuhr();
    }
    else if(currentTime <= getAsr() && settingsCache->asrEnabled)
    {
        //dout<< "HIT ASR";

        nextPrayerLabel = tr("Asr");
        return getAsr();
    }
    else if(currentTime <= getMagrib() && settingsCache->magribEnabled)
    {
        //dout<< "HIT MAGRIB";

        nextPrayerLabel = tr("Magrib");
        return getMagrib();
    }
    else if(currentTime <= getIsha() && settingsCache->ishaEnabled)
    {
        //dout<< "HIT ISHA";

        nextPrayerLabel = tr("Isha");
        return getIsha();
    }
    else
    {
        if(settingsCache->fajrEnabled)  // This is need again, hard to explain why!!!
        {
            //dout<< "HIT FAJR L";

            nextPrayerLabel = tr("Fajr");
            return getFajr();
        }
        else if(settingsCache->duhrEnabled)
        {
            //dout<< "HIT DUHR L";

            nextPrayerLabel = tr("Duhr");
            return getDuhr();
        }
        else if(settingsCache->asrEnabled)
        {
            //dout<< "HIT ASR L";

            nextPrayerLabel = tr("Asr");
            return getAsr();
        }
        else if(settingsCache->magribEnabled)
        {
            //dout<< "HIT MAGRIB L";

            nextPrayerLabel = tr("Magrib");
            return getMagrib();
        }
        else if(settingsCache->ishaEnabled)
        {
            //dout<< "HIT ISHA L";

            nextPrayerLabel =tr( "Isha");
            return getIsha();
        }
        else
        {
            //dout<< "NONE";

            nextPrayerLabel = "";
            return QTime();
        }
    }
}

QString MunadiEngine::getNextPrayerLabel()
{
    return nextPrayerLabel;
}

QString MunadiEngine::getCurrPrayerLabel()
{
    return currentPrayerLabel;
}

void MunadiEngine::checkAthan()
{
    calculatePrayer();

    QTime nextPrayer = getNextPrayer();

    if(nextPrayer.isNull())    return;

    QTime currentTime(QTime::currentTime().hour(),QTime::currentTime().minute(), QTime::currentTime().second());

    if(currentTime == nextPrayer)
    {
        playAthan();
    }

    if(currentTime.second() == 0)
    {
        dout << "Minute elapsed, update time left";
        emit minuteElapsed();
    }
}

QString MunadiEngine::getHijriDate()
{
    sDate hDate;

    // Convert using hijri code from meladi to hijri
    int ret = h_date(&hDate,
                     QDate::currentDate().day(),
                     QDate::currentDate().month(),
                     QDate::currentDate().year());

    if (ret)
        return "";

    QString hijriDate;
    hijriDate = hijriDate.append(QString::number(hDate.day));   // Date number
    hijriDate = hijriDate.append(" ");   // Date name
    hijriDate = hijriDate.append(hDate.to_mname);   // Date name
    hijriDate = hijriDate.append(", ");   // Date name
    hijriDate = hijriDate.append(QString::number(hDate.year));   // Date name

    hijriEvent = "";

    for (int i = 0; hDate.event[i] != NULL; i++)
    {
       //printf("  Day's Event      - %s\n", mydate.event[i]);
        hijriEvent.append(hDate.event[i]);
    }
    free(hDate.event);

    return hijriDate;
}

QString MunadiEngine::getHijriEvent()
{
    return hijriEvent;
}
#ifdef DESKTOP
void MunadiEngine::createTrayMenu()
{
    trayMenu->addAction("Play Athan", this, SLOT(playAthan()));
    trayMenu->addAction("Stop Athan", this, SLOT(stopAthan()));
    trayMenu->addSeparator();
    trayMenu->addAction("Toggle Visiblity", this, SLOT(toggleView()));
    trayMenu->addSeparator();
    trayMenu->addAction("Close Munadi", parent, SLOT(close()));
}

void MunadiEngine::setStartup(bool set)
{
#ifdef Q_OS_WIN

    dout << "void MunadiEngine::setStarup(bool set)";

    if(set)
    {
        QSettings startup("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                          QSettings::NativeFormat);
        startup.setValue(QApplication::applicationName(), "\"" + QApplication::applicationFilePath().replace('/','\\') + "\"" + " -startup");
    }
    else
    {
        QSettings startup("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                          QSettings::NativeFormat);
        startup.remove(QApplication::applicationName());
    }
#endif
}
#endif
void MunadiEngine::refreshSettingsCache()
{
    settingsCache->refresh();
    calculatePrayer();
    getNextPrayer();
    emit minuteElapsed();   // Well, so both time left and prayer times update
}

void MunadiEngine::setVolume(int level, bool mute)
{
    athanObject->setMuted(mute);
    athanObject->setVolume(level);
}

bool MunadiEngine::eventFilter(QObject * object, QEvent * event)
{
    //dout << "QObject: " << object << " QEvent: " << event;

    if (event->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent *e = (QWindowStateChangeEvent*)event;

        if ( (e->oldState() != Qt::WindowMinimized) && (parent->windowState() & Qt::WindowMinimized) )
        {
            QTimer::singleShot(0, parent, SLOT(hide()));
            event->ignore();
        }
    }

    return false;
}

bool MunadiEngine::init()
{
    QSettings qSettings;
#ifdef DESKTOP
    QString athanFile = QApplication::applicationDirPath() + "/audio/athan.mp3";
#endif
#ifdef Q_OS_ANDROID
    QString athanFile = "assets:/audio/athan.ogg";
#endif

    dout << "Athan file: " << athanFile;

    if( (athanObject = new QMediaPlayer()) == 0)    exit(1);
    athanObject->setMedia(QUrl::fromLocalFile((athanFile)));
    athanObject->setVolume(qSettings.value("Athan/volume", 80).toInt());
    athanObject->setMuted(qSettings.value("Athan/muted", false).toBool());

    calculatePrayer();
    getNextPrayer();

    timer->setInterval(1000);
    timer->start();

    connect(timer, SIGNAL(timeout()), this, SLOT(checkAthan()));
    connect(athanObject, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
#ifdef DESKTOP
    parent->setGeometry(qSettings.value("Window/geometry",QRect(400,300,300,450)).toRect());
    parent->setWindowState( (Qt::WindowState) qSettings.value("Window/state",0).toInt());
    setStartup(qSettings.value("General/autoStartUp", true).toBool());

    if(qSettings.value("General/checkForUpdates", true).toBool())
        updater = new Updater();

    trayMenu = new QMenu("trayMenu");
    createTrayMenu();

    tray = new QSystemTrayIcon(QIcon(":/img/munadi.png"), this);
    tray->setContextMenu(trayMenu);
    tray->show();
#endif
    parent->setTitle(tr("Munadi"));

#ifdef Q_OS_WIN //Mac doesn't like this
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    CreateMutex(0, true, L"99da0a5f-1f0d-4e14-97f6-b0919b9ec9cd");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {

        QMessageBox::information(0, tr("Munadi Is Running"),
                                 tr("You cannot start more than one instance of Munadi."));
        exit(0);
    }
#endif

    return true;
}
