#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <SettingsManager.hpp>

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent) { }

SettingsManager::~SettingsManager() { }

void SettingsManager::deleteToken() {
    m_Settings.setValue("GithubToken", "");
    emit deletedToken();
    return;
}

void SettingsManager::setSettings(bool darkMode,
                                  bool startOnStartup,
                                  bool showPopup,
                                  bool soundAlert,
                                  bool useNotifySend) {
    m_Settings.setValue("darkMode", darkMode);
    m_Settings.setValue("runOnStartup", startOnStartup);
    m_Settings.setValue("showNotifications", showPopup);
    m_Settings.setValue("useSoundAlert", soundAlert);
    m_Settings.setValue("useNotifySend", useNotifySend);

    if(startOnStartup) {
        auto arguments = QCoreApplication::arguments();
        QString entry = QString::fromUtf8("[Desktop Entry]\n"
                                          "Name=AppImageUpdater\n"
                                          "Type=Application\n"
                                          "Exec=%1 --startup\n"
                                          "Terminal=false");
        entry = entry.arg(QFileInfo(arguments.at(0)).absolutePath() +
                          QString::fromUtf8("/") +
                          QFileInfo(arguments.at(0)).fileName());
        QFile entryFile(QDir::homePath() + QString::fromUtf8("/.config/autostart/BlueGlow.desktop"));
        if(!entryFile.open(QIODevice::WriteOnly)) {
            emit finished();
            return;
        }
        entryFile.write(entry.toLatin1());
        entryFile.close();
    } else {
        QFile::remove(QDir::homePath() + QString::fromUtf8("/.config/autostart/BlueGlow.desktop"));
    }
    emit finished();
    return;
}

void SettingsManager::getSettings() {
    bool authenticatedToken = true;
    QString token = m_Settings.value("GithubToken").toString();
    if(token.isEmpty()) {
        authenticatedToken = false;
    }
    emit settings(
        m_Settings.value("darkMode").toBool(),
        m_Settings.value("runOnStartup").toBool(),
        m_Settings.value("showNotifications").toBool(),
        m_Settings.value("useSoundAlert").toBool(),
        authenticatedToken,
        m_Settings.value("useNotifySend").toBool()
    );
    return;
}
