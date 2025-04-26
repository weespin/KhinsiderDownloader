#ifndef SETTINGSVIEWMODEL_H
#define SETTINGSVIEWMODEL_H
#include "Settings.h"

class SettingsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Settings* settings READ settings NOTIFY settingsChanged)
public:
    explicit SettingsViewModel(QObject *parent = nullptr)
        : QObject(parent){}

    Settings* settings() const { return m_Settings; }

public slots:
    void setSettingsObject(Settings* settings)
    {
        m_Settings = settings;
        emit settingsChanged();
    }

signals:
    void updateFound(const QString& newVersion);
    void settingsChanged();

private:
    Settings* m_Settings;
};
#endif //SETTINGSVIEWMODEL_H
