#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QObject>
#include <QMessageLogContext>

#include "pages/settings/Settings.h"

class LogController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString logPath READ logPath CONSTANT)
    Q_PROPERTY(QString logDir READ logDir)

public:
    static LogController &instance();

    void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    void setSettings(Settings *settings) {
        m_settings = settings;
    }

    QString logPath() const;

    QString logDir() const;

private:
    LogController(QObject *parent = nullptr);

    ~LogController();

    Settings *m_settings = nullptr;
    QFile m_logFile;
    QTextStream m_stream;
};


#endif //LOGCONTROLLER_H
