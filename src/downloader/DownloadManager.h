#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QVector>
#include <QThread>
#include "DownloadWorker.h"

class AsyncDownloadManager : public QObject {
    Q_OBJECT

public:
    explicit AsyncDownloadManager(QObject *parent = nullptr);

    ~AsyncDownloadManager();

    void setDownloadLimitPerThread(int limit);

    void setThreadCount(int count);

    void reduceWorkers();

    void addWorkers();

    void enqueueDownload(WDownloadReplyBase *downloadable, const QUrl &url);

    void cancelDownload(WDownloadReplyBase *downloadable);

private:
    DownloadWorker *getLeastBusyWorker();

private:
    int m_limitPerThread;
    int m_threadCount;

    QVector<QThread *> m_threads;
    QVector<DownloadWorker *> m_workers;

    int m_currentWorkerIndex;

    void initWorkers();

    void cleanupWorkers();

    QMutex m_workersMutex;
};
#endif
