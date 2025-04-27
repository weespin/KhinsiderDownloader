#pragma once
#include <QString>
#include <libxml/tree.h>

class HTMLParserHelper {
public:
    static xmlNodePtr SkipTo(const QString &title, xmlNodePtr node);

    static xmlNodePtr TraverseNext(xmlNodePtr node, int depth);

    static unsigned long convertToKB(const QString &str);

    static int convertToSeconds(const QString &time_str);

    static QString cleanFormat(const QString &str);

    static int GetNextCount(xmlNodePtr node);

    static xmlNodePtr ToNextTag(xmlNodePtr In, const QString &tag);
};

