#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDir>
#include <QObject>
#include <QSettings>
#include <QUrl>
#include "parser/KhinsiderParser.h"

class Settings : public QObject {
    Q_OBJECT

    Q_ENUM(DownloadQuality)

    Q_PROPERTY(QString downloadPath READ downloadPath WRITE setDownloadPath NOTIFY downloadPathChanged)
    Q_PROPERTY(bool enableLogging READ enableLogging WRITE setEnableLogging NOTIFY enableLoggingChanged)
    Q_PROPERTY(int downloadThreads READ downloadThreads WRITE setDownloadThreads NOTIFY downloadThreadsChanged)
    Q_PROPERTY(
        int maxConcurrentDownloadsPerThread READ maxConcurrentDownloadsPerThread
        WRITE setMaxConcurrentDownloadsPerThread NOTIFY maxConcurrentDownloadsPerThreadChanged)

    Q_PROPERTY(DownloadQuality preferredAudioQuality READ preferredAudioQuality WRITE setPreferredAudioQuality NOTIFY preferredAudioQualityChanged)
    Q_PROPERTY(bool downloadArt READ downloadArt WRITE setDownloadArt NOTIFY downloadArtChanged)
    Q_PROPERTY(bool skipDownloaded READ skipDownloaded WRITE setSkipDownloaded NOTIFY settingsChanged)

public:
    explicit Settings(QObject *parent = nullptr)
        : QObject(parent) {
    }

    QString downloadPath() { return m_downloadPath; }
    bool enableLogging() { return m_enableLogging; }
    int downloadThreads() { return m_downloadThreads; }
    DownloadQuality preferredAudioQuality() { return m_preferredAudioQuality; }

    void ParseFromConfig(QSettings &settings);

    int maxConcurrentDownloadsPerThread() { return m_maxConcurrentDownloadsPerThread; }
    bool downloadArt() { return m_downloadArt; }
signals:
    void downloadPathChanged();

    void enableLoggingChanged();

    void downloadThreadsChanged();

    void preferredAudioQualityChanged();

    void settingsChanged();

    void downloadArtChanged();

    void maxConcurrentDownloadsPerThreadChanged();

public slots:
    void setMaxConcurrentDownloadsPerThread(int maxConcurrentDownloadsPerThread) {
        m_maxConcurrentDownloadsPerThread = maxConcurrentDownloadsPerThread;
        emit maxConcurrentDownloadsPerThreadChanged();
        emit settingsChanged();
    }

    bool skipDownloaded() { return m_skipDownloaded; }

    void setSkipDownloaded(bool skipDownloaded) {
        m_skipDownloaded = skipDownloaded;
        emit settingsChanged();
    }

    void setDownloadArt(bool value) {
        m_downloadArt = value;
        emit downloadArtChanged();
        emit settingsChanged();
    }

    void setDownloadPath(const QString &downloadPath) {
        if (downloadPath != m_downloadPath) {
            const QUrl url(downloadPath);
            if (url.isLocalFile()) {
                m_downloadPath = (QDir::toNativeSeparators(url.toLocalFile()));
            } else {
                m_downloadPath = downloadPath;
            }
            QDir().mkpath(m_downloadPath);
            emit downloadPathChanged();
            emit settingsChanged();
        }
    }

    void setEnableLogging(bool enableLogging) {
        if (enableLogging != m_enableLogging) {
            m_enableLogging = enableLogging;
            emit enableLoggingChanged();
            emit settingsChanged();
        }
    }

    void setDownloadThreads(int downloadThreads) {
        if (downloadThreads != m_downloadThreads) {
            m_downloadThreads = downloadThreads;
            emit downloadThreadsChanged();
            emit settingsChanged();
        }
    }

    void setPreferredAudioQualityInt(int preferredAudioQualityInt) {
        auto preferredAudioQuality = static_cast<DownloadQuality>(preferredAudioQualityInt);
        if (preferredAudioQuality != m_preferredAudioQuality) {
            m_preferredAudioQuality = preferredAudioQuality;
            emit preferredAudioQualityChanged();
            emit settingsChanged();
        }
    }

    void setPreferredAudioQuality(DownloadQuality preferredAudioQuality) {
        if (preferredAudioQuality != m_preferredAudioQuality) {
            m_preferredAudioQuality = preferredAudioQuality;
            emit preferredAudioQualityChanged();
            emit settingsChanged();
        }
    }

private:
    QString m_downloadPath;
    bool m_downloadArt = false;
    bool m_enableLogging;
    int m_maxConcurrentDownloadsPerThread = 5;
    int m_downloadThreads = 1;
    DownloadQuality m_preferredAudioQuality;
    bool m_skipDownloaded = true;
};


#endif //SETTINGS_H
