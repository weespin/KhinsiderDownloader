#ifndef ALBUMVIEWMODEL_H
#define ALBUMVIEWMODEL_H

#include <QObject>
#include "SearchController.h"
#include "SearchResultModel.h"

class SearchViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SearchResultModel* searchResult READ searchResult NOTIFY searchResultChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex NOTIFY selectedIndexChanged)
public:
    explicit SearchViewModel(QObject *parent = nullptr)
        : QObject(parent)
    {
        m_resultModel = new SearchResultModel(this);
    }

    SearchResultModel* searchResult() const { return m_resultModel; }
    int selectedIndex() const { return m_selectedIndex; }

public slots:
    void setSelectedIndex(int index);

public slots:
    void onSearchResultsReceived(const QVector<QSharedPointer<Album>> &result)
    {
        m_selectedIndex = -1;
        m_resultModel->setAlbums(result);
        emit searchResultChanged();
    }

signals:
        void selectedIndexChanged();
        void albumSelected(QSharedPointer<Album> album);
        void searchResultChanged();
        void performSearch(const QString &query);
private:
    SearchResultModel* m_resultModel;
    int m_selectedIndex = -1;
};

inline void SearchViewModel::setSelectedIndex(int index) {
    QSharedPointer<Album> album = m_resultModel->getAlbumAt(index);
    if (album)
    {
        emit albumSelected(album);
    }
    m_selectedIndex = index;
    emit selectedIndexChanged();
}
#endif // ALBUMVIEWMODEL_H
