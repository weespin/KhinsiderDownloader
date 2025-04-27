#ifndef SEARCHRESULTMODEL_H
#define SEARCHRESULTMODEL_H
#include <QAbstractListModel>

#include "parser/Album.h"

class SearchResultModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
    };

    explicit SearchResultModel(QObject *parent = nullptr)
        : QAbstractListModel(parent) {
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return m_albums.count();
    }

    QSharedPointer<Album> getAlbumAt(int index) const {
        if (index >= 0 && index < m_albums.count())
            return m_albums.at(index);
        return nullptr;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= m_albums.count())
            return QVariant();

        auto album = m_albums.at(index.row());

        switch (role) {
            case NameRole:
                return album->name();
        }

        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[NameRole] = "name";
        return roles;
    }

    void setAlbums(const QVector<QSharedPointer<Album> > &albums) {
        beginResetModel();
        m_albums = albums;
        endResetModel();
    }

    int selectedIndex() const { return m_selectedIndex; }
    QVector<QSharedPointer<Album> > &albums() { return m_albums; }

public slots:
    void onSearchResultsReceived(const QVector<QSharedPointer<Album> > &result) {
        m_selectedIndex = -1;
        setAlbums(result);
        emit searchCompleted();
    }

    void setSelectedIndex(int index) {
        QSharedPointer<Album> album = getAlbumAt(index);
        if (album) {
            emit albumSelected(album);
        }
        m_selectedIndex = index;
        emit selectedIndexChanged();
    }

signals:
    void searchStarted();

    void searchCompleted();

    void performSearch(const QString &query);

    void requestAddAlbums(QVector<QSharedPointer<Album> > albums, DownloadQuality quality);

    void selectedIndexChanged();

    void albumSelected(QSharedPointer<Album> album);

private:
    QVector<QSharedPointer<Album> > m_albums;
    int m_selectedIndex;
};
#endif //SEARCHRESULTMODEL_H
