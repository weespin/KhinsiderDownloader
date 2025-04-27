#include "App.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QNetworkDiskCache>
#include <QStandardPaths>
#include <QQmlNetworkAccessManagerFactory>
#include "curl/curl.h"
#include "ImagePrecache.h"
#include "LogController.h"

class NetworkManagerFactory : public QQmlNetworkAccessManagerFactory {
public:
    QNetworkAccessManager *create(QObject *parent) override {
        QNetworkAccessManager *manager = new QNetworkAccessManager(parent);
        QNetworkDiskCache *diskCache = new QNetworkDiskCache(manager);
        diskCache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
        manager->setCache(diskCache);
        return manager;
    }
};

int main(int argc, char *argv[]) {
    curl_global_init(CURL_GLOBAL_ALL);
    NetworkManagerFactory *networkManagerFactory = new NetworkManagerFactory();
    QCoreApplication::setOrganizationName("Weespin");
    QCoreApplication::setApplicationName("KhinsiderDownloader");

    QGuiApplication app(argc, argv);
    qmlRegisterType<Settings>("App.Models", 1, 0, "Settings");
    qmlRegisterType<Album>("App.Models", 1, 0, "Album");
    qmlRegisterType<SearchAlbumInfoViewModel>("App.ViewModels", 1, 0, "AlbumInfoViewModel");
    qmlRegisterType<SettingsViewModel>("App.ViewModels", 1, 0, "SettingsViewModel");
    qmlRegisterType<DownloaderModel>("App.Models", 1, 0, "DownloaderModel");


    App *kApp = new App();

    QQmlApplicationEngine engine;
    QNetworkAccessManager *globalManager = networkManagerFactory->create(&engine);
    engine.setNetworkAccessManagerFactory(networkManagerFactory);

    //Move it somewhere, wtf it's doing here
    auto *precache = new ImagePrecache(globalManager, &engine);

    engine.rootContext()->setContextProperty("app", kApp);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("KhinsiderQT", "Main");
    int result = app.exec();

    // Clean up curl???
    curl_global_cleanup();

    return result;
}
