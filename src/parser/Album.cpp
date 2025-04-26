#include "Album.h"
void Album::resetState()
{
    m_isDownloading = false;
    m_hasErrors = false;
    for (auto song : m_songs) {
        song->setDownloaded(false);
        song->setDownloadedSize(0);
        song->setDownloadSpeed(0);
        song->setErrored(false);
    }
}
