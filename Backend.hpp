#ifndef BACKEND_HPP_INCLUDED
#define BACKEND_HPP_INCLUDED
#include <QTimer>
#include <QObject>
#include <QSystemTrayIcon>
#include <QSettings>
#include <GithubAPI.hpp>

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(NOTIFY showOrHide);
    Q_PROPERTY(NOTIFY showApp);
    Q_PROPERTY(NOTIFY showAuthPage);
    Q_PROPERTY(NOTIFY showSettingsPage);
    Q_PROPERTY(NOTIFY showLoader);
    Q_PROPERTY(NOTIFY settingsUpdated);
    Q_PROPERTY(NOTIFY error);
    Q_PROPERTY(NOTIFY finished);
  public:
    Backend(QObject *parent = nullptr);
    ~Backend();
  public Q_SLOTS:
    Q_INVOKABLE void init();
    Q_INVOKABLE void updateSettings();
    Q_INVOKABLE void setAuthToken(const QString&);
  private Q_SLOTS:
    void handleLogin(bool);
    void handleNotifications(qint64, qint64);
    void resetPages();
  signals:
    void quit();
    void finished();
    void settingsUpdated();
    void showOrHide();
    void showApp(bool visible);
    void showLoader(bool visible);
    void showAuthPage(bool visible);
    void showSettingsPage(bool visible);
    void error(QString title, QString message);
  private:
    // These settings are the only thing we need to cache
    // and others are really not needed.
    bool b_ShowNotifications = false,
         b_UseSoundAlert = false,
         b_UseNotifySend = false,
	 b_DarkMode = false;
    QString m_Token;

    QSettings m_Settings;
    QSystemTrayIcon *m_TIcon = nullptr;
    GithubAPI *m_API = nullptr;
};

#endif // BACKEND_HPP_INCLUDED
