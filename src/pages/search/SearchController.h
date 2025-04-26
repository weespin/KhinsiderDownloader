#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QString>

#include "parser/KhinsiderParser.h"

class Album;

class SearchController : public QObject
{
    Q_OBJECT

public:
    explicit SearchController(QObject* parent = nullptr);
    void doSearch(const QString& text);
    void fetchFullAlbumData(QSharedPointer<Album> album);
 signals:
    void onDownloadAddRequest(QSharedPointer<Album> Album, DownloadQuality quality);
    void searchResultsReceived(const QVector<QSharedPointer<Album>> &result);
    void onFullFetched();
private:
    QNetworkAccessManager* networkManager;
};

#endif //SEARCHCONTROLLER_H
