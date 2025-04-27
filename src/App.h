#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "LogController.h"
#include "pages/about/aboutcontroller.h"
#include "pages/search/SearchController.h"
#include "pages/settings/SettingsController.h"
#include "pages/download/DownloaderController.h"

class App : public QObject {
    Q_OBJECT

    Q_PROPERTY(SearchController* searchController READ searchController CONSTANT)
    Q_PROPERTY(SettingsController* settingsController READ settingsController CONSTANT)
    Q_PROPERTY(DownloaderController* downloaderController READ downloaderController CONSTANT)
    Q_PROPERTY(AboutController* aboutController READ aboutController CONSTANT)
    Q_PROPERTY(LogController* logController READ logController)
    Q_PROPERTY(Settings* settings READ settings)

public:
    explicit App(QObject *parent = nullptr)
        : QObject(parent) {
        m_settings = new Settings(this);
        m_searchController = new SearchController(this);
        m_settingsController = new SettingsController(this, m_settings);
        m_downloadController = new DownloaderController(this, m_settings);
        m_aboutController = new AboutController(this);

        setupConnections();

        LogController::instance().setSettings(m_settings);
        qSetMessagePattern("%{type} %{message}");
        qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg) {
         LogController::instance().messageHandler(type, context, msg);
        });
    }

    [[nodiscard]] SearchController *searchController() const { return m_searchController; }
    [[nodiscard]] SettingsController *settingsController() const { return m_settingsController; }
    [[nodiscard]] DownloaderController *downloaderController() const { return m_downloadController; }
    [[nodiscard]] AboutController *aboutController() const { return m_aboutController; }
    static LogController *logController() { return &LogController::instance(); }
    Settings *settings() const { return m_settings; }

private:
    void setupConnections() {
        connect(m_searchController, &SearchController::onDownloadAddRequest,
                m_downloadController, &DownloaderController::onDownloadListAdd);

        connect(m_settings, &Settings::maxConcurrentDownloadsPerThreadChanged,
                m_downloadController, &DownloaderController::onMaxConcurrentOperationsChanged);

        connect(m_settings, &Settings::downloadThreadsChanged,
                m_downloadController, &DownloaderController::onThreadsNumberChanged);

        connect(m_searchController, &SearchController::onDownloadAllAddRequest,
                m_downloadController, &DownloaderController::requestAddAlbums);
    }

private:
    SettingsController *m_settingsController;
    SearchController *m_searchController;
    DownloaderController *m_downloadController;
    AboutController *m_aboutController;
    Settings *m_settings;
};

#endif // APPCONTROLLER_H
