#include "SearchController.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QQmlApplicationEngine>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include "ImagePrecache.h"
#include "parser/KhinsiderParser.h"

#define SEARCH_URL "https://downloads.khinsider.com/search?search="
#include "parser/Album.h"

SearchController::SearchController(QObject *parent) {
    networkManager = new QNetworkAccessManager(this);
}

void SearchController::doSearch(const QString &query) {
    QUrl url(SEARCH_URL + QUrl::toPercentEncoding(query) + "&albumListSize=compact");
    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [this,reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Request failed:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QString html = reply->readAll();
        reply->deleteLater();

        std::string htmlStr = html.toStdString();
        htmlDocPtr doc = htmlReadMemory(htmlStr.c_str(), htmlStr.size(), nullptr, nullptr, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
        QVector<QSharedPointer<Album>> res;
        if (!doc)
        {
           qWarning() << "HTML read failed";
        }
        KhinsiderParser::ParseSearchResults(doc,res);
        emit searchResultsReceived(res);
        xmlFreeDoc(doc);
    });
}

//TODO: Duplicated code! We have this already in DownloadController!
void SearchController::fetchFullAlbumData(QSharedPointer<Album> album)
{
    if (album->isInfoParsed()) {
        return;
    }
    QUrl url(album->albumLink());
    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [this,reply,album]() {
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Request failed:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QString html = reply->readAll();
        reply->deleteLater();

        std::string htmlStr = html.toStdString();
        htmlDocPtr doc = htmlReadMemory(htmlStr.c_str(), htmlStr.size(), nullptr, nullptr, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

        if (!doc)
        {
           qWarning() << "HTML read failed";
        }
        KhinsiderParser::ParseAlbumFullData(doc,album);
        ImagePrecache::instance()->precache(album->albumImage());
        emit onFullFetched();
        xmlFreeDoc(doc);
    });
}
