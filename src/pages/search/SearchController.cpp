#include "SearchController.h"
#include <QNetworkReply>
#include <QQmlApplicationEngine>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include "ImagePrecache.h"
#include "parser/KhinsiderParser.h"

#define SEARCH_URL "https://downloads.khinsider.com/search?search="
#include "parser/Album.h"

SearchController::SearchController(QObject *parent) {
    // Own manager (single worker) so lookups never queue behind a long download list
    m_fetchManager = new AsyncDownloadManager(this);
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
    auto *reply = new WDownloadReplyMemory(nullptr);

    connect(reply, &WDownloadReplyBase::downloadFinished, this, [this, reply](QNetworkReply::NetworkError error) {
        if (error != QNetworkReply::NoError) {
            qWarning() << "Search request failed, error:" << error;
            reply->deleteLater();
            return;
        }

        const QByteArray html = reply->getData();
        reply->deleteLater();

        htmlDocPtr doc = htmlReadMemory(html.constData(), html.size(), nullptr, nullptr,
                                        HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
        QVector<QSharedPointer<Album> > res;
        if (!doc)
        {
            qWarning() << "HTML read failed";
        }

        KhinsiderParser::ParseSearchResults(doc, res);
        emit searchResultsReceived(res);
        xmlFreeDoc(doc);
    }, Qt::QueuedConnection);
    m_fetchManager->enqueueDownload(reply, url);
}

//TODO: Duplicated code! We have this already in DownloadController!
void SearchController::fetchFullAlbumData(QSharedPointer<Album> album) {
    if (album->isInfoParsed()) {
        return;
    }
    QUrl url(album->albumLink());
    auto *reply = new WDownloadReplyMemory(nullptr);

    connect(reply, &WDownloadReplyBase::downloadFinished, this, [this, reply, album](QNetworkReply::NetworkError error) {
        if (error != QNetworkReply::NoError) {
            qWarning() << "Album info request failed, error:" << error;
            album->setHasErrors(true);
            reply->deleteLater();
            return;
        }

        const QByteArray html = reply->getData();
        reply->deleteLater();

        htmlDocPtr doc = htmlReadMemory(html.constData(), html.size(), nullptr, nullptr,
                                        HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

        if (!doc)
        {
            album->setHasErrors(true);
            xmlFreeDoc(doc);
            qWarning() << "HTML read failed";
            return;
        }
        if(!KhinsiderParser::ParseAlbumFullData(doc, album))
        {
            album->setHasErrors(true);
            xmlFreeDoc(doc);
        }
        ImagePrecache::instance()->precache(album->albumImage());
        emit onFullFetched();
        xmlFreeDoc(doc);
    }, Qt::QueuedConnection);
    m_fetchManager->enqueueDownload(reply, url);
}
