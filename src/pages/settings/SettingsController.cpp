#include "SettingsController.h"

#include <qversionnumber.h>

SettingsController::SettingsController(QObject *parent, Settings *settings): QObject(parent),
                                                                             m_settings("config.ini",
                                                                                 QSettings::IniFormat) {
    QSettings::setDefaultFormat(QSettings::IniFormat);
    m_SettingsObject = settings;
    m_SettingsObject->ParseFromConfig(m_settings);
    m_settingsVM = new SettingsViewModel(parent);
    m_settingsVM->setSettingsObject(m_SettingsObject);
    connect(m_SettingsObject, &Settings::settingsChanged, m_settingsVM, &SettingsViewModel::settingsChanged);
    connect(m_SettingsObject, &Settings::settingsChanged, this, &SettingsController::saveConfig);
}

void SettingsController::saveConfig() {
    m_settings.setValue("downloadPath", m_SettingsObject->downloadPath());
    m_settings.setValue("suppressLogs", m_SettingsObject->suppressLogs());
    m_settings.setValue("downloadThreads", m_SettingsObject->downloadThreads());
    m_settings.setValue("preferredAudioQuality", static_cast<int>(m_SettingsObject->preferredAudioQuality()));
    m_settings.setValue("maxConcurrentDownloadsPerThread",
                        static_cast<int>(m_SettingsObject->maxConcurrentDownloadsPerThread()));
    m_settings.setValue("downloadArt", static_cast<bool>(m_SettingsObject->downloadArt()));
    m_settings.setValue("skipDownloaded", static_cast<bool>(m_SettingsObject->skipDownloaded()));
    m_settings.sync();
}
