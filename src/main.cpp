
#include "main.h"

#include "maincontroller.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QNetworkDiskCache>
#include <QStandardPaths>
#include <QQmlNetworkAccessManagerFactory>
#include "curl/curl.h"
#include "ImagePrecache.h"

class NetworkManagerFactory : public QQmlNetworkAccessManagerFactory {
public:
    QNetworkAccessManager* create(QObject* parent) override {
        QNetworkAccessManager* manager = new QNetworkAccessManager(parent);
        QNetworkDiskCache* diskCache = new QNetworkDiskCache(manager);
        diskCache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
        manager->setCache(diskCache);
        return manager;
    }
};

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString level;
    switch (type) {
    case QtDebugMsg:    level = "DEBUG"; break;
    case QtInfoMsg:     level = "INFO"; break;
    case QtWarningMsg:  level = "WARNING"; break;
    case QtCriticalMsg: level = "CRITICAL"; break;
    case QtFatalMsg:    level = "FATAL"; break;
    }

    QString logMsg = QString("[%1] %2: %3 (%4:%5, %6)\n")
                         .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                         .arg(level)
                         .arg(msg)
                         .arg(context.file)
                         .arg(context.line)
                         .arg(context.function);

    QString logPath;

#ifdef Q_OS_WIN
    logPath = QCoreApplication::applicationDirPath() + "/logs/";
#else
    logPath = QDir::homePath() + "/." + QCoreApplication::applicationName() + "/logs/";
#endif

    QDir().mkpath(logPath);
    logPath += "app.log";

    QFile logFile(logPath);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&logFile);
        stream << logMsg;
        logFile.close();
    }

    fprintf(stderr, "%s", logMsg.toLocal8Bit().constData());
}


int main(int argc, char *argv[])
{
    curl_global_init(CURL_GLOBAL_ALL);
    NetworkManagerFactory* networkManagerFactory = new NetworkManagerFactory();
    qSetMessagePattern("%{type} %{message}");
    qInstallMessageHandler(messageHandler);

    QCoreApplication::setOrganizationName("Weespin");
    QCoreApplication::setApplicationName("KhinsiderDownloader");

    QGuiApplication app(argc, argv);
    qmlRegisterType<Settings>("App.Models", 1, 0, "Settings");
    qmlRegisterType<Album>("App.Models", 1, 0, "Album");
    qmlRegisterType<SearchAlbumInfoViewModel>("App.ViewModels", 1, 0, "AlbumInfoViewModel");
    qmlRegisterType<SettingsViewModel>("App.ViewModels", 1, 0, "SettingsViewModel");
    qmlRegisterType<DownloaderModel>("App.Models", 1, 0, "DownloaderModel");


    AppController* viewModel = new AppController();

    //Remove AppSingleton completely!
    AppSingleton::instance().SetAppController(viewModel);
    QQmlApplicationEngine engine;
    QNetworkAccessManager* globalManager = networkManagerFactory->create(&engine);
    engine.setNetworkAccessManagerFactory(networkManagerFactory);
    //Move it somewhere, wtf it's doing here
    auto* precache = new ImagePrecache(globalManager, &engine);
    engine.rootContext()->setContextProperty("appController", viewModel);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("KhinsiderQT", "Main");
    int result = app.exec();

    // Clean up curl
    curl_global_cleanup();

    return result;
}
