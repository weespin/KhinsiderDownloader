#include "KhinsiderParser.h"

#include <iostream>
#include <QSharedPointer>

#include "Album.h"
#include "HTMLParserHelper.h"
#include <libxml/xpath.h>

// wellcome to hell
auto safeGetContent = [](xmlNodePtr node) -> QString
{
	if (!node) return QString();
	xmlChar* content = xmlNodeGetContent(node);
	if (!content) return QString();
	QString result = QString::fromUtf8(reinterpret_cast<const char*>(content));
	xmlFree(content);
	return result;
};


auto safeGetProp = [](xmlNodePtr node, const char* propName) -> QString
{
	if (!node) return QString();

	xmlChar* prop = xmlGetProp(node, reinterpret_cast<const xmlChar*>(propName));
	if (!prop) return QString();

	size_t propLen = xmlStrlen(prop);

	QString result = QString::fromUtf8(reinterpret_cast<const char*>(prop), static_cast<int>(propLen));
	int newsize = result.size();
	xmlFree(prop);

	return result;
};

bool KhinsiderParser::ParseAlbumPage(htmlDocPtr Document, QVector<QSharedPointer<Album>>& Out)
{
	QSharedPointer<Album> Album;
	bool bResult = ParseAlbumFullData(Document, Album);
	if (!bResult) {
		return false;
	}
	Out.push_back(Album);
	return true;
}
bool KhinsiderParser::ParseAlbumFullData(htmlDocPtr Document, QSharedPointer<Album> Out)
{

	//don't forget to clean contexts!
	const xmlXPathContextPtr PathContextPtr = xmlXPathNewContext(Document);

	auto cleanup = [&](bool success) {
		xmlXPathFreeContext(PathContextPtr);
		return success;
		};

	//Parse AlbumInfo
	const xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(
		reinterpret_cast<const xmlChar*>("//div[@id='pageContent']"), PathContextPtr);

	if (!xpathObj || !xpathObj->nodesetval || xpathObj->nodesetval->nodeNr < 1) {
		if (xpathObj) xmlXPathFreeObject(xpathObj);
		return cleanup(false);
	}

	const xmlNodePtr Page = xpathObj->nodesetval->nodeTab[0]->children;
	if (!Page) {
		if (xpathObj) xmlXPathFreeObject(xpathObj);
		return cleanup(false);
	}

	xmlXPathFreeObject(xpathObj);

	QSharedPointer<Album> newAlbum = Out;

	xmlNodePtr AlbumNamePtr = HTMLParserHelper::TraverseNext(Page, 3);
	if (AlbumNamePtr) {
		newAlbum->setName(safeGetContent(AlbumNamePtr));
	}


	const xmlNodePtr AlbumDescPtr = HTMLParserHelper::ToNextTag(AlbumNamePtr, "p");

	if (!AlbumDescPtr) {
		return cleanup(false);
	}
	//Platforms
	xmlNodePtr Start = AlbumDescPtr->children;

	if ((Start = HTMLParserHelper::SkipTo("Platforms:", AlbumDescPtr->children)))
	{
		for (xmlNode* node = Start; node; node = node->next)
		{
			if (node->name && xmlStrEqual(node->name, (const xmlChar*)"br"))
			{
				break;
			}
			if (node->type == XML_ELEMENT_NODE && node->name &&
				xmlStrEqual(node->name, (const xmlChar*)"a") && node->children)
			{
				newAlbum->addPlatform(safeGetContent(node->children));
			}
		}
	}
	if ((Start = HTMLParserHelper::SkipTo("Year:", AlbumDescPtr->children)))
	{
		Start = Start->next;
		if (Start && Start->type == XML_ELEMENT_NODE && Start->name &&
			xmlStrEqual(Start->name, (const xmlChar*)"b") && Start->children) {
			newAlbum->setYear(safeGetContent(Start->children));
		}
	}


	// Publisher
	if ((Start = HTMLParserHelper::SkipTo("Published by:", AlbumDescPtr->children)))
	{
		for (xmlNode* node = Start; node; node = node->next)
		{
			if (node->name && xmlStrEqual(node->name, (const xmlChar*)"br"))
			{
				Start = node;
				break;
			}
			if (node->type == XML_ELEMENT_NODE && node->name &&
				xmlStrEqual(node->name, (const xmlChar*)"a") && node->children)
			{
				newAlbum->setPublisher(safeGetContent(node->children));
				break;
			}
		}
	}

	//Publisher
	if ((Start = HTMLParserHelper::SkipTo("Album type:", AlbumDescPtr->children)))
	{
		Start = Start->next;
		if (Start && Start->type == XML_ELEMENT_NODE && Start->name &&
			xmlStrEqual(Start->name, (const xmlChar*)"b") && Start->children) {
			newAlbum->setType(safeGetContent(Start->children));
		}
	}
	xmlNodePtr ImageTable = HTMLParserHelper::ToNextTag(AlbumNamePtr, "table");
	int depth = 0;

	while (ImageTable && ImageTable->children && depth < 3)
	{
		ImageTable = ImageTable->children;
		++depth;
	}

	if (ImageTable)
	{
		ImageTable = ImageTable->next;
		newAlbum->setAlbumImage(QSet<QString>());
	}
	for (xmlNodePtr imageTableNode = ImageTable; imageTableNode; imageTableNode = imageTableNode->next)
	{
		QString className = safeGetProp(imageTableNode,"class");
		if (!imageTableNode->children || !imageTableNode->children->next || !className.contains("albumImage"))
		{
			continue;
		}
		xmlNodePtr trueImageNode = imageTableNode->children->next;
		newAlbum->addAlbumImage(safeGetProp(trueImageNode, "href"));
	}

	///////////
	//SONG LIST
	const xmlXPathObjectPtr songList = xmlXPathEvalExpression(
		reinterpret_cast<const xmlChar*>("//table[@id='songlist']"), PathContextPtr);

	if (!songList || !songList->nodesetval || songList->nodesetval->nodeNr < 1)
	{
		if (songList) xmlXPathFreeObject(songList);

		return cleanup(true);
	}
	//SONG HEADER
	const xmlNodePtr tableNode = songList->nodesetval->nodeTab[0];
	if (!tableNode || !tableNode->children)
	{
		xmlXPathFreeObject(songList);

		return cleanup(true);
	}

	if(!tableNode->children->next)
	{
		cleanup(false);
	}

	const xmlNodePtr SongListHeaderPtr = tableNode->children->next->children;
	if (!SongListHeaderPtr)
	{
		xmlXPathFreeObject(songList);

		return cleanup(true);
	}

	int nSongNameIndex = 0;
	xmlNodePtr SongNameHeaderPtr = nullptr;
	for (xmlNodePtr snamenode = SongListHeaderPtr; snamenode; snamenode = snamenode->next)
	{
		if (snamenode->children && snamenode->children->children && snamenode->children->children->type == XML_TEXT_NODE)
		{
			QString content = safeGetContent(snamenode->children->children);
			if (content.contains("Song Name"))
			{
				SongNameHeaderPtr = snamenode;
				break;
			}
		}

		nSongNameIndex++;
	}

	if (!SongNameHeaderPtr) {
		xmlXPathFreeObject(songList);

		return cleanup(true);
	}

	int nNodes = HTMLParserHelper::GetNextCount(SongNameHeaderPtr);
	int nFormats = (1 + (nNodes - 6)) / 2;

	for (int nFormat = 0; nFormat < nFormats; ++nFormat)
	{
		const xmlNodePtr SongFormatNode = HTMLParserHelper::TraverseNext(SongListHeaderPtr, nSongNameIndex + 2 + (nFormat * 2));
		if (SongFormatNode && SongFormatNode->children && SongFormatNode->children->children)
		{
			QString formatName = safeGetContent(SongFormatNode->children->children);
			if (formatName.size())
			{
				newAlbum->addFormat(formatName);
			}
		}
	}

	//SONG HEADER END
	const xmlNodePtr SongListPtr = HTMLParserHelper::TraverseNext(tableNode->children,3);

	if (!SongListPtr) {
		xmlXPathFreeObject(songList);

		return cleanup(true);
	}

	for (xmlNodePtr node = SongListPtr; node; node = node->next)
	{
		QString NodeId = safeGetProp(node, "id");
		if (NodeId.size() && NodeId.contains("songlist_footer")) {
			break;
		}

		QSharedPointer<Song> newSong = QSharedPointer<Song>::create();
		const xmlNodePtr songRow = node->children;
		if (!songRow)
		{
			continue;
		}

		const xmlNodePtr SongNameRowPTR = HTMLParserHelper::TraverseNext(songRow, nSongNameIndex);
		if (SongNameRowPTR && SongNameRowPTR->children)
		{
			newSong->setSongLink(safeGetProp(SongNameRowPTR->children, "href"));

			if (SongNameRowPTR->children->children)
			{
				newSong->setName(safeGetContent(SongNameRowPTR->children->children));
			}
		}
		const xmlNodePtr SongLength = HTMLParserHelper::TraverseNext(SongNameRowPTR, 2);

		if (SongLength && SongLength->children && SongLength->children->children)
		{
			QString lengthStr = safeGetContent(SongLength->children->children);
			if (lengthStr.size())
			{
				newSong->setLengthInSeconds(HTMLParserHelper::convertToSeconds(lengthStr));
			}
		}
		for (int nFormat = 0; nFormat < nFormats && nFormat < newAlbum->formats().size(); ++nFormat)
		{
			const xmlNodePtr SongFormat = HTMLParserHelper::TraverseNext(SongLength, 2 + (nFormat * 2));
			if (SongFormat && SongFormat->children && SongFormat->children->children)
			{
				QString sizeStr = safeGetContent(SongFormat->children->children);
				if (!sizeStr.size())
				{
					newSong->addSize(newAlbum->formats()[nFormat],HTMLParserHelper::convertToKB(sizeStr));
				}
			}
		}
		newAlbum->addSong(newSong);

	}
	Out->setIsInfoParsed(true);
	return cleanup(true);
}

bool KhinsiderParser::ParseSearchResults(htmlDocPtr Document, QVector<QSharedPointer<Album>>& Out)
{
	const xmlXPathContextPtr PathContextPtr = xmlXPathNewContext(Document);
	if (!PathContextPtr) 
	{
		return false;
	}
	auto cleanup = [&](bool success) {
		xmlXPathFreeContext(PathContextPtr);
		return success;
		};

	const xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(
		reinterpret_cast<const xmlChar*>("//div[@id='pageContent']"), PathContextPtr);

	if (!xpathObj || !xpathObj->nodesetval || xpathObj->nodesetval->nodeNr < 1) 
	{
		if (xpathObj) xmlXPathFreeObject(xpathObj);
		return cleanup(false);
	}

	const xmlNodePtr albumNameNode = xpathObj->nodesetval->nodeTab[0];

	const xmlNodePtr firstChild = albumNameNode->children;

	if (firstChild && xmlNodeGetContent(firstChild) != nullptr 
		&& safeGetContent(firstChild) != "Search")
	{
		return ParseAlbumPage(Document, Out);
	}

	//Parse album list
	const xmlXPathObjectPtr albumListObj = xmlXPathEvalExpression(
		reinterpret_cast<const xmlChar*>("//table[@class='albumList']"), PathContextPtr);

	if (!albumListObj || !albumListObj->nodesetval || albumListObj->nodesetval->nodeNr < 1 ||
		!albumListObj->nodesetval->nodeTab[0]->children)
	{
		if (albumListObj) xmlXPathFreeObject(albumListObj);
		return cleanup(true); 
	}

	const xmlNodePtr albumListNode = HTMLParserHelper::TraverseNext(albumListObj->nodesetval->nodeTab[0]->children,3);
	for (xmlNodePtr rowNode = albumListNode; rowNode != nullptr; rowNode = rowNode->next)
	{
		QSharedPointer<Album> newAlbum = QSharedPointer<Album>::create();
		if (!rowNode->children || !rowNode->children->next)
		{
			continue;
		}

		const xmlNodePtr albumIcon = rowNode->children->next->children;
		for (xmlNode* node = albumIcon->children; node; node = node->next)
		{
			if (node && node->type == XML_ELEMENT_NODE && xmlStrEqual(node->name, (const xmlChar*)"img"))
			{
				newAlbum->addAlbumImage(safeGetProp(node,"src"));
			}
		}

		const xmlNodePtr albumDataNode = HTMLParserHelper::TraverseNext(rowNode->children, 3);
		for (xmlNode* node = albumDataNode->children; node; node = node->next)
		{
			if (node && node->type == XML_ELEMENT_NODE && xmlStrEqual(node->name, (const xmlChar*)"a"))
			{
				newAlbum->setName(safeGetContent(node));
				newAlbum->setAlbumLink("https://downloads.khinsider.com" + safeGetProp(node, "href"));
			}
		}


		const xmlNodePtr albumPlatformsNode = HTMLParserHelper::TraverseNext(albumDataNode, 2); //5
		for (xmlNode* node = albumPlatformsNode->children; node; node = node->next)
		{
			if (node && node->type == XML_ELEMENT_NODE && xmlStrEqual(node->name, (const xmlChar*)"a"))
			{
				newAlbum->addPlatform((safeGetContent(node)));
			}
		}

		const xmlNodePtr albumTypeNode = HTMLParserHelper::TraverseNext(albumPlatformsNode, 2); //2
		for (xmlNode* node = albumTypeNode->children; node; node = node->next)
		{
			newAlbum->setType(safeGetContent(node));
		}

		const xmlNodePtr albumYearNode = HTMLParserHelper::TraverseNext(albumTypeNode, 2); //9
		for (xmlNode* node = albumYearNode->children; node; node = node->next)
		{
			newAlbum->setYear(safeGetContent(node));
		}
		Out.emplace_back(newAlbum);
	}
	return cleanup(true);
}

bool KhinsiderParser::ParsePlaylist(htmlDocPtr Document, QVector<QSharedPointer<Album>>& Out)
{
	//don't forget to clean contexts!
	const xmlXPathContextPtr PathContextPtr = xmlXPathNewContext(Document);

	auto cleanup = [&](bool success) {
		xmlXPathFreeContext(PathContextPtr);
		return success;
		};

	const xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(
		reinterpret_cast<const xmlChar*>("//div[@id='pageContent']"), PathContextPtr);

	if (!xpathObj || !xpathObj->nodesetval || xpathObj->nodesetval->nodeNr < 1) {
		if (xpathObj) xmlXPathFreeObject(xpathObj);
		return cleanup(false);
	}

	const xmlNodePtr Page = xpathObj->nodesetval->nodeTab[0]->children;
	if (!Page) {
		if (xpathObj) xmlXPathFreeObject(xpathObj);
		return cleanup(false);
	}

	xmlXPathFreeObject(xpathObj);

	QSharedPointer<Album> newAlbum = QSharedPointer<Album>::create();

	xmlNodePtr AlbumNamePtr = HTMLParserHelper::TraverseNext(Page, 2);
	if (AlbumNamePtr) {
		newAlbum->setName(safeGetContent(AlbumNamePtr->children));
	}

	newAlbum->setIsPlaylist(true);
	
	///////////
	//SONG LIST
	const xmlXPathObjectPtr songList = xmlXPathEvalExpression(
		reinterpret_cast<const xmlChar*>("//table[@id='songlist']"), PathContextPtr);

	if (!songList || !songList->nodesetval || songList->nodesetval->nodeNr < 1)
	{
		if (songList) xmlXPathFreeObject(songList);
		Out.emplace_back(newAlbum);
		return cleanup(true); // Still return true as we may have partial album info
	}
	//SONG HEADER
	const xmlNodePtr tableNode = songList->nodesetval->nodeTab[0];
	if (!tableNode || !tableNode->children)
	{
		xmlXPathFreeObject(songList);
		Out.emplace_back(newAlbum);
		return cleanup(true);
	}

	if (!tableNode->children->next)
	{
		cleanup(false);
	}

	const xmlNodePtr SongListHeaderPtr = tableNode->children->next->children;
	if (!SongListHeaderPtr)
	{
		xmlXPathFreeObject(songList);
		Out.emplace_back(newAlbum);
		return cleanup(true);
	}

	int nSongNameIndex = 0;
	xmlNodePtr SongNameHeaderPtr = nullptr;
	for (xmlNodePtr snamenode = SongListHeaderPtr; snamenode; snamenode = snamenode->next)
	{
		if (snamenode->children && snamenode->children->children && snamenode->children->children->type == XML_TEXT_NODE)
		{
			if (safeGetContent(snamenode->children->children).contains("Song"))
			{
				SongNameHeaderPtr = snamenode;
				break;
			}
		}

		nSongNameIndex++;
	}

	if (!SongNameHeaderPtr) {
		xmlXPathFreeObject(songList);
		Out.emplace_back(newAlbum);
		return cleanup(true);
	}

	//SONG HEADER END
	const xmlNodePtr SongListPtr = HTMLParserHelper::TraverseNext(tableNode->children, 3);

	if (!SongListPtr) {
		xmlXPathFreeObject(songList);
		Out.emplace_back(newAlbum);
		return cleanup(true);
	}

	for (xmlNodePtr node = SongListPtr; node; node = node->next)
	{
		if (safeGetProp(node, "id").contains("songlist_footer")) {
			break;
		}

		QSharedPointer<Song> newSong = QSharedPointer<Song>::create();
		const xmlNodePtr songRow = node->children;
		if (!songRow)
		{
			continue;
		}
		
		const xmlNodePtr SongNameRowPTR = HTMLParserHelper::TraverseNext(songRow, nSongNameIndex);
		if (SongNameRowPTR && SongNameRowPTR->children && SongNameRowPTR->children->next)
		{
			newSong->setSongLink(safeGetProp(SongNameRowPTR->children->next, "href"));

			if (SongNameRowPTR->children->next->children)
			{
				newSong->setName(safeGetContent(SongNameRowPTR->children->next->children));
			}
		}

		//Song art
		const xmlNodePtr SongArtTD = HTMLParserHelper::TraverseNext(SongNameRowPTR,-2);
		if (SongArtTD)
		{
			if (safeGetProp(SongArtTD, "class") == "albumIcon")
			{
				if(SongArtTD->children && SongArtTD->children->children)
				{
					newAlbum->addAlbumImage(safeGetProp(SongArtTD->children->children,"src"));
				}
			}
		}

		const xmlNodePtr SongLength = HTMLParserHelper::TraverseNext(SongNameRowPTR, 2);
		if (SongLength && SongLength->children && SongLength->children->children)
		{

				newSong->setLengthInSeconds( HTMLParserHelper::convertToSeconds(safeGetContent(SongLength->children->children)));

		}
		newAlbum->addSong(newSong);

	}
	Out.emplace_back(newAlbum);
	return cleanup(true);
}

bool KhinsiderParser::ParseDownloadLink(htmlDocPtr Document, QSharedPointer<Song> Out)
{
	const xmlXPathContextPtr PathContextPtr = xmlXPathNewContext(Document);
	if (!PathContextPtr)
	{
		return false;
	}
	auto cleanup = [&](bool success) {
		xmlXPathFreeContext(PathContextPtr);
		return success;
		};

	const xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(
		reinterpret_cast<const xmlChar*>("//*[@id='pageContent']/p[4]"), PathContextPtr);

	if (!xpathObj || !xpathObj->nodesetval || xpathObj->nodesetval->nodeNr < 1)
	{
		if (xpathObj) xmlXPathFreeObject(xpathObj);
		return cleanup(false);
	}
	

	const xmlNodePtr FirstDownloadLink = xpathObj->nodesetval->nodeTab[0];
	for (xmlNodePtr DownloadNode = FirstDownloadLink; DownloadNode; DownloadNode = DownloadNode->next)
	{
		if (DownloadNode->children && DownloadNode->children->type == XML_ELEMENT_NODE)
		{
			const QString link = safeGetProp(DownloadNode->children, "href");
			QString ext = link.mid(link.lastIndexOf('.') + 1);
			ext = ext.toUpper();  // Convert to uppercase
			Out->addDownloadLink(ext,safeGetProp(DownloadNode->children, "href"));
		}
		DownloadNode = DownloadNode->next;
		if(DownloadNode == nullptr)
		{
			break;
		}
	}
	
	return cleanup(true);
}
