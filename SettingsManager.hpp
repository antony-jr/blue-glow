#ifndef SETTINGS_MANAGER_HPP_INCLUDED
#define SETTINGS_MANAGER_HPP_INCLUDED
#include <QObject>
#include <QString>
#include <QSettings>

class SettingsManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(NOTIFY deletedToken)
    Q_PROPERTY(NOTIFY settings)
    Q_PROPERTY(NOTIFY error)
    Q_PROPERTY(NOTIFY finished)

    QSettings m_Settings;
  public:
    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager();
    Q_INVOKABLE void setSettings(bool darkMode,
		                 bool startOnStartup,
				 bool showPopup,
				 bool soundAlert,
				 bool useNotifySend);    
    Q_INVOKABLE void getSettings();
    Q_INVOKABLE void deleteToken();

  signals:
    void finished();
    void error(QString);
    void deletedToken();
    void settings(bool darkMode,
		  bool startOnStartup,
		  bool showPopup,
		  bool soundAlert,
		  bool authenticated,
		  bool notifySend);    
 
};

#endif // SETTINGS_MANAGER_HPP_INCLUDED
