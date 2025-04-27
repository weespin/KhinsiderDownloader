#ifndef IMAGEPRECACHE_H
#define IMAGEPRECACHE_H


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class ImagePrecache : public QObject {
    Q_OBJECT

public:
    static ImagePrecache *instance();

    explicit ImagePrecache(QNetworkAccessManager *manager, QObject *parent = nullptr);

    Q_INVOKABLE void precache(const QSet<QString> &urls);

private:
    QNetworkAccessManager *m_manager;
    static ImagePrecache *s_instance;
};


#endif //IMAGEPRECACHE_H
