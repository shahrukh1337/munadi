#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets/QApplication>
#include <QObject>

#include "libitl/prayer.h"

struct City
{
    QString name;
    int calcMethod;
    Location location;
    Method method;
    Prayer ptList[6];
};

class SettingsCache : public QObject
{
    Q_OBJECT

public: //TODO: change to private

    QSettings qSettings;
    City currentCity;
    bool fajrEnabled, duhrEnabled, asrEnabled, magribEnabled, ishaEnabled;
    int fajrAdjustment, duhrAdjustment, asrAdjustment, magribAdjustment, ishaAdjustment;

public:

    Q_INVOKABLE void refresh()
    {
        fajrEnabled = qSettings.value("Athan/fajrEnabled", true).toBool();
        duhrEnabled = qSettings.value("Athan/duhrEnabled", true).toBool();
        asrEnabled = qSettings.value("Athan/asrEnabled", true).toBool();
        magribEnabled = qSettings.value("Athan/magribEnabled", true).toBool();
        ishaEnabled = qSettings.value("Athan/ishaEnabled", true).toBool();

        fajrAdjustment = qSettings.value("Athan/fajrAdjustment", 0).toInt();
        duhrAdjustment = qSettings.value("Athan/duhrAdjustment", 0).toInt();
        asrAdjustment = qSettings.value("Athan/asrAdjustment", 0).toInt();
        magribAdjustment = qSettings.value("Athan/magribAdjustment", 0).toInt();
        ishaAdjustment = qSettings.value("Athan/ishaAdjustment", 0).toInt();

        //Location
        currentCity.name = qSettings.value("Location/name", "Mecca, Saudi Arabia").toString();
        currentCity.location.degreeLat = qSettings.value("Location/latitude", 21.42667).toDouble();
        currentCity.location.degreeLong = qSettings.value("Location/longitude", 39.82611).toDouble();
        currentCity.location.gmtDiff = qSettings.value("Location/gmtOffset", 3.0).toDouble();
        currentCity.location.dst = qSettings.value("Location/dst", 0).toBool();

        currentCity.calcMethod = qSettings.value("Method/calcMethod", 4).toInt();

        getMethod(currentCity.calcMethod, &currentCity.method);
        currentCity.method.mathhab = qSettings.value("Method/mathhab", 1).toInt() + 1;  //Combobox index starts 0

        //... defaults
        currentCity.method.round = 3;   //Better rounding for imsaak, agressive
        currentCity.location.pressure = 1010;
        currentCity.location.seaLevel = 0;
        currentCity.location.temperature = 10;
    }

    Q_INVOKABLE QStringList getCountries()
    {
        QFile f(":/data/countries.tsv");

        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
                 qDebug() << "Failed to open file!";
                 return QStringList();
        }

        QTextStream ts(&f);

        QStringList countries;

        while(!ts.atEnd())
        {
            countries << ts.readLine();
        }

        f.close();

        return countries;
    }

    Q_INVOKABLE QStringList getCities(QString cc)
    {
        QFile f(":/data/cities.tsv");

        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
                 qDebug() << "Failed to open file!";
                 return QStringList();
        }

        QTextStream ts(&f);

        QStringList cities;
        QString line;

        while(!ts.atEnd())
        {
            line = ts.readLine();

            if(line.startsWith(cc))
                cities << line;
        }

        f.close();

        return cities;
    }

    SettingsCache()
        :
          fajrEnabled(true),
          duhrEnabled(true),
          asrEnabled(true),
          magribEnabled(true),
          ishaEnabled(true),
          fajrAdjustment(0),
          duhrAdjustment(0),
          asrAdjustment(0),
          magribAdjustment(0),
          ishaAdjustment(0)
    {

        refresh();
    }
};

#endif // SETTINGS_H
