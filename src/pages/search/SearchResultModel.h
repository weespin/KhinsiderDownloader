#ifndef SEARCHRESULTMODEL_H
#define SEARCHRESULTMODEL_H
#include <QAbstractListModel>

#include "parser/Album.h"
class SearchResultModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
    };
    explicit SearchResultModel(QObject *parent = nullptr)
           : QAbstractListModel(parent) {}
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

    void setAlbums(const QVector<QSharedPointer<Album>> &albums) {
        beginResetModel();
        m_albums = albums;
        endResetModel();
    }

private:
    QVector<QSharedPointer<Album>> m_albums;
};
#endif //SEARCHRESULTMODEL_H
