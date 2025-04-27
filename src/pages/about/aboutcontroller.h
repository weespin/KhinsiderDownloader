#ifndef ABOUTCONTROLLER_H
#define ABOUTCONTROLLER_H
#include <QNetworkReply>
#include <QObject>

class AboutController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString appVersion READ appVersion NOTIFY appVersionChanged)

public:
    explicit AboutController(QObject *parent = nullptr);

    void checkForUpdates();

signals:
    void foundNewUpdate(const QString &version);

public slots:
    void onUpdateCheckFinished(QNetworkReply *reply);


    QString appVersion() const;

signals:
    void appVersionChanged();

private:
    bool isNewVersion(const QString &latestVersion, const QString &currentVersion);

private:
    QString m_version;
};
#endif // ABOUTCONTROLLER_H
