#include "aboutcontroller.h"
#include <qdir.h>

AboutController::AboutController(QObject *parent)
    : QObject(parent)
{
    m_version = "Unknown";
    QFile file(":/APP_VERSION");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        m_version = in.readLine().trimmed();
    }

}

QString AboutController::appVersion() const {
    return m_version;
}
