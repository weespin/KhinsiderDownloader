#ifndef SEARCHALBUMINFOVIEWMODEL_H
#define SEARCHALBUMINFOVIEWMODEL_H
#include <QObject>
#include "SearchResultModel.h"

class SearchAlbumInfoViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(Album* currentAlbum READ currentAlbum NOTIFY currentAlbumChanged)
    Q_PROPERTY(QStringList albumImages READ albumImages NOTIFY currentAlbumChanged)
    Q_PROPERTY(QVariantList filteredAlbumFields READ filteredAlbumFields NOTIFY currentAlbumChanged)
    Q_PROPERTY(QVariantList formats READ formats NOTIFY currentAlbumChanged)
public:
    explicit SearchAlbumInfoViewModel(QObject *parent = nullptr) : QObject(parent) {
    }

    QVariantList formats() const
    {
        QVariantList list;
        if (!m_currentAlbum)
            return list;
        for (const QString& format : m_currentAlbum->formats()) {
            list.append(format);
        }

        return list;
    }

    QVariantList filteredAlbumFields() const {
        QVariantList list;
        if (!m_currentAlbum)
            return list;

        auto add = [&](const QString &label, const QString &value)
        {
            if (!value.trimmed().isEmpty()) {
                QVariantMap item;
                item["label"] = label;
                item["value"] = value;
                list.append(item);
            }
        };

        add("Name", m_currentAlbum->name());
        add("Platforms", m_currentAlbum->platforms().join(", "));
        add("Type", m_currentAlbum->type());
        add("Year", m_currentAlbum->year());
        add("Publisher", m_currentAlbum->publisher());
        return list;
    }

    QStringList albumImages() const {
        if (m_currentAlbum) {
            return QStringList(m_currentAlbum->albumImage().begin(), m_currentAlbum->albumImage().end());
        }
        return QStringList() << "qrc:/icons/albumplaceholder.jpg"; // Default placeholder
    }

    Album *currentAlbum() const { return m_currentAlbum; }

signals:
    void currentAlbumChanged();
    void downloadRequested(QSharedPointer<Album> album, DownloadQuality quality);

public slots:
    void setCurrentAlbum(QSharedPointer<Album> album) {
        m_currentAlbumPtr = album;
        // Expose the raw pointer to QML
        m_currentAlbum = album.data();
        emit currentAlbumChanged();
    }

    void onFullFetch() {
        emit currentAlbumChanged();
    }
    void requestDownload(const QString& index)
    {
        DownloadQuality quality = DownloadQuality::UNKNOWN;
        if (index == "MP3") {
            quality = DownloadQuality::MP3;
        }
        else {
            quality = DownloadQuality::BEST;
        }
        emit downloadRequested(m_currentAlbumPtr, quality );
    }

private:
    Album *m_currentAlbum = nullptr; //TODO: well...
    QSharedPointer<Album> m_currentAlbumPtr;
};
#endif //SEARCHALBUMINFOVIEWMODEL_H
