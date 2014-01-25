#ifndef MUNADIENGINE_H
#define MUNADIENGINE_H

#include <QtGui>
#include <QtCore>
#include <QtMultimedia/QMediaPlayer>

#if defined Q_OS_WIN || defined Q_OS_OSX
#define DESKTOP
#endif

#define APP_VERSION "14.01"
#define dout qDebug()

#include "libitl/prayer.h"
#include "libitl/hijri.h"
#include "settingscache.h"
#include <QStringList>

#ifdef DESKTOP
#include <QMenu>
#include <QSystemTrayIcon>
#endif


class MunadiEngine : public QObject
{
    Q_OBJECT

public:

    MunadiEngine(class QQuickView *parent = 0);
    virtual ~MunadiEngine();

    enum PrayerType{Fajr, Sunrise, Duhr, Asr, Magrib, Isha};

    Q_INVOKABLE bool isAthanPlaying();

    Q_INVOKABLE QTime getFajr();
    Q_INVOKABLE QTime getDuhr();
    Q_INVOKABLE QTime getAsr();
    Q_INVOKABLE QTime getMagrib();
    Q_INVOKABLE QTime getIsha();
    Q_INVOKABLE QTime getSunrise();
    Q_INVOKABLE QTime getNextPrayer();
    Q_INVOKABLE QString getHijriDate();
    Q_INVOKABLE QString getHijriEvent();
    Q_INVOKABLE QString getNextPrayerLabel();
    Q_INVOKABLE QString getCurrPrayerLabel();
    Q_INVOKABLE QString getTimeDifference();
    Q_INVOKABLE QString getLocationName();
    Q_INVOKABLE double getQibla();
    Q_INVOKABLE void refreshSettingsCache();
    Q_INVOKABLE void setVolume(int level, bool mute);
    Q_INVOKABLE QString getVersionNo() { return APP_VERSION; }
#ifdef DESKTOP
    Q_INVOKABLE void setStartup(bool set);
#endif

    Q_INVOKABLE bool isRtl()
    {
#ifdef ARABIC
    return true;
#else
    return false;
#endif
    }

    SettingsCache * settingsCache;
    QMediaPlayer * athanObject;

protected:
    virtual bool eventFilter(QObject *object, QEvent *);


private:

    class QQuickView * parent;
    QTimer * timer;
    QString nextPrayerLabel;
    QString currentPrayerLabel;
    QString hijriEvent;
#ifdef DESKTOP
    QSystemTrayIcon * tray;
    QMenu * trayMenu;
    void createTrayMenu();
    class Updater * updater;
#endif

    bool init();

public slots:
    void playAthan();
    void stopAthan();
    void checkAthan();
    void calculatePrayer();
    void showMainWindow();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void toggleView();

signals:
    void athanStarted();
    void athanStopped();
    void minuteElapsed();

};

#endif // MUNADIENGINE_H
