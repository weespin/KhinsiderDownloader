#include "SettingsController.h"

#include <qversionnumber.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

SettingsController::SettingsController(QObject *parent): QObject(parent), m_settings("config.ini", QSettings::IniFormat) {

    QSettings::setDefaultFormat(QSettings::IniFormat);
    m_SettingsObject = new Settings(parent);
    m_SettingsObject->ParseFromConfig(m_settings);
    m_settingsVM = new SettingsViewModel(parent);
    m_settingsVM->setSettingsObject(m_SettingsObject);
    connect(m_SettingsObject, &Settings::settingsChanged, m_settingsVM, &SettingsViewModel::settingsChanged);
    connect(m_SettingsObject, &Settings::settingsChanged, this, &SettingsController::saveConfig);
    connect(this, &SettingsController::foundNewUpdate,m_settingsVM, &SettingsViewModel::updateFound);
    checkForUpdates();
}

void SettingsController::checkForUpdates()
{
    QString currentVersion = readCurrentVersion();

    // Perform the API request to get the latest release version
    QNetworkRequest request(QUrl("https://api.github.com/repos/weespin/KhinsiderDownloader/releases"));
    request.setHeader(QNetworkRequest::UserAgentHeader, "KhinsiderDownloader");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &SettingsController::onUpdateCheckFinished);
    manager->get(request);
}

  void SettingsController::onUpdateCheckFinished(QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Failed to check for updates!";
            reply->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonArray releases = doc.array();

        QString currentVersion = this->readCurrentVersion();

        QList<QJsonObject> newerReleases;

        for (const QJsonValue& value : releases) {
            QJsonObject release = value.toObject();
            QString tagName = release["tag_name"].toString();
            bool isPrerelease = release["prerelease"].toBool();

            if (isPrerelease && isNewVersion(tagName, currentVersion)) {
                newerReleases.append(release);
            }
        }

        std::sort(newerReleases.begin(), newerReleases.end(), [](const QJsonObject& a, const QJsonObject& b) {
            return QVersionNumber::fromString(a["tag_name"].toString()) <
                   QVersionNumber::fromString(b["tag_name"].toString());
        });

        if (!newerReleases.isEmpty()) {
            QJsonObject latestRelease = newerReleases.first();
            QString latestVersion = latestRelease["tag_name"].toString();

                emit foundNewUpdate(latestVersion);
        } else {
            //We're ahead of release!
        }

        reply->deleteLater();
    }

void SettingsController::saveConfig() {
    m_settings.setValue("downloadPath", m_SettingsObject->downloadPath());
    m_settings.setValue("suppressLogs", m_SettingsObject->suppressLogs());
    m_settings.setValue("downloadThreads", m_SettingsObject->downloadThreads());
    m_settings.setValue("preferredAudioQuality", static_cast<int>(m_SettingsObject->preferredAudioQuality()));
    m_settings.setValue("maxConcurrentDownloadsPerThread", static_cast<int>(m_SettingsObject->maxConcurrentDownloadsPerThread()));
    m_settings.setValue("downloadArt", static_cast<bool>(m_SettingsObject->downloadArt()));
    m_settings.setValue("skipDownloaded", static_cast<bool>(m_SettingsObject->skipDownloaded()));
    m_settings.sync();
}

QString SettingsController::readCurrentVersion() {
    QFile file(":/APP_VERSION");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        return in.readLine().trimmed();
    }
    return "";
}

bool SettingsController::isNewVersion(const QString &latestVersion, const QString &currentVersion) {
    QVersionNumber currentVer = QVersionNumber::fromString(currentVersion.split('-').first());
    QVersionNumber latestVer = QVersionNumber::fromString(latestVersion.split('-').first());

    // Compare the version numbers
    return latestVer > currentVer;
}
