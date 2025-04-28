#ifndef ALBUM_H
#define ALBUM_H
#include <QObject>
#include <QString>
#include <QMap>
#include <qset.h>
#include "KhinsiderParser.h"
#include "Song.h"

class Album : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY metadataChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY metadataChanged)
    Q_PROPERTY(QString year READ year WRITE setYear NOTIFY metadataChanged)
    Q_PROPERTY(QString publisher READ publisher WRITE setPublisher NOTIFY metadataChanged)
    Q_PROPERTY(QString developer READ developer WRITE setDeveloper NOTIFY metadataChanged)
    Q_PROPERTY(QString albumLink READ albumLink WRITE setAlbumLink NOTIFY metadataChanged)
    Q_PROPERTY(bool isPlaylist READ isPlaylist WRITE setIsPlaylist NOTIFY metadataChanged)
    Q_PROPERTY(QSet<QString> albumImage READ albumImage WRITE setAlbumImage NOTIFY metadataChanged)
    Q_PROPERTY(QVector<QString> platforms READ platforms WRITE setPlatforms NOTIFY metadataChanged)
    Q_PROPERTY(QVector<QString> formats READ formats WRITE setFormats NOTIFY metadataChanged)

    Q_PROPERTY(QVector<QSharedPointer<Song>> songs READ songs WRITE setSongs NOTIFY contentChanged)

    Q_PROPERTY(bool isInfoParsed READ isInfoParsed WRITE setIsInfoParsed NOTIFY stateChanged)
    Q_PROPERTY(bool isDownloading READ isDownloading WRITE setIsDownloading NOTIFY stateChanged)
    Q_PROPERTY(bool hasErrors READ hasErrors WRITE setHasErrors NOTIFY stateChanged)
    Q_PROPERTY(QString state READ state NOTIFY stateChanged)

public:
    explicit Album(QObject *parent = nullptr) : QObject(parent) {
    }

    QString name() const { return QString::fromStdString(m_name); }
    QString type() const { return QString::fromStdString(m_type); }
    QString year() const { return QString::fromStdString(m_year); }
    QString publisher() const { return QString::fromStdString(m_publisher); }
    QString developer() const { return QString::fromStdString(m_developer); }
    QString albumLink() const { return QString::fromStdString(m_albumLink); }
    bool isPlaylist() const { return m_isPlaylist; }
    QSet<QString> albumImage() const { return m_albumImage; }
    QVector<QString> platforms() const { return m_platforms; }
    QVector<QString> formats() const { return m_formats; }

    QVector<QSharedPointer<Song> > songs() const { return m_songs; }

    bool isInfoParsed() const { return m_isInfoParsed; }
    bool isDownloading() const { return m_isDownloading; }
    bool hasErrors() const { return m_hasErrors; }
    DownloadQuality downloadQuality() const { return m_downloadQuality; }

    void resetState();

    QString state() const {
        if (m_hasErrors) {
            return "errored";
        }
        if (m_isDownloading) {
            return "downloading";
        }
        if (!m_isInfoParsed) {
            return "unparsed";
        }
        if (m_isInfoParsed) {
            return "parsed";
        }

        return "unknown";
    }

    int downloadedSize() const {
        int totaldownloadedSize = 0;
        for (const auto &song: m_songs) {
            totaldownloadedSize += song->downloadedSize();
        }
        return totaldownloadedSize;
    }

    int downloadSpeed() const {
        int totaldownloadSpeed = 0;
        for (const auto &song: m_songs) {
            if (!song->downloaded()) {
                totaldownloadSpeed += song->downloadSpeed();
            }
        }
        return totaldownloadSpeed;
    }

    int downloadedSongs() const {
        int totaldownloadedSongs = 0;
        for (const auto &song: m_songs) {
            if (song->downloaded()) {
                totaldownloadedSongs++;
            }
        }
        return totaldownloadedSongs;
    }

    int totalSongs() const {
        return m_songs.size();
    }

    // Metadata setters
    void setName(const QString &name) {
        if (m_name != name.toStdString()) {
            m_name = name.toStdString();
            emit metadataChanged();
        }
    }

    void setType(const QString &type) {
        if (m_type != type.toStdString()) {
            m_type = type.toStdString();
            emit metadataChanged();
        }
    }

    void setYear(const QString &year) {
        if (m_year != year.toStdString()) {
            m_year = year.toStdString();
            emit metadataChanged();
        }
    }

    void setPublisher(const QString &publisher) {
        if (m_publisher != publisher.toStdString()) {
            m_publisher = publisher.toStdString();
            emit metadataChanged();
        }
    }

    void setDeveloper(const QString &developer) {
        if (m_developer != developer.toStdString()) {
            m_developer = developer.toStdString();
            emit metadataChanged();
        }
    }

    void setAlbumLink(const QString &albumLink) {
        if (m_albumLink != albumLink.toStdString()) {
            m_albumLink = albumLink.toStdString();
            emit metadataChanged();
        }
    }

    void setIsPlaylist(bool isPlaylist) {
        if (m_isPlaylist != isPlaylist) {
            m_isPlaylist = isPlaylist;
            emit metadataChanged();
        }
    }

    void setAlbumImage(const QSet<QString> &albumImage) {
        if (m_albumImage != albumImage) {
            m_albumImage = albumImage;
            emit metadataChanged();
        }
    }

    void addAlbumImage(const QString &albumImage) {
        m_albumImage.insert(albumImage);
        emit metadataChanged();
    }

    void setPlatforms(const QVector<QString> &platforms) {
        if (m_platforms != platforms) {
            m_platforms = platforms;
            emit metadataChanged();
        }
    }

    void addPlatform(const QString &platform) {
        m_platforms.push_back(platform);
        emit metadataChanged();
    }

    void setFormats(const QVector<QString> &formats) {
        if (m_formats != formats) {
            m_formats = formats;
            emit metadataChanged();
        }
    }

    void addFormat(const QString &format) {
        m_formats.push_back(format);
        emit metadataChanged();
    }

    void setSongs(const QVector<QSharedPointer<Song> > &songs) {
        // Disconnect old songs
        for (const auto &song: m_songs) {
            disconnectSong(song);
        }

        m_songs = songs;

        for (const auto &song: m_songs) {
            connectSong(song);
        }

        emit contentChanged();
    }

    void addSong(QSharedPointer<Song> song) {
        connectSong(song);
        m_songs.push_back(song);
        emit contentChanged();
    }

    void setIsInfoParsed(bool isInfoParsed) {
        if (m_isInfoParsed != isInfoParsed) {
            m_isInfoParsed = isInfoParsed;
            emit stateChanged();
        }
    }

    void setIsDownloading(bool isDownloading) {
        if (m_isDownloading != isDownloading) {
            m_isDownloading = isDownloading;
            emit stateChanged();
        }
    }

    void setHasErrors(bool hasErrors) {
        if (m_hasErrors != hasErrors) {
            m_hasErrors = hasErrors;
            emit stateChanged();
            emit progressUpdated();
        }
    }

    void setDownloadQuality(DownloadQuality downloadQuality) {
        if (m_downloadQuality != downloadQuality) {
            m_downloadQuality = downloadQuality;
        }
    }

signals:
    void metadataChanged(); // For album metadata changes
    void contentChanged(); // For song list changes
    void stateChanged(); // For state changes
    void progressUpdated(); // For download progress updates

public slots:
    void onSongDataChanged() {
        emit progressUpdated();
    }

private:
    void connectSong(const QSharedPointer<Song> &song) {
        connect(song.data(), &Song::dataChanged, this, &Album::onSongDataChanged);
        connect(song.data(), &Song::stateChanged, this, &Album::onSongDataChanged);
        connect(song.data(), &Song::downloadProgressChanged, this, &Album::onSongDataChanged);
    }

    void disconnectSong(const QSharedPointer<Song> &song) {
        disconnect(song.data(), &Song::dataChanged, this, &Album::onSongDataChanged);
        disconnect(song.data(), &Song::stateChanged, this, &Album::onSongDataChanged);
        disconnect(song.data(), &Song::downloadProgressChanged, this, &Album::onSongDataChanged);
    }

private:
    std::string m_name;
    std::string m_type;
    std::string m_year;
    std::string m_developer;
    std::string m_publisher;
    std::string m_albumLink;
    bool m_isPlaylist = false;
    QSet<QString> m_albumImage;
    QVector<QString> m_platforms;
    QVector<QString> m_formats;

    QVector<QSharedPointer<Song> > m_songs;
    //States
    bool m_isInfoParsed = false;
    bool m_isDownloading = false;
    bool m_hasErrors = false;
    DownloadQuality m_downloadQuality = UNKNOWN;
};


#endif //ALBUM_H
