#include "Settings.h"
#include <QDir>
#include "ImagePrecache.h"

void Settings::ParseFromConfig(QSettings &settings) {
    setDownloadPath(settings.value("downloadPath", QDir::currentPath() + "/Downloads/").toString());
    setDownloadThreads(settings.value("downloadThreads", "1").toInt());
    setMaxConcurrentDownloadsPerThread(settings.value("maxConcurrentDownloadsPerThread", "0").toInt());
    int val = settings.value("preferredAudioQuality", 0).toInt();
    val = qBound(0, val, static_cast<int>(DownloadQuality::BEST));
    setPreferredAudioQuality(static_cast<DownloadQuality>(val));
    setDownloadThreads(settings.value("downloadThreads", 1).toInt());
    setEnableLogging(settings.value("enableLogging", true).toBool());
    setDownloadArt(settings.value("downloadArt", false).toBool());
    setSkipDownloaded(settings.value("skipDownloaded", true).toBool());
}
