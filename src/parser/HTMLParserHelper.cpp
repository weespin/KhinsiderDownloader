#include "HTMLParserHelper.h"
#include <QList>
#include <qtextstream.h>

xmlNodePtr HTMLParserHelper::SkipTo(const QString &title, xmlNodePtr node) {
	if (node == nullptr) {
		return nullptr;
	}
	do {
		if (node->content) {
			QString content = QString::fromUtf8((const char *) node->content);
			if (content.contains(title, Qt::CaseInsensitive)) {
				return node;
			}
		}
		node = node->next;
	} while (node && node->next);
	return nullptr;
}

unsigned long HTMLParserHelper::convertToKB(const QString &str) {
	QString trimmed = str.trimmed();
	QStringList parts = trimmed.split(' ', Qt::SkipEmptyParts);

	if (parts.size() < 2)
		return 0;

	double value = parts[0].toDouble();
	QString unit = parts[1];

	if (unit == "MB") {
		return value * 1024;
	} else if (unit == "GB") {
		return value * 1024 * 1024;
	} else if (unit == "KB") {
		return value;
	}

	return 0;
}

int HTMLParserHelper::convertToSeconds(const QString &time_str) {
	QStringList parts = time_str.split(':');

	if (parts.size() != 2)
		return 0;

	int minutes = parts[0].toInt();
	int seconds = parts[1].toInt();

	return (minutes * 60) + seconds;
}

QString HTMLParserHelper::cleanFormat(const QString &str) {
	QString cleanedStr = str;
	cleanedStr.remove('(');
	cleanedStr.remove(')');
	return cleanedStr;
}

int HTMLParserHelper::GetNextCount(xmlNodePtr node) {
	int count = 0;
	while ((node = node->next)) {
		++count;
	}
	return count;
}

xmlNodePtr HTMLParserHelper::TraverseNext(xmlNodePtr In, int depth) {
	if (depth == 0 || !In) {
		return In;
	}

	if (depth > 0) {
		for (int i = 0; i < depth && In; ++i)
			In = In->next;
	} else {
		for (int i = 0; i < -depth && In; ++i)
			In = In->prev;
	}

	return In;
}

xmlNodePtr HTMLParserHelper::ToNextTag(xmlNodePtr In, const QString &tag) {
	do {
		In = In->next;
		if (In) {
			if (In->name) {
				QString name = QString::fromUtf8((const char *) In->name);
				if (name.contains(tag, Qt::CaseInsensitive)) {
					return In;
				}
			}
		}
	} while (In);
	return nullptr;
}
