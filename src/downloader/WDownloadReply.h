#ifndef WDOWNLOADREPLY_H
#define WDOWNLOADREPLY_H
#include <QDir>
#include <qfileinfo.h>
#include <QNetworkReply>
#include <QObject>

class WDownloadReplyBase : public QObject {
    Q_OBJECT

public:
    explicit WDownloadReplyBase(QObject *parent = nullptr) : QObject(parent),
                                                             maxRetryCount(3), currentRetryCount(0) {
    }

    virtual ~WDownloadReplyBase() {
    }

    void setMaxRetryCount(int count) { maxRetryCount = count; }
    int getMaxRetryCount() const { return maxRetryCount; }
    int getCurrentRetryCount() const { return currentRetryCount; }
    void incrementRetryCount() { currentRetryCount++; }
    void resetRetryCount() { currentRetryCount = 0; }

    bool isCanceled() const { return m_canceled; }

    QUrl url() const { return m_url; }
    void setURL(const QUrl &url) { m_url = url; }

    qint64 lastBytesReceived() const { return m_lastBytesReceived; }
    qint64 lastSpeedUpdate() const { return m_lastSpeedUpdate; }
    void setLastBytesRecieved(qint64 value) { m_lastBytesReceived = value; }
    void setLastSpeedUpdate(qint64 value) { m_lastSpeedUpdate = value; }

    virtual void appendData(const char *pData, size_t size) = 0;

    virtual void ensureContainerSize(qint64 minsize) = 0;

    virtual qint64 getDataSize() const = 0;

public slots:
    void cancel() { m_canceled = true; }
signals:
    void downloadFinished(QNetworkReply::NetworkError error = QNetworkReply::NoError);

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void downloadSpeed(qint64 bytesPerSecond);

    void downloadRetry();

protected:
    int maxRetryCount;
    int currentRetryCount;
    bool m_canceled = false;
    QUrl m_url;
    qint64 m_lastBytesReceived = 0;
    qint64 m_lastSpeedUpdate = 0;
};

class WDownloadReplyMemory : public WDownloadReplyBase {
public:
    explicit WDownloadReplyMemory(QObject *parent = nullptr) : WDownloadReplyBase(parent) {
    }

    virtual ~WDownloadReplyMemory() {
    }

    void appendData(const char *pData, size_t size) override {
        m_data.append(pData, size);
    }

    void ensureContainerSize(qint64 minsize) override {
        if (m_data.capacity() < minsize && minsize > 0) {
            m_data.reserve(minsize);
        }
    }

    qint64 getDataSize() const override {
        return m_data.size();
    }

    // Get the data
    const QByteArray &getData() const { return m_data; }
    QByteArray &getData() { return m_data; }

private:
    QByteArray m_data;
};

class WDownloadReplyFile : public WDownloadReplyBase {
public:
    explicit WDownloadReplyFile(QObject *parent = nullptr) : WDownloadReplyBase(parent) {
    }

    virtual ~WDownloadReplyFile() {
        if (m_file.isOpen()) {
            m_file.close();
        }

        if (isCanceled()) {
            QFile::remove(m_file.fileName());
        }
    }

    bool setFilePath(const QString &filePath) {
        if (m_file.isOpen()) {
            m_file.close();
        }

        m_file.setFileName(filePath);

        QDir dir = QFileInfo(filePath).dir();
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                qWarning() << "Failed to create directory in " << dir.path();
                cancel();
            }
        }

        return m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    }

    void appendData(const char *pData, size_t size) override {
        if (m_file.isOpen()) {
            m_file.write(pData, size);
        }
    }

    void ensureContainerSize(qint64 minsize) override {
        Q_UNUSED(minsize);
    }

    qint64 getDataSize() const override {
        return m_file.size();
    }

    QFile &getFile() { return m_file; }
    const QFile &getFile() const { return m_file; }

    QString filePath() const { return m_file.fileName(); }

private:
    QFile m_file;
};
#endif //WDOWNLOADREPLY_H
