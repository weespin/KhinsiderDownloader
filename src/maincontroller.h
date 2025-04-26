#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "pages/about/aboutcontroller.h"
#include "pages/search/SearchViewModel.h"
#include "pages/search/SearchAlbumInfoViewModel.h"
#include "pages/settings/SettingsController.h"
#include "pages/download/DownloaderController.h"
//TODO: Organize!
class AppController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(SearchViewModel* searchVM READ searchVM CONSTANT)
    Q_PROPERTY(SearchAlbumInfoViewModel* albumInfoVM READ albumInfoVM CONSTANT)
    Q_PROPERTY(SettingsViewModel* settingsVM READ settingsVM CONSTANT)
    Q_PROPERTY(DownloaderModel* downloaderModel READ downloaderModel CONSTANT)
    Q_PROPERTY(AboutController* aboutController READ aboutController CONSTANT)

public:
    explicit AppController(QObject *parent = nullptr)
        : QObject(parent)
    {
        m_searchVM = new SearchViewModel(this);
        m_albumInfoVM = new SearchAlbumInfoViewModel(this);
        m_searchController = new SearchController(this);
        m_settingsController = new SettingsController(this);
        m_downloadController = new DownloaderController(this,m_settingsController->settingsVM()->settings());
        m_aboutController = new AboutController(this);
        setupConnections();
    }
    DownloaderModel* downloaderModel() const {return m_downloadController->downloaderVM();}
    SearchViewModel* searchVM() const { return m_searchVM; }
    SearchAlbumInfoViewModel* albumInfoVM() const { return m_albumInfoVM; }
    SettingsViewModel* settingsVM() const { return m_settingsController->settingsVM(); }
    SettingsController* settingsController() const { return m_settingsController; }
    AboutController* aboutController() const {return m_aboutController;}

private:
    void setupConnections()
    {

        connect(m_searchVM, &SearchViewModel::albumSelected,
                m_searchController, &SearchController::fetchFullAlbumData);
        connect(m_searchVM, &SearchViewModel::albumSelected,
                m_albumInfoVM, &SearchAlbumInfoViewModel::setCurrentAlbum);
        connect(m_searchController, &SearchController::searchResultsReceived,
                       m_searchVM, &SearchViewModel::onSearchResultsReceived);
        connect(m_searchVM, &SearchViewModel::performSearch,
                m_searchController, &SearchController::doSearch);
        connect(m_searchController, &SearchController::onFullFetched,
                       m_albumInfoVM, &SearchAlbumInfoViewModel::onFullFetch);
        connect(m_searchController, &SearchController::onDownloadAddRequest,
                      m_downloadController, &DownloaderController::onDownloadListAdd);
        connect(m_albumInfoVM, &SearchAlbumInfoViewModel::downloadRequested,
                     m_downloadController, &DownloaderController::onDownloadListAdd);
        connect(m_settingsController->settingsVM()->settings(), &Settings::maxConcurrentDownloadsPerThreadChanged,
                    m_downloadController, &DownloaderController::onMaxConcurrentOperationsChanged);
        connect(m_settingsController->settingsVM()->settings(), &Settings::downloadThreadsChanged,
                   m_downloadController, &DownloaderController::onThreadsNumberChanged);
        connect(m_searchVM, &SearchViewModel::addAllAlbumsRequested,
                   m_searchVM->searchResult(), &SearchResultModel::addAllAlbumsRequested);
        connect(m_searchVM->searchResult(), &SearchResultModel::requestAddAlbums,m_downloadController, &DownloaderController::requestAddAlbums);
    }
private:
    // Member variables
    SearchViewModel* m_searchVM;
    SearchAlbumInfoViewModel* m_albumInfoVM;
    SettingsController* m_settingsController;
    SearchController *m_searchController;
    DownloaderController *m_downloadController;
    AboutController* m_aboutController;
};

#endif // APPCONTROLLER_H
