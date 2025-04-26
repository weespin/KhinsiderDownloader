#include "DownloaderModel.h"
#include "parser/KhinsiderParser.h"
#include "parser/Album.h"
DownloaderModel::DownloaderModel(QObject *parent) : QAbstractListModel(parent) {}

int DownloaderModel::rowCount(const QModelIndex &parent) const {
    return m_albums.count();
}

QVariant DownloaderModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_albums.count())
        return QVariant();

    auto album = m_albums.at(index.row());
    switch (role) {
        case NameRole: return album->name();
        //case StatusRole: return album->status();
        case ProgressRole:
        {
            if (album->totalSongs() == 0) return 0;
            return (album->downloadedSongs() * 100) / album->totalSongs();
        };
        case SpeedRole: return album->downloadSpeed();
        case SizeRole: return album->downloadedSongs() / album->totalSongs();
        case StateRole: return album->state();
        case DownloadedSongsRole: return album->downloadedSongs();
        case TotalSongsRole: return album->totalSongs();
        default: return QVariant();
    }
}

QHash<int, QByteArray> DownloaderModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[StatusRole] = "status";
    roles[ProgressRole] = "progress";
    roles[SpeedRole] = "speed";
    roles[SizeRole] = "size";
    roles[StateRole] = "state";
    roles[DownloadedSongsRole] = "downloadedSongs";
    roles[TotalSongsRole] = "totalSongs";
    return roles;
}

double DownloaderModel::totalProgress() const {
    qint64 totalBytes = 0, downloadedBytes = 0;
    for (auto* album : m_albums) {
       // totalBytes += album->totalSize();
        downloadedBytes += album->downloadedSize();
    }
    return totalBytes ? (downloadedBytes * 100.0 / totalBytes) : 0;
}

qint64 DownloaderModel::totalDownloadedSize() const {
    qint64 total = 0;
    for (auto* album : m_albums) total += album->downloadedSize();
    return total;
}

qint64 DownloaderModel::totalSpeed() const {
    qint64 total = 0;
    for (auto* album : m_albums)
        total += album->downloadSpeed();
    return total;
}

qint64 DownloaderModel::totalSize() const {
    qint64 total = 0;
    //for (auto* album : m_albums) total += album->totalSize();
    return total;
}

int DownloaderModel::totalDownloadedSongs() const {
    int total = 0;
    for (auto* album : m_albums) total += album->downloadedSongs();
    return total;
}

int DownloaderModel::totalSongs() const {
    int total = 0;
    for (auto* album : m_albums) total += album->totalSongs();
    return total;
}

// Slots for canceling
void DownloaderModel::cancelAllDownloads() {
   for(auto* Album : m_albums)
   {
     emit albumCancelRequested(Album);
   }
   beginResetModel();
        m_albums.clear();
    endResetModel();
}

void DownloaderModel::cancelAlbum(int index) {
    if (index >= 0 && index < m_albums.size())
        emit albumCancelRequested(m_albums.at(index));
    beginResetModel();
        m_albums.removeAt(index);
    endResetModel();
}
void DownloaderModel::retryAlbum(int index) {
    if (index >= 0 && index < m_albums.size())
        emit albumRetryRequested(m_albums.at(index));
}


void DownloaderModel::setAlbums(const QVector<Album*> &albums) {
    beginResetModel();
    m_albums = albums;
    for (auto* album : m_albums) {
        connect(album, &Album::progressUpdated, this, [this, album]() {
            int row = m_albums.indexOf(album);
            if (row >= 0) emit dataChanged(index(row), index(row));
            emit totalsChanged();
        });
    }
    endResetModel();
}

void DownloaderModel::insertAlbum(Album* album) {
    beginInsertRows(QModelIndex(), m_albums.count(), m_albums.count());
    m_albums.append(album);
    connect(album, &Album::progressUpdated, this, [this, album]() {
     int idx = m_albums.indexOf(album);
     if (idx >= 0) {
         emit dataChanged(index(idx), index(idx));
         emit totalsChanged();
     }
 });
    endInsertRows();
}