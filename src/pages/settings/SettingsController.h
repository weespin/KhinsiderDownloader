#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H
#include <QObject>
#include <QString>
#include <QSettings>

#include "Settings.h"
#include "SettingsViewModel.h"
class SettingsController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SettingsViewModel* settingsVM READ settingsVM CONSTANT)

    public:
    explicit SettingsController(QObject *parent = nullptr);

public:
    SettingsViewModel* settingsVM() {return m_settingsVM;}
    void checkForUpdates();
signals:
    void foundNewUpdate(const QString& version);
public slots:
    void onUpdateCheckFinished(QNetworkReply* reply);

    void saveConfig();

private:
    QString readCurrentVersion();

    bool isNewVersion(const QString& latestVersion, const QString& currentVersion);

private:
    SettingsViewModel *m_settingsVM;
    QSettings m_settings;
    Settings* m_SettingsObject;
};



#endif //SETTINGSCONTROLLER_H
