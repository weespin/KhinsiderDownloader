#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDir>
#include <QObject>
#include <QSettings>
#include <QUrl>
#include "parser/KhinsiderParser.h"

class Settings : public QObject
{
    Q_OBJECT

    Q_ENUM(DownloadQuality)

    Q_PROPERTY(QString downloadPath READ downloadPath NOTIFY downloadPathChanged)
    Q_PROPERTY(bool suppressLogs READ suppressLogs NOTIFY suppressLogsChanged)
    Q_PROPERTY(int downloadThreads READ downloadThreads NOTIFY downloadThreadsChanged)
    Q_PROPERTY(int maxConcurrentDownloadsPerThread READ maxConcurrentDownloadsPerThread NOTIFY maxConcurrentDownloadsPerThreadChanged)
    Q_PROPERTY(DownloadQuality preferredAudioQuality READ preferredAudioQuality NOTIFY preferredAudioQualityChanged)
    Q_PROPERTY(bool downloadArt READ downloadArt NOTIFY downloadArtChanged)
    Q_PROPERTY(bool skipDownloaded READ skipDownloaded NOTIFY settingsChanged)
public:
    explicit Settings(QObject *parent = nullptr)
         : QObject(parent) {
    }
    QString downloadPath(){return m_downloadPath;}
    bool suppressLogs(){return m_suppressLogs;}
    int downloadThreads(){return m_downloadThreads;}
    DownloadQuality preferredAudioQuality(){return m_preferredAudioQuality;}
    void ParseFromConfig(QSettings &settings);
    int maxConcurrentDownloadsPerThread(){return m_maxConcurrentDownloadsPerThread;}
    bool downloadArt(){return m_downloadArt;}
signals:
    void downloadPathChanged();
    void suppressLogsChanged();
    void downloadThreadsChanged();
    void preferredAudioQualityChanged();
    void settingsChanged();
    void downloadArtChanged();
    void maxConcurrentDownloadsPerThreadChanged();
public slots:
    void setMaxConcurrentDownloadsPerThread(int maxConcurrentDownloadsPerThread) {
    m_maxConcurrentDownloadsPerThread = maxConcurrentDownloadsPerThread;
   emit maxConcurrentDownloadsPerThreadChanged(); emit settingsChanged();
}
    bool skipDownloaded(){return m_skipDownloaded;}
    void setSkipDownloaded(bool skipDownloaded) {
    m_skipDownloaded = skipDownloaded;
    emit settingsChanged();
}
    void setDownloadArt(bool value){m_downloadArt = value; emit suppressLogsChanged(); emit settingsChanged();}
    void setDownloadPath(const QString& downloadPath)
    {
        if (downloadPath != m_downloadPath)
        {
            const QUrl url(downloadPath);
            if (url.isLocalFile()) {
                m_downloadPath = (QDir::toNativeSeparators(url.toLocalFile()));
            } else {
               m_downloadPath = downloadPath;
            }
            emit downloadPathChanged();
            emit settingsChanged();
        }

    }
    void setSuppressLogs(bool suppressLogs) {
        if (suppressLogs != m_suppressLogs)
        {
            m_suppressLogs = suppressLogs;
            emit suppressLogsChanged();
            emit settingsChanged();
        }
    }
    void setDownloadThreads(int downloadThreads)
    {
        if (downloadThreads != m_downloadThreads) {
            m_downloadThreads = downloadThreads;
            emit downloadThreadsChanged();
            emit settingsChanged();
        }
    }
    void setPreferredAudioQualityInt(int preferredAudioQualityInt)
    {
        auto preferredAudioQuality = static_cast<DownloadQuality>(preferredAudioQualityInt);
        if (preferredAudioQuality != m_preferredAudioQuality) {
            m_preferredAudioQuality = preferredAudioQuality;
            emit preferredAudioQualityChanged();
            emit settingsChanged();
        }
    }
    void setPreferredAudioQuality(DownloadQuality preferredAudioQuality)
    {
        if (preferredAudioQuality != m_preferredAudioQuality) {
            m_preferredAudioQuality = preferredAudioQuality;
            emit preferredAudioQualityChanged();
            emit settingsChanged();
        }
    }


private:
    QString m_downloadPath;
    bool m_downloadArt = false;
    bool m_suppressLogs;
    int m_maxConcurrentDownloadsPerThread = 5;
    int m_downloadThreads = 1;
    DownloadQuality m_preferredAudioQuality;
    bool m_skipDownloaded = true;
};



#endif //SETTINGS_H
