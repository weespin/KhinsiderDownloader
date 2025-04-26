#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H

#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QFile>
#include <QThread>
#include <curl/curl.h>
#include <qtimer.h>

#include "WDownloadReply.h"


class DownloadWorker : public QObject
{
	Q_OBJECT

public:
	explicit DownloadWorker(int id, int maxConcurrentDownloads, QObject* parent = nullptr);
	~DownloadWorker() override;

public slots:
	void enqueueDownload(WDownloadReplyBase* downloadable, const QUrl& url);
	void stop();
	void start();

private slots:
	void checkMultiInfo();
	void handleDownloadFinished(WDownloadReplyBase* downloadable, CURLcode result);
	void checkCancellations();
	void checkQueue();

	void prepareForShutdown() { m_gracefulStopping = true; }
signals:
	void stopped();

public:
	int activeDownloadCount() const;

	void setMaxConcurrentDownloads(int nMaxDownloads);
	bool IsGracefulStopping() { return m_gracefulStopping; }
	bool IsRunning() { return m_running; }
	void scheduleRetry(WDownloadReplyBase* downloadable);
	void setRetryInterval(int milliseconds)
	{
		m_retryInvetrval = milliseconds;
	}

	int retryInterval() const
	{
		return m_retryInvetrval;
	}
private:
	int m_activeMultiInfoTimerInterval = 50;
	int m_sleepingMultiInfoTimerInterval = 1000;
	int m_queueTimerInterval = 1000;
	int m_id;
	int m_maxConcurrentDownloads;
	bool m_running;
	int m_retryInvetrval = 5000;
	//it means that we don't accept new orders in queue and will self destroy after finishing download
	bool m_gracefulStopping = false;

	QQueue<WDownloadReplyBase*> m_queue;
	QMutex m_queueMutex;
	QWaitCondition m_queueCondition;
	QTimer* m_multiTimer;
	QTimer* m_queueTimer;
	QMutex m_maxConCurrentDownloadsMutex;
	CURLM* m_multiHandle;
	QHash<CURL*, WDownloadReplyBase*> m_activeDownloads;

	void startDownload(WDownloadReplyBase* request);
	static size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata);



	static int progressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal,
	                            curl_off_t ulnow);
	QNetworkReply::NetworkError mapCurlErrorToNetworkError(CURLcode curlError);
};
#endif
