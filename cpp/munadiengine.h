#ifndef MUNADIENGINE_H
#define MUNADIENGINE_H

#include <QtGui>
#include <QtCore>
#include "libitl/prayer.h"
#include "libitl/hijri.h"
#include "settings.h"
#include <QStringList>

#if defined(USE_SFML)
#include <SFML/Audio.hpp>
#else
#include <QtMultimedia/QMediaPlayer>
#endif

#define dout qDebug()

class MunadiEngine : public QObject
{
    Q_OBJECT

public:

    MunadiEngine(void * parent = 0);
    virtual ~MunadiEngine();

    enum PrayerType{Fajr, Sunrise, Duhr, Asr, Magrib, Isha};

    Q_INVOKABLE bool isAthanPlaying();

    Q_INVOKABLE QTime getFajr();
    Q_INVOKABLE QTime getDuhr();
    Q_INVOKABLE QTime getAsr();
    Q_INVOKABLE QTime getMagrib();
    Q_INVOKABLE QTime getIsha();
    Q_INVOKABLE QTime getTest();
    Q_INVOKABLE QTime getSunrise();
    Q_INVOKABLE QTime getNextPrayer();
    Q_INVOKABLE QString getHijriDate();
    Q_INVOKABLE QString getHijriEvent();
    Q_INVOKABLE QString getNextPrayerLabel();
    Q_INVOKABLE QString getCurrPrayerLabel();
    Q_INVOKABLE QString getTimeDifference();
    Q_INVOKABLE QString getLocationName();
    Q_INVOKABLE void ShowSettings();

    Settings * settings;
    QTime testTime;

#if defined(USE_SFML)
    sf::Music * athanObject;
#else
    QMediaPlayer * athanObject;
#endif

protected:
    virtual bool eventFilter(QObject *object, QEvent *);


private:

    void * parent;
    QTimer * timer;
    QString nextPrayerLabel;
    QString currentPrayerLabel;
    QString hijriEvent;

    bool init();

public slots:
    void playAthan();
    void stopAthan();
    void checkAthan();
    void calculatePrayer();
    void calculateAngle();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus);

signals:
    void athanStarted();
    void athanStopped();
    void minuteElapsed();

};

#endif // MUNADIENGINE_H
