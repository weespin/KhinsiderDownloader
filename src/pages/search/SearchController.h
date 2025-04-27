#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QString>

#include "SearchAlbumInfoViewModel.h"
#include "SearchResultModel.h"
#include "parser/KhinsiderParser.h"

class Album;

class SearchController : public QObject {
    Q_OBJECT

    Q_PROPERTY(SearchAlbumInfoViewModel* albumInfoVM READ albumInfoVM CONSTANT)
    Q_PROPERTY(SearchResultModel* searchResultVM READ searchResultVM CONSTANT)

public:
    SearchAlbumInfoViewModel *albumInfoVM() { return m_albumInfoVM; }
    SearchResultModel *searchResultVM() { return m_searchResultVM; }

    explicit SearchController(QObject *parent = nullptr);

    void fetchFullAlbumData(QSharedPointer<Album> album);

signals:
    void onDownloadAllAddRequest(QVector<QSharedPointer<Album> > Albums, DownloadQuality quality);

    void onDownloadAddRequest(QSharedPointer<Album> Album, DownloadQuality quality);

    void searchResultsReceived(const QVector<QSharedPointer<Album> > &result);

    void onFullFetched();

    void searchStarted();

public slots:
    void doSearch(const QString &text);

    void addAllAlbumsToDownloads() {
        emit onDownloadAllAddRequest(m_searchResultVM->albums(), DownloadQuality::UNKNOWN);
    }

private:
    QNetworkAccessManager *networkManager;
    SearchResultModel *m_searchResultVM;
    SearchAlbumInfoViewModel *m_albumInfoVM;
};

#endif //SEARCHCONTROLLER_H
