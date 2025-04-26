#include "ImagePrecache.h"

ImagePrecache* ImagePrecache::s_instance = nullptr;

ImagePrecache::ImagePrecache(QNetworkAccessManager* manager, QObject* parent)
    : QObject(parent), m_manager(manager) {
    s_instance = this;
}

ImagePrecache* ImagePrecache::instance() {
    return s_instance;
}

void ImagePrecache::precache(const QSet<QString>& urls)
{
    for (const QString& urlStr : urls)
        {
        QUrl url(urlStr);
        QNetworkRequest request(url);
        QNetworkReply* reply = m_manager->get(request);
        connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);
    }
}

