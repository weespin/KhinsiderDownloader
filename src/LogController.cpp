#include "LogController.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QDesktopServices>

LogController &LogController::instance() {
    static LogController instance;
    return instance;
}

LogController::LogController(QObject *parent)
    : QObject(parent) {
    const QString logDirectory = logDir();
    QDir().mkpath(logDirectory);

    m_logFile.setFileName(logDirectory + "app.log");
    m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    m_stream.setDevice(&m_logFile);
}

LogController::~LogController() {
    m_logFile.close();
}

void LogController::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QString level;
    switch (type) {
        case QtDebugMsg: level = "DEBUG";
            break;
        case QtInfoMsg: level = "INFO";
            break;
        case QtWarningMsg: level = "WARNING";
            break;
        case QtCriticalMsg: level = "CRITICAL";
            break;
        case QtFatalMsg: level = "FATAL";
            break;
    }

    QString logMsg = QString("[%1] %2: %3 (%4:%5, %6)\n")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
            .arg(level)
            .arg(msg)
            .arg(context.file)
            .arg(context.line)
            .arg(context.function);

    if (m_settings && !m_settings->suppressLogs() && m_logFile.isOpen()) {
        m_stream << logMsg << Qt::flush;
    }
#ifdef QT_DEBUG
    fprintf(stderr, "%s", logMsg.toLocal8Bit().constData());
#endif
}

QString LogController::logPath() const {
    return m_logFile.fileName();
}

QString LogController::logDir() const {
#ifdef Q_OS_WIN
    QString path = QCoreApplication::applicationDirPath() + "/logs/";
#else
    QString path = QDir::homePath() + "/." + QCoreApplication::applicationName() + "/logs/";
#endif
    return path;
}
