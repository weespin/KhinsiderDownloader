#pragma once
#include <libxml/HTMLparser.h>


#include <QObject>
#include <QString>
#include <QMap>
#include <qset.h>

#include "downloader/WDownloadReply.h"

enum DownloadQuality {
	MP3,
	BEST,
	UNKNOWN
};

class Album;
class Song;

class KhinsiderParser {
public:
	static bool ParseAlbumPage(htmlDocPtr Document, QVector<QSharedPointer<Album> > &Out);

	static bool ParseAlbumFullData(htmlDocPtr Document, QSharedPointer<Album> Out);

	static bool ParseSearchResults(htmlDocPtr Document, QVector<QSharedPointer<Album> > &Out);

	static bool ParsePlaylist(htmlDocPtr Document, QSharedPointer<Album> Out);

	static bool ParseDownloadLink(htmlDocPtr Document, QSharedPointer<Song> Out);
};

