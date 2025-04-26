#ifndef SONG_H
#define SONG_H


#include <QObject>
#include <QString>
#include <QMap>


class Song : public QObject
{
    Q_OBJECT

    // Group related properties with a single NOTIFY signal
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY dataChanged)
    Q_PROPERTY(int lengthInSeconds READ lengthInSeconds WRITE setLengthInSeconds NOTIFY dataChanged)
    Q_PROPERTY(QMap<QString, unsigned long> size READ size WRITE setSize NOTIFY dataChanged)
    Q_PROPERTY(QString songLink READ songLink WRITE setSongLink NOTIFY dataChanged)
    Q_PROPERTY(QMap<QString, QString> downloadLinks READ downloadLinks WRITE setDownloadLinks NOTIFY dataChanged)

    // State properties that can all use a single state change signal
    Q_PROPERTY(bool downloaded READ downloaded WRITE setDownloaded NOTIFY stateChanged)
    Q_PROPERTY(bool errored READ errored WRITE setErrored NOTIFY stateChanged)
    Q_PROPERTY(qint64 downloadedSize READ downloadedSize WRITE setDownloadedSize NOTIFY downloadProgressChanged)
    Q_PROPERTY(qint64 downloadSpeed READ downloadSpeed WRITE setDownloadSpeed NOTIFY downloadProgressChanged)
    //Q_PROPERTY(bool wantToCancel READ wantToCancel WRITE setWantToCancel NOTIFY downloadProgressChanged)


public:
    explicit Song(QObject *parent = nullptr) : QObject(parent) {}

    // Data accessors
    QString name() const { return QString::fromStdString(m_name); }
    int lengthInSeconds() const { return m_lengthInSeconds; }
    QMap<QString, unsigned long> size() const { return m_size; }
    QString songLink() const { return QString::fromStdString(m_songLink); }
    QMap<QString, QString> downloadLinks() const { return m_downloadLinks; }

    // State accessors
    bool downloaded() const { return m_downloaded; }
    bool errored() const { return m_erored; }
    qint64 downloadedSize() const { return m_downloadedSize; }
    qint64 downloadSpeed() const { return m_downloadSpeed; }
    // Additional accessors
    bool getAllDownloadLinksParsed() const { return m_downloadLinksParsed; }
    QString activeDownloadLink() const { return m_activeDownloadLink; }
    QString activeDownloadPath() const { return m_activeDownloadPath; }

    // Modified setters that reduce code duplication
    void cancel() {
        emit wantToCancel();
    }
    void setName(const QString &name) {
        if (m_name != name.toStdString()) {
            m_name = name.toStdString();
            emit dataChanged();
        }
    }

    void setLengthInSeconds(int lengthInSeconds) {
        if (m_lengthInSeconds != lengthInSeconds) {
            m_lengthInSeconds = lengthInSeconds;
            emit dataChanged();
        }
    }

    void setSize(const QMap<QString, unsigned long> &size) {
        if (m_size != size) {
            m_size = size;
            emit dataChanged();
        }
    }

    void addSize(const QString& format, unsigned long size) {
        m_size[format] = size;
        emit dataChanged();
    }

    void setSongLink(const QString &songLink) {
        if (m_songLink != songLink.toStdString()) {
            m_songLink = songLink.toStdString();
            emit dataChanged();
        }
    }

    void setDownloadLinks(const QMap<QString, QString> &downloadLinks) {
        if (m_downloadLinks != downloadLinks) {
            m_downloadLinks = downloadLinks;
            emit dataChanged();
        }
    }

    void addDownloadLink(const QString &format, const QString &url) {
        m_downloadLinks[format] = url;
        emit dataChanged();
    }

    // State setters
    void setDownloaded(bool downloaded) {
        if (m_downloaded != downloaded) {
            m_downloaded = downloaded;
            emit stateChanged();
        }
    }

    void setErrored(bool errored) {
        if (m_erored != errored) {
            m_erored = errored;
            emit stateChanged();
        }
    }

    void setDownloadedSize(qint64 downloadedSize) {
        m_downloadedSize = downloadedSize;
        emit downloadProgressChanged();
    }

    void setDownloadSpeed(qint64 downloadSpeed) {
        m_downloadSpeed = downloadSpeed;
        emit downloadProgressChanged();
    }

    // Additional setters
    void setAllDownloadLinksParsed(bool value) {
        m_downloadLinksParsed = value;
    }

    void setActiveDownloadLink(const QString &activeDownloadLink) {
        m_activeDownloadLink = activeDownloadLink;
    }

    void setActiveDownloadPath(const QString &activeDownloadPath) {
        m_activeDownloadPath = activeDownloadPath;
    }

signals:
    // Reduced to just 3 signals instead of many individual ones
    void dataChanged();          // For metadata changes
    void stateChanged();         // For state changes
    void downloadProgressChanged(); // For download progress updates
    void wantToCancel();

private:
    // Metadata
    std::string m_name;
    int m_lengthInSeconds = 0;
    QMap<QString, unsigned long> m_size;
    std::string m_songLink;
    QMap<QString, QString> m_downloadLinks;

    // State
    bool m_downloaded = false;
    bool m_erored = false;
    qint64 m_downloadedSize = 0;
    qint64 m_downloadSpeed = 0;

    // Additional data
    bool m_downloadLinksParsed = false;
    QString m_activeDownloadLink;
    QString m_activeDownloadPath;
};



#endif //SONG_H
