#ifndef ABOUTCONTROLLER_H
#define ABOUTCONTROLLER_H
#include <QObject>

class AboutController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString appVersion READ appVersion NOTIFY appVersionChanged)

public:
    explicit AboutController(QObject *parent = nullptr);

    QString appVersion() const;

signals:
    void appVersionChanged();

private:
    QString m_version;
};
#endif // ABOUTCONTROLLER_H
