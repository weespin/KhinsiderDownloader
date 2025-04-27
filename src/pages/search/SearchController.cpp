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
    m_albumInfoVM = new SearchAlbumInfoViewModel(this);
    m_searchResultVM = new SearchResultModel(this);

    connect(m_searchResultVM, &SearchResultModel::albumSelected,
            this, &SearchController::fetchFullAlbumData);
    connect(m_searchResultVM, &SearchResultModel::albumSelected,
            m_albumInfoVM, &SearchAlbumInfoViewModel::setCurrentAlbum);
    connect(this, &SearchController::searchResultsReceived,
            m_searchResultVM, &SearchResultModel::onSearchResultsReceived);
    connect(m_searchResultVM, &SearchResultModel::performSearch,
            this, &SearchController::doSearch);
    connect(this, &SearchController::searchStarted,
            m_searchResultVM, &SearchResultModel::searchStarted);
    connect(this, &SearchController::onFullFetched,
            m_albumInfoVM, &SearchAlbumInfoViewModel::onFullFetch);
    connect(m_albumInfoVM, &SearchAlbumInfoViewModel::downloadRequested, this, &SearchController::onDownloadAddRequest);
    connect(m_searchResultVM, &SearchResultModel::requestAddAlbums, this, &SearchController::onDownloadAllAddRequest);
}

void SearchController::doSearch(const QString &query) {
    emit searchStarted();
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
        htmlDocPtr doc = htmlReadMemory(htmlStr.c_str(), htmlStr.size(), nullptr, nullptr,
                                        HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
        QVector<QSharedPointer<Album> > res;
        if (!doc) {
            qWarning() << "HTML read failed";
        }
        KhinsiderParser::ParseSearchResults(doc, res);
        emit searchResultsReceived(res);
        xmlFreeDoc(doc);
    });
}

//TODO: Duplicated code! We have this already in DownloadController!
void SearchController::fetchFullAlbumData(QSharedPointer<Album> album) {
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
        htmlDocPtr doc = htmlReadMemory(htmlStr.c_str(), htmlStr.size(), nullptr, nullptr,
                                        HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

        if (!doc) {
            qWarning() << "HTML read failed";
        }
        KhinsiderParser::ParseAlbumFullData(doc, album);
        ImagePrecache::instance()->precache(album->albumImage());
        emit onFullFetched();
        xmlFreeDoc(doc);
    });
}
