#include "DownloadWorker.h"
#include <QDebug>
#include <QDateTime>
#include "QTimer"
#include "QUrl"
#include <QtNetwork/QNetworkReply>

DownloadWorker::DownloadWorker(int id, int maxConcurrentDownloads, QObject *parent)
	: QObject(parent), m_id(id), m_maxConcurrentDownloads(maxConcurrentDownloads), m_running(true) {
	m_multiHandle = curl_multi_init();
	static_assert(std::is_same<decltype(m_activeDownloads)::key_type, CURL *>::value,
	              "m_activeDownloads key type must be CURL*");
	static_assert(std::is_same<decltype(m_activeDownloads)::mapped_type, WDownloadReplyBase *>::value,
	              "m_activeDownloads value type must be WDownloadReplyBase*");
}

DownloadWorker::~DownloadWorker() {
	stop();

	if (m_multiHandle) {
		curl_multi_cleanup(m_multiHandle);
		m_multiHandle = nullptr;
	}

	delete m_multiTimer;
	delete m_queueTimer;
}

void DownloadWorker::enqueueDownload(WDownloadReplyBase *downloadable, const QUrl &url) {
	qDebug() << "Worker" << m_id << "- Queuing download from:" << url.toString();
	QMutexLocker locker(&m_queueMutex);
	downloadable->setURL(url);
	m_queue.enqueue(downloadable);
	m_queueCondition.wakeOne();

	//kick queue :)
	QMetaObject::invokeMethod(this, "checkQueue", Qt::QueuedConnection);
}

void DownloadWorker::stop() {
	m_running = false;
	m_queueCondition.wakeAll();

	for (auto it = m_activeDownloads.begin(); it != m_activeDownloads.end(); ++it) {
		curl_multi_remove_handle(m_multiHandle, it.key());
		curl_easy_cleanup(it.key());
	}
	m_activeDownloads.clear();

	emit stopped();
}

void DownloadWorker::start() {
	m_multiTimer = new QTimer(this);
	connect(m_multiTimer, &QTimer::timeout, this, &DownloadWorker::checkMultiInfo);
	m_multiTimer->start(m_sleepingMultiInfoTimerInterval);

	m_queueTimer = new QTimer(this);
	connect(m_queueTimer, &QTimer::timeout, this, &DownloadWorker::checkQueue);
	m_queueTimer->start(m_queueTimerInterval);
}

void DownloadWorker::checkQueue() {
	if (!m_running) return; {
		QMutexLocker locker(&m_maxConCurrentDownloadsMutex);
		if (m_maxConcurrentDownloads != 0 && m_activeDownloads.size() >= m_maxConcurrentDownloads) return;
	}
	WDownloadReplyBase *request; {
		QMutexLocker locker(&m_queueMutex);


		while (!m_queue.isEmpty()) {
			request = m_queue.head();
			if (!request || !request->isCanceled()) {
				break;
			}

			emit request->downloadFinished(QNetworkReply::OperationCanceledError);
			m_queue.dequeue();
		}

		if (m_queue.isEmpty()) return;
		request = m_queue.dequeue();
	}

	if (request) {
		startDownload(request);
	}
}

int DownloadWorker::activeDownloadCount() const {
	return m_queue.size() + m_activeDownloads.size();
}

void DownloadWorker::setMaxConcurrentDownloads(int nMaxDownloads) {
	QMutexLocker locker(&m_maxConCurrentDownloadsMutex);
	m_maxConcurrentDownloads = nMaxDownloads;
}

void DownloadWorker::checkMultiInfo() {
	if (!m_running || !m_multiHandle) {
		return;
	}

	if (m_gracefulStopping && m_activeDownloads.empty()) {
		stop();
		return;
	}

	int stillRunning = 0;
	CURLMcode mc;
	do {
		mc = curl_multi_perform(m_multiHandle, &stillRunning);
	} while (mc == CURLM_CALL_MULTI_PERFORM);

	if (mc != CURLM_OK) {
		qWarning() << "curl_multi_perform failed with error:" << curl_multi_strerror(mc);
		return;
	}

	CURLMsg *msg;
	int msgsLeft;

	while ((msg = curl_multi_info_read(m_multiHandle, &msgsLeft))) {
		if (msg->msg == CURLMSG_DONE) {
			CURL *easyHandle = msg->easy_handle;
			CURLcode result = msg->data.result;

			if (m_activeDownloads.contains(easyHandle)) {
				WDownloadReplyBase *downloadable = m_activeDownloads.value(easyHandle);

				curl_multi_remove_handle(m_multiHandle, easyHandle);
				curl_easy_cleanup(easyHandle);

				handleDownloadFinished(downloadable, result);
				//queue it's time to check itself!
				QMetaObject::invokeMethod(this, "checkQueue", Qt::QueuedConnection);
			}
		}
	}

	if (stillRunning > 0) {
		//We still have to process some data, use working interval
		m_multiTimer->start(m_activeMultiInfoTimerInterval);
	} else if (m_multiTimer->interval() != m_sleepingMultiInfoTimerInterval) {
		//We're empty, it's time to relax
		m_multiTimer->start(m_sleepingMultiInfoTimerInterval);
	}
	checkCancellations();
}

void DownloadWorker::startDownload(WDownloadReplyBase *request) {
	if (!request || request->url().isEmpty()) {
		emit request->downloadFinished(QNetworkReply::ContentNotFoundError);
	}

	if (request->isCanceled()) {
		emit request->downloadFinished(QNetworkReply::OperationCanceledError);
		return;
	}

	qDebug() << "Worker" << m_id << "- Starting download from:" << request->url().toString();

	CURL *easyHandle = curl_easy_init();
	if (!easyHandle) {
		emit request->downloadFinished(QNetworkReply::UnknownNetworkError);
		return;
	}

	// Create download data structure
	request->setLastBytesRecieved(0);
	request->setLastSpeedUpdate(QDateTime::currentMSecsSinceEpoch());

	// Set options
	curl_easy_setopt(easyHandle, CURLOPT_URL, request->url().toEncoded().constData());
	curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, request);
	curl_easy_setopt(easyHandle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(easyHandle, CURLOPT_PRIVATE, request);
	curl_easy_setopt(easyHandle, CURLOPT_TIMEOUT, 60L);
	curl_easy_setopt(easyHandle, CURLOPT_BUFFERSIZE, CURL_MAX_READ_SIZE); // 2MB buffer
	curl_easy_setopt(easyHandle, CURLOPT_TCP_NODELAY, 1L);
	curl_easy_setopt(easyHandle, CURLOPT_TCP_KEEPALIVE, 1L);
	curl_easy_setopt(easyHandle, CURLOPT_TCP_FASTOPEN, 1L);
	curl_easy_setopt(easyHandle, CURLOPT_PIPEWAIT, 1L);
	curl_easy_setopt(easyHandle, CURLOPT_SSL_VERIFYPEER, 0L);
	// Set up progress callback
	curl_easy_setopt(easyHandle, CURLOPT_XFERINFOFUNCTION, progressCallback);
	curl_easy_setopt(easyHandle, CURLOPT_XFERINFODATA, request);
	curl_easy_setopt(easyHandle, CURLOPT_NOPROGRESS, 0L);

	// Add to multi handle
	CURLMcode mc = curl_multi_add_handle(m_multiHandle, easyHandle);
	if (mc != CURLM_OK) {
		emit request->downloadFinished(QNetworkReply::UnknownNetworkError);
		curl_easy_cleanup(easyHandle);
		return;
	}

	// Store mappings
	m_activeDownloads.insert(easyHandle, request);
	qDebug() << "Inserting... " << QThread::currentThreadId();
	//kick multitimer, it has some work
	QMetaObject::invokeMethod(this, "checkMultiInfo", Qt::QueuedConnection);
}

size_t DownloadWorker::writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	auto data = static_cast<WDownloadReplyBase *>(userdata);
	if (!data) {
		return 0;
	}

	size_t bytesToWrite = size * nmemb;
	data->appendData(ptr, bytesToWrite);

	return bytesToWrite;
}

void DownloadWorker::checkCancellations() {
	//bool bSomethingCanceled = false;
	//for (auto it = m_activeDownloads.begin(); it != m_activeDownloads.end(); ++it) {
	//	if (it.value()->isCanceled()) {
	//		curl_easy_setopt(it.key(), CURLOPT_TIMEOUT_MS, 1);
	//		bSomethingCanceled = true;
	//	}
	//}
	//

	//if (bSomethingCanceled) {
	//	QMetaObject::invokeMethod(this, "checkMultiInfo", Qt::QueuedConnection);
	//}
}

int DownloadWorker::progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal,
                                     curl_off_t ulnow) {
	Q_UNUSED(ultotal);
	Q_UNUSED(ulnow);

	auto data = static_cast<WDownloadReplyBase *>(clientp);
	if (!data) {
		return 1;
	}

	if (data->isCanceled()) {
		return 1;
	}

	emit data->downloadProgress(dlnow, dltotal);
	data->ensureContainerSize(dltotal);
	qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
	qint64 elapsed = currentTime - data->lastSpeedUpdate();

	if (elapsed >= 1000) {
		// update speed every second
		qint64 bytesReceived = dlnow - data->lastBytesReceived();
		qint64 bytesPerSecond = (bytesReceived * 1000) / elapsed;
		if (bytesPerSecond != 0) {
			emit data->downloadSpeed(bytesPerSecond);
		}
		data->setLastBytesRecieved(dlnow);
		data->setLastSpeedUpdate(currentTime);
	}

	return 0;
}

void DownloadWorker::handleDownloadFinished(WDownloadReplyBase *downloadable, CURLcode result) {
	if (!downloadable || !m_activeDownloads.values().contains(downloadable)) {
		return;
	}

	CURL *easyHandle = nullptr;
	for (auto it = m_activeDownloads.begin(); it != m_activeDownloads.end(); ++it) {
		if (it.value() == downloadable) {
			easyHandle = it.key();
			break;
		}
	}
	qDebug() << "Active Downloads" << m_activeDownloads.size();
	// Find both the handle and data

	if (!easyHandle) {
		return; // Should not happen
	}

	if (result == CURLE_OK) {
		qDebug() << "Worker" << m_id << "- Download successful from:" << downloadable->url().toString();

		// Done, emit finished
		emit downloadable->downloadFinished(QNetworkReply::NoError);
	} else if (downloadable->isCanceled()) {
		emit downloadable->downloadFinished(QNetworkReply::OperationCanceledError);
	} else {
		qDebug() << "Worker" << m_id << "- Download failed from:" << downloadable->url().toString()
				<< "with error:" << curl_easy_strerror(result);

		QNetworkReply::NetworkError networkError = mapCurlErrorToNetworkError(result);


		if (downloadable->getCurrentRetryCount() < downloadable->getMaxRetryCount()) {
			downloadable->incrementRetryCount();
			emit downloadable->downloadRetry();

			// Re-queue for download
			downloadable->setLastBytesRecieved(0);

			scheduleRetry(downloadable);
		} else {
			emit downloadable->downloadFinished(networkError);
		}
	}
	m_activeDownloads.remove(easyHandle);
}

void DownloadWorker::scheduleRetry(WDownloadReplyBase *downloadable) {
	if (!downloadable)
		return;

	QTimer *retryTimer = new QTimer(this);
	retryTimer->setSingleShot(true);
	connect(retryTimer, &QTimer::timeout, [this, downloadable, retryTimer]() {
		QMutexLocker locker(&m_queueMutex);
		m_queue.enqueue(downloadable);
		m_queueCondition.wakeOne();
		retryTimer->deleteLater();

		// Invoke checkQueue to process the retry immediately
		QMetaObject::invokeMethod(this, "checkQueue", Qt::QueuedConnection);
	});

	qDebug() << "Worker" << m_id << "- Scheduling retry for:" << downloadable->url().toString()
			<< "in" << m_retryInvetrval << "ms";

	retryTimer->start(m_retryInvetrval);
}

QNetworkReply::NetworkError DownloadWorker::mapCurlErrorToNetworkError(CURLcode curlError) {
	switch (curlError) {
		case CURLE_COULDNT_RESOLVE_HOST:
			return QNetworkReply::HostNotFoundError;
		case CURLE_COULDNT_CONNECT:
			return QNetworkReply::ConnectionRefusedError;
		case CURLE_OPERATION_TIMEDOUT:
			return QNetworkReply::TimeoutError;
		case CURLE_SSL_CONNECT_ERROR:
			return QNetworkReply::SslHandshakeFailedError;
		case CURLE_GOT_NOTHING:
		case CURLE_RECV_ERROR:
			return QNetworkReply::RemoteHostClosedError;
		case CURLE_ABORTED_BY_CALLBACK:
			return QNetworkReply::OperationCanceledError;
		default:
			return QNetworkReply::UnknownNetworkError;
	}
}
