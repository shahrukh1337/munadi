#include "munadiengine.h"
#include "settings.h"
#include "settingsdialog.h"
#include "mainwindow.h"
#include <QtWidgets/QMessageBox>
#include <QTime>
#include <QSettings>
#include <QQuickView>

MunadiEngine::MunadiEngine(void *parent)
    : QObject(0)
    , parent(parent)
    , athanObject(0)
    , settings(new Settings())
    , timer(new QTimer(this))
{
    init();
}
MunadiEngine::~MunadiEngine()
{
    stopAthan();

    QSettings().setValue("mainWindow", ((QQuickView *) parent)->geometry());
    if(athanObject) delete athanObject;
    if(settings)    delete settings;
}
void MunadiEngine::calculatePrayer()
{
    Date date;  // Different from QDate
    date.day = QDate::currentDate().day();
    date.month = QDate::currentDate().month();
    date.year = QDate::currentDate().year();

    getPrayerTimes(&settings->currentCity.location,
                   &settings->currentCity.method,
                   &date,
                   settings->currentCity.ptList);
}

void MunadiEngine::calculateAngle()
{
    settings->qiblaAngle = getNorthQibla(&settings->currentCity.location);
}

void MunadiEngine::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    dout << "onMediaStatusChanged: " << status;

    switch(status)
    {
    case QMediaPlayer::EndOfMedia:
        QTimer::singleShot(0, this, SIGNAL(athanStopped()) );    // Needed so events get through, for player to update state
        break;
    }
}

QTime MunadiEngine::getFajr()
{
    return QTime(settings->currentCity.ptList[Fajr].hour,
                 settings->currentCity.ptList[Fajr].minute).addSecs(settings->farjAdjustment * 60);
}
QTime MunadiEngine::getDuhr()
{
    return QTime(settings->currentCity.ptList[Duhr].hour,
                 settings->currentCity.ptList[Duhr].minute).addSecs(settings->duhrAdjustment * 60);
}
QTime MunadiEngine::getAsr()
{
    return QTime(settings->currentCity.ptList[Asr].hour,
                 settings->currentCity.ptList[Asr].minute).addSecs(settings->asrAdjustment * 60);
}
QTime MunadiEngine::getMagrib()
{
    return QTime(settings->currentCity.ptList[Magrib].hour,
                 settings->currentCity.ptList[Magrib].minute).addSecs(settings->magribAdjustment * 60);
}
QTime MunadiEngine::getIsha()
{
    return QTime(settings->currentCity.ptList[Isha].hour,
                 settings->currentCity.ptList[Isha].minute).addSecs(settings->ishaAdjustment * 60);
}
QTime MunadiEngine::getTest()
{
    return testTime;
}
QTime MunadiEngine::getSunrise()
{
    return QTime(settings->currentCity.ptList[Sunrise].hour,
                 settings->currentCity.ptList[Sunrise].minute);
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

    athanObject->play();

    emit athanStarted();
}
void MunadiEngine::stopAthan()
{
    athanObject->stop();
    emit athanStopped();
}

QString MunadiEngine::getLocationName()
{
    return settings->getCity().name;
}

void MunadiEngine::ShowSettings()
{
    settingsDialog * sd = new settingsDialog(this, 0);
    sd->show();
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

    return time.toString("H 'hours' m 'minutes'");
}

QTime MunadiEngine::getNextPrayer()
{
    QTime currentTime(QTime::currentTime().hour(),QTime::currentTime().minute(), QTime::currentTime().second());

    if(currentTime <= getFajr() && settings->farjEnabled)
    {
        dout<< "HIT FAJR";

        nextPrayerLabel = "Fajr";
        return getFajr();
    }
    else if(currentTime <= getDuhr() && settings->duhrEnabled)
    {
        dout<< "HIT THUHR";

        nextPrayerLabel = "Duhr";
        return getDuhr();
    }
    else if(currentTime <= getAsr() && settings->asrEnabled)
    {
        dout<< "HIT ASR";

        nextPrayerLabel = "Asr";
        return getAsr();
    }
    else if(currentTime <= getMagrib() && settings->magribEnabled)
    {
        dout<< "HIT MAGRIB";

        nextPrayerLabel = "Magrib";
        return getMagrib();
    }
    else if(currentTime <= getIsha() && settings->ishaEnabled)
    {
        dout<< "HIT ISHA";

        nextPrayerLabel = "Isha";
        return getIsha();
    }
    else
    {
        if(settings->farjEnabled)  // This is need again, hard to explain why!!!
        {
            dout<< "HIT FAJR L";

            nextPrayerLabel = "Fajr";
            return getFajr();
        }
        else if(settings->duhrEnabled)
        {
            dout<< "HIT DUHR L";

            nextPrayerLabel = "Duhr";
            return getDuhr();
        }
        else if(settings->asrEnabled)
        {
            dout<< "HIT ASR L";

            nextPrayerLabel = "Asr";
            return getAsr();
        }
        else if(settings->magribEnabled)
        {
            dout<< "HIT MAGRIB L";

            nextPrayerLabel = "Magrib";
            return getMagrib();
        }
        else if(settings->ishaEnabled)
        {
            dout<< "HIT ISHA L";

            nextPrayerLabel = "Isha";
            return getIsha();
        }
        else
        {
            dout<< "NONE";

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
    //dout << "checkAthan() called";

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
    if(hijriEvent == "")
        return "Subhan Allah, Alhamdulliah, Allahuakbar";
    else
        return hijriEvent;
}

bool MunadiEngine::init()
{
    QString athanFile;
    athanFile = settings->athanPath;
    dout << "Athan file: " << athanFile;

    if( (athanObject = new QMediaPlayer()) == 0)    exit(1);
    athanObject->setMedia(QUrl::fromLocalFile((athanFile)));
    athanObject->setVolume(100);

    calculatePrayer();
    getNextPrayer();

    timer->setInterval(1000);
    timer->start();

    connect(timer, SIGNAL(timeout()), this, SLOT(checkAthan()));
    connect(athanObject, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));

//    QTimer::singleShot(5000, this, SLOT(playAthan()));
//    QTimer::singleShot(8000, this, SLOT(stopAthan()));

    ((QQuickView *) parent)->setGeometry(QSettings().value("mainWindow",QRect(400,300,300,450)).toRect());

    //    if(munadi->settings->showOnAthan)
    //    {
    //        ui->actionShow_On_Athan->setChecked(true);
    //        connect(munadi, SIGNAL(athanTriggered()), this, SLOT(show()));
    //    }

    //    if(munadi->settings->checkForUpdates)
    //    {
    //        ui->actionCheck_for_updates->setChecked(true);
    //        Updater * up = new Updater(this);
    //    }

    //    if(munadi->settings->compactMode)
    //    {
    //        on_actionCompact_mode_triggered(true);
    //        ui->actionCompact_mode->setChecked(true);
    //    }
    //    else
    //    {
    //        on_actionCompact_mode_triggered(false);
    //    }

    //#ifdef Q_OS_WIN
    //    QSettings startup("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
    //                      QSettings::NativeFormat);
    //    QString ret = startup.value(QApplication::applicationName(), "0").toString();
    //    if(ret.toUpper() != "0")
    //    {
    //        ui->actionStart_with_PC->setChecked(true);
    //    }
    //#endif

    //#ifdef Q_OS_MAC
    //    ui->actionStart_with_PC->setVisible(false);
    //#endif

    //    connect(munadi, SIGNAL(stateChanged()), this, SLOT(updateDisplay()));
    //    updateDisplay();

    //    trayMenu = new QMenu("trayMenu", this);
    //    createTrayMenu();

    //    tray = new QSystemTrayIcon(QIcon(":/icons/app"), this);
    //    tray->setContextMenu(trayMenu);
    //    tray->show();

    //#ifdef Q_OS_WIN
    //    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    //            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    //#endif

    return true;
}

bool MunadiEngine::eventFilter(QObject * object, QEvent * event)
{
    //dout << "QObject: " << object << " QEvent: " << event;

    return false;
}
