#include "DownloaderController.h"
#include "Album.h"

static const QMap<QString, int> qualityRanking = {
    {"FLAC", 4},
    {"ALAC", 3},
    {"AAC", 2},
    {"MP3", 1}
};

DownloaderController::DownloaderController(QObject *parent, Settings *settings)
    : QObject(parent), m_settings(settings) {
    m_downloadManager = new AsyncDownloadManager(parent);
    m_Model = new DownloaderModel();
    connect(m_Model, &DownloaderModel::addToDownloadList, this, &DownloaderController::addToDownloadList);
    connect(m_Model, &DownloaderModel::albumCancelRequested, this, &DownloaderController::cancelAlbum);
    connect(m_Model, &DownloaderModel::albumRetryRequested, this, &DownloaderController::retryAlbum);

    if (m_settings) {
        m_downloadManager->setDownloadLimitPerThread(m_settings->maxConcurrentDownloadsPerThread());
        m_downloadManager->setThreadCount(m_settings->downloadThreads());
    }
}

void DownloaderController::addAlbumToDownload(QSharedPointer<Album> album, DownloadQuality quality) {
    m_addedAlbums[album] = quality;
}

void DownloaderController::fetchFullAlbumData(QSharedPointer<Album> album) {
    if (album->isInfoParsed()) {
        return;
    }

    WDownloadReplyMemory *reply = new WDownloadReplyMemory(nullptr);
    connect(reply, &WDownloadReplyMemory::downloadFinished, this,
            [this, reply, album](QNetworkReply::NetworkError error) {
                if (error != QNetworkReply::NoError) {
                    album->setHasErrors(true);
                    reply->deleteLater();
                }
                htmlDocPtr doc = htmlReadMemory(reply->getData(),
                                                reply->getData().size(),
                                                nullptr, nullptr,
                                                HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
                if (!doc) {
                    qWarning() << "HTML read failed";
                    return;
                }
                if (album->isPlaylist()) {
                    KhinsiderParser::ParsePlaylist(doc, album);
                } else {
                    KhinsiderParser::ParseAlbumFullData(doc, album);
                }
                xmlFreeDoc(doc);

                DownloadQuality quality = m_addedAlbums[album];
                onAlbumDataFetched(album, quality);
                reply->deleteLater();
            }, Qt::QueuedConnection);
    m_downloadManager->enqueueDownload(
        reply,
        album->albumLink()
    );
}


void DownloaderController::onAlbumDataFetched(QSharedPointer<Album> album, DownloadQuality quality) {
    processSongDownloads(album, quality);
    if (m_settings->downloadArt()) {
        processArtDownloads(album);
    }
}

void DownloaderController::processSongDownloads(QSharedPointer<Album> album, DownloadQuality quality) {
    for (auto song: album->songs()) {
        if (!song->getAllDownloadLinksParsed()) {
            fetchSongDownloadLinks(song, quality, album);
        } else {
            album->setIsDownloading(true);
            downloadSongFile(song, quality, album);
        }
    }
}

void DownloaderController::processArtDownloads(QSharedPointer<Album> album) {
    for (auto downloadLink: album->albumImage()) {
        QString downloadPath = m_settings->downloadPath();

        QString albumName = album ? album->name() : "Unknown Album";

        const QString invalidChars = "\\/:*?\"<>|";
        for (QChar c: invalidChars) {
            albumName.remove(c);
        }

        QDir albumDir(downloadPath + QDir::separator() + albumName + QDir::separator() + "Art");
        if (!albumDir.exists()) {
            albumDir.mkpath(".");
        }

        QUrl url(downloadLink);
        QString fileName = QUrl::fromPercentEncoding(url.path().section('/', -1).toUtf8());

        for (const QChar &c: invalidChars) {
            fileName.replace(c, '_');
        }

        QString filePath = albumDir.path() + QDir::separator() + fileName;
        if (QFileInfo(filePath).exists() && QFileInfo(filePath).size() > 1024 && m_settings->skipDownloaded()) {
            return;
        }

        WDownloadReplyFile *artDownloadReply = new WDownloadReplyFile(nullptr);
        artDownloadReply->setFilePath(filePath);

        connect(artDownloadReply, &WDownloadReplyFile::downloadFinished, this,
                [this, album,fileName,artDownloadReply, filePath](QNetworkReply::NetworkError error) {
                    if (error != QNetworkReply::NoError) {
                        album->setHasErrors(true);

                        qWarning() << "Request failed downloading art:" << fileName << "to" << filePath;
                        artDownloadReply->deleteLater();
                        return;
                    }

                    qDebug() << "Successfully downloaded:" << fileName << "to" << filePath;

                    artDownloadReply->deleteLater();
                }, Qt::QueuedConnection);


        m_downloadManager->enqueueDownload(artDownloadReply, downloadLink);
    }
}

void DownloaderController::cancelAlbum(Album *album) {
    for (auto song: album->songs()) {
        song->cancel();
    }
    album->setIsDownloading(false);
    album->resetState();
    for (auto it = m_addedAlbums.begin(); it != m_addedAlbums.end(); ++it) {
        if (it.key().data() == album) {
            m_addedAlbums.erase(it);
            break;
        }
    }
}

void DownloaderController::retryAlbum(Album *album) {
    if (!album->hasErrors()) {
        return;
    }

    QSharedPointer<Album> albumPtr;
    DownloadQuality quality = DownloadQuality::BEST;

    for (auto it = m_addedAlbums.begin(); it != m_addedAlbums.end(); ++it) {
        if (it.key().data() == album) {
            albumPtr = it.key();
            quality = it.value();
            break;
        }
    }

    if (!albumPtr) {
        qWarning() << "Cannot retry - album not found in managed albums";
        return;
    }

    album->setHasErrors(false);

    if (!album->isInfoParsed()) {
        fetchFullAlbumData(albumPtr);
        return;
    }

    bool hasErroredSongs = false;
    for (auto song: album->songs()) {
        if (song->errored()) {
            hasErroredSongs = true;
            song->setErrored(false);
            song->setDownloaded(false);

            if (!song->getAllDownloadLinksParsed()) {
                fetchSongDownloadLinks(song, quality, albumPtr);
            } else {
                downloadSongFile(song, quality, albumPtr);
            }
        }
    }


    if (!hasErroredSongs) {
        album->resetState();
        fetchFullAlbumData(albumPtr);
    }
}

void DownloaderController::fetchSongDownloadLinks(QSharedPointer<Song> song, DownloadQuality quality,
                                                  QSharedPointer<Album> album) {
    WDownloadReplyMemory *songDownloadListReply = new WDownloadReplyMemory(nullptr);

    connect(songDownloadListReply, &WDownloadReplyMemory::downloadFinished, this,
            [this, song, quality, album, songDownloadListReply](QNetworkReply::NetworkError error) {
                if (error != QNetworkReply::NoError) {
                    album->setHasErrors(true);
                    song->setErrored(true);

                    qWarning() << "Request failed parsing song download links";
                    songDownloadListReply->deleteLater();
                    return;
                }

                htmlDocPtr doc = htmlReadMemory(songDownloadListReply->getData(),
                                                songDownloadListReply->getData().size(),
                                                nullptr, nullptr,
                                                HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
                KhinsiderParser::ParseDownloadLink(doc, song);
                xmlFreeDoc(doc);

                song->setAllDownloadLinksParsed(true);

                onSongLinksFound(song, quality, album);
                songDownloadListReply->deleteLater();
            }, Qt::QueuedConnection);

    connect(song.get(), &Song::wantToCancel, songDownloadListReply, &WDownloadReplyMemory::cancel,
            Qt::QueuedConnection);

    m_downloadManager->enqueueDownload(
        songDownloadListReply,
        "https://downloads.khinsider.com" + song.get()->songLink()
    );
}

void DownloaderController::onSongLinksFound(QSharedPointer<Song> song, DownloadQuality quality,
                                            QSharedPointer<Album> album) {
    album->setIsDownloading(true);
    downloadSongFile(song, quality, album);
}

void DownloaderController::downloadSongFile(QSharedPointer<Song> song, DownloadQuality quality,
                                            QSharedPointer<Album> album) {
    QString downloadLink = "";

    if (quality == DownloadQuality::BEST) {
        if (song.isNull()) {
            // Check pointer validity
            qWarning() << "Song pointer is null!";
            return;
        }

        const auto &links = song->downloadLinks(); // Store reference
        if (links.isEmpty()) {
            // Early exit if no links
            qWarning() << "No download links available for song:" << song->name();
            return;
        }

        int highestRank = 0;
        for (auto it = links.constBegin(); it != links.constEnd(); ++it) {
            int currentRank = qualityRanking.value(it.key(), 0);
            if (currentRank > highestRank) {
                highestRank = currentRank;
                downloadLink = it.value();
            }
        }
    }

    if (downloadLink.isEmpty()) {
        downloadLink = song->downloadLinks().value("MP3");
    }

    if (downloadLink.isEmpty()) {
        qWarning() << "No download link found for song:" << song->name();
        return;
    }

    QString downloadPath = m_settings->downloadPath();

    QString albumName = album ? album->name() : "Unknown Album";

    const QString invalidChars = "\\/:*?\"<>|";
    for (QChar c: invalidChars) {
        albumName.remove(c);
    }

    QDir albumDir(downloadPath + QDir::separator() + albumName);
    if (!albumDir.exists()) {
        albumDir.mkpath(".");
    }

    QUrl url(downloadLink);
    QString fileName = QUrl::fromPercentEncoding(url.path().section('/', -1).toUtf8());

    for (const QChar &c: invalidChars) {
        fileName.replace(c, '_');
    }

    QString filePath = albumDir.path() + QDir::separator() + fileName;

    if (QFileInfo(filePath).exists() && QFileInfo(filePath).size() > 1024 && m_settings->skipDownloaded()) {
        song->setDownloaded(true);
        emit songDownloadFinished(song);
        return;
    }

    WDownloadReplyFile *songDownloadReply = new WDownloadReplyFile(nullptr);
    songDownloadReply->setFilePath(filePath);

    connect(songDownloadReply, &WDownloadReplyFile::downloadFinished, this,
            [this, song,album, songDownloadReply, filePath](QNetworkReply::NetworkError error) {
                if (error != QNetworkReply::NoError) {
                    song->setErrored(true);
                    album->setHasErrors(true);

                    qWarning() << "Request failed downloading song:" << song->name() << "to" << filePath;
                    songDownloadReply->deleteLater();
                    return;
                }

                qDebug() << "Successfully downloaded:" << song->name() << "to" << filePath;
                song->setDownloaded(true);
                emit songDownloadFinished(song);
                songDownloadReply->deleteLater();
            }, Qt::QueuedConnection);

    connect(songDownloadReply, &WDownloadReplyBase::downloadSpeed,
            this, [this, song, songDownloadReply, filePath](qint64 speed) {
                song->setDownloadSpeed(speed);
            },
            Qt::QueuedConnection);

    connect(song.get(), &Song::wantToCancel, songDownloadReply, &WDownloadReplyMemory::cancel, Qt::QueuedConnection);
    m_downloadManager->enqueueDownload(songDownloadReply, downloadLink);
}

void DownloaderController::startDownload() {
    for (auto it = m_addedAlbums.begin(); it != m_addedAlbums.end(); ++it) {
        auto album = it.key();
        DownloadQuality quality = it.value();

        if (!album->isInfoParsed()) {
            fetchFullAlbumData(album);
        } else {
            processSongDownloads(album, quality);
        }
    }
}

void DownloaderController::onDownloadListAdd(QSharedPointer<Album> album, DownloadQuality quality) {
    addAlbumToDownload(album, quality);
    updateModelData();

    if (!album->isInfoParsed()) {
        fetchFullAlbumData(album);
    } else {
        processSongDownloads(album, quality);
    }
}

void DownloaderController::updateModelData() {
    QVector<Album *> albumsForModel;
    for (auto it = m_addedAlbums.begin(); it != m_addedAlbums.end(); ++it) {
        albumsForModel.append(it.key().data());
    }

    m_Model->setAlbums(albumsForModel);
}

void DownloaderController::addToDownloadList(const QString &List) {
    QStringList url_list = List.split('\n');
    for (const QString &url: std::as_const(url_list)) {
        bool alreadyAdded = false;
        for (auto it = m_addedAlbums.begin(); it != m_addedAlbums.end(); ++it) {
            QSharedPointer<Album> album = it.key();
            if (album->albumLink() == url) {
                alreadyAdded = true;
                break;
            }
        }

        if (alreadyAdded) {
            continue;
        }

        static const QRegularExpression albumre("game-soundtracks/album/(.*)");
        static const QRegularExpression playlistre("/playlist/(.*)");

        auto processMatch = [&](const QRegularExpressionMatch &match, const QString &url, bool isPlaylist) {
            QString albumName = match.captured(1);
            auto newAlbum = QSharedPointer<Album>::create();

            if (isPlaylist) {
                newAlbum->setIsPlaylist(true);
            }

            newAlbum->setName(albumName);
            newAlbum->setAlbumLink(url);
            onDownloadListAdd(newAlbum, m_settings->preferredAudioQuality());
        };

        QRegularExpressionMatch match = albumre.match(url);
        if (match.hasMatch()) {
            processMatch(match, url, false);
            continue;
        }

        match = playlistre.match(url);
        if (match.hasMatch()) {
            processMatch(match, url, true);
        }
    }
}

void DownloaderController::onThreadsNumberChanged() {
    m_downloadManager->setThreadCount(m_settings->downloadThreads());
}

void DownloaderController::onMaxConcurrentOperationsChanged() {
    m_downloadManager->setDownloadLimitPerThread(m_settings->maxConcurrentDownloadsPerThread());
}

void DownloaderController::requestAddAlbums(QVector<QSharedPointer<Album> > albums, DownloadQuality quality) {
    for (const auto &album: albums) {
        addAlbumToDownload(album, quality);

        if (!album->isInfoParsed()) {
            fetchFullAlbumData(album);
        } else {
            processSongDownloads(album, quality);
        }
    }

    updateModelData();
}

void DownloaderController::parseDownloadLinks(QSharedPointer<Album> album) {
    // unused
    for (QSharedPointer<Song> &song: album->songs()) {
        if (!song->getAllDownloadLinksParsed()) {
            fetchSongDownloadLinks(song, m_addedAlbums[album], album);
        }
    }
}
