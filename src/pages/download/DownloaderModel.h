#ifndef DOWNLOADERMODEL_H
#define DOWNLOADERMODEL_H
#include <QAbstractListModel>
#include "parser/KhinsiderParser.h"


class DownloaderModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        StatusRole,
        ProgressRole,
        SpeedRole,
        SizeRole,
        StateRole,
        DownloadedSongsRole,
        TotalSongsRole
    };

    explicit DownloaderModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Album *getAlbumAt(int index) const;

    Q_INVOKABLE double totalProgress() const;

    Q_INVOKABLE qint64 totalDownloadedSize() const;

    Q_INVOKABLE qint64 totalSpeed() const;

    Q_INVOKABLE qint64 totalSize() const;

    Q_INVOKABLE int totalDownloadedSongs() const;

    Q_INVOKABLE int totalSongs() const;

public slots:
    void cancelAllDownloads();

    void cancelAlbum(int index);

    void retryAlbum(int index);

    void setAlbums(const QVector<Album *> &albums);

    void insertAlbum(Album *album);

signals:
    void albumCancelRequested(Album *album);

    void albumRetryRequested(Album *album);

    void totalsChanged();

    void addToDownloadList(const QString &List);

private:
    QVector<Album *> m_albums;
};

#endif // DOWNLOADERMODEL_H
