#include "DownloadManager.h"
#include <QDir>
#include <QEventLoop>

AsyncDownloadManager::AsyncDownloadManager(QObject *parent)
	: QObject(parent), m_limitPerThread(0), m_threadCount(1), m_currentWorkerIndex(0) {
	addWorkers();
}

AsyncDownloadManager::~AsyncDownloadManager() {
	cleanupWorkers();
}

void AsyncDownloadManager::setDownloadLimitPerThread(int limit) {
	QMutexLocker locker(&m_workersMutex);

	if (limit > 0 && limit != m_limitPerThread) {
		m_limitPerThread = limit;

		for (auto &worker: m_workers) {
			worker->setMaxConcurrentDownloads(limit);
		}
	}
}

void AsyncDownloadManager::setThreadCount(int count) {
	if (count > m_threadCount) {
		m_threadCount = count;
		addWorkers();
	} else if (count < m_threadCount) {
		m_threadCount = count;
		reduceWorkers();
	}
}

void AsyncDownloadManager::reduceWorkers() {
	QMutexLocker locker(&m_workersMutex);
	int workersToRemove = m_workers.size() - m_threadCount;
	if (workersToRemove <= 0) return;

	std::sort(m_workers.begin(), m_workers.end(),
	          [](DownloadWorker *a, DownloadWorker *b) {
		          return a->activeDownloadCount() < b->activeDownloadCount();
	          });

	for (int i = 0; i < workersToRemove && i < m_workers.size(); ++i) {
		QMetaObject::invokeMethod(m_workers[i], "prepareForShutdown", Qt::QueuedConnection);
	}
}

void AsyncDownloadManager::addWorkers() {
	QMutexLocker locker(&m_workersMutex);
	int nNeededWorkers = m_threadCount - m_threads.size();
	if (nNeededWorkers <= 0) {
		return;
	}

	for (int i = 0; i < nNeededWorkers; ++i) {
		auto thread = new QThread();
		auto worker = new DownloadWorker(i, m_limitPerThread, nullptr);

		worker->moveToThread(thread);

		connect(thread, &QThread::started, worker, &DownloadWorker::start);
		connect(thread, &QThread::finished, worker, &QObject::deleteLater);
		connect(thread, &QThread::finished, thread, &QObject::deleteLater);
		connect(worker, &DownloadWorker::stopped, this, [this, worker, thread]() {
			QMutexLocker locker(&m_workersMutex);
			m_workers.removeOne(worker);
			m_threads.removeOne(thread);
			thread->quit();
		});

		m_threads.append(thread);
		m_workers.append(worker);

		thread->start();
	}
}

DownloadWorker *AsyncDownloadManager::getLeastBusyWorker() {
	std::vector<DownloadWorker *> activeWorkers;
	std::copy_if(m_workers.begin(), m_workers.end(), std::back_inserter(activeWorkers),
	             [](DownloadWorker *w) { return !w->IsGracefulStopping() && w->IsRunning(); });

	if (activeWorkers.empty()) return nullptr;

	const auto it = std::min_element(activeWorkers.begin(), activeWorkers.end(),
	                                 [](DownloadWorker *a, DownloadWorker *b) {
		                                 return a->activeDownloadCount() < b->activeDownloadCount();
	                                 });

	return *it;
}

void AsyncDownloadManager::enqueueDownload(WDownloadReplyBase *downloadable, const QUrl &url) {
	if (!downloadable || url.isEmpty()) {
		return;
	}

	QMutexLocker locker(&m_workersMutex);


	DownloadWorker *worker = getLeastBusyWorker();

	if (!worker) {
		qWarning() << "Failed to find an available worker";
		return;
	}

	QMetaObject::invokeMethod(worker, "enqueueDownload",
	                          Q_ARG(WDownloadReplyBase*, downloadable),
	                          Q_ARG(QUrl, url));
}

void AsyncDownloadManager::cleanupWorkers() {
	if (m_workers.isEmpty()) return;


	for (DownloadWorker *worker: m_workers) {
		QMetaObject::invokeMethod(worker, "stop", Qt::QueuedConnection);
	}


	QEventLoop loop;
	int workersStopped = 0;
	const int totalWorkers = m_workers.size();


	auto onStopped = [&]() {
		workersStopped++;
		if (workersStopped == totalWorkers) {
			loop.quit();
		}
	};


	for (DownloadWorker *worker: m_workers) {
		connect(worker, &DownloadWorker::stopped, &loop, onStopped);
	}

	loop.exec();

	for (QThread *thread: m_threads) {
		thread->quit();
		if (!thread->wait(2000)) {
			thread->terminate();
		}
		delete thread;
	}

	m_workers.clear();
	m_threads.clear();
	m_currentWorkerIndex = 0;
}
