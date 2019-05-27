#ifndef BLUE_GLOW_HPP_INCLUDED
#define BLUE_GLOW_HPP_INCLUDED
#include <QMessageBox>
#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QEvent>
#include <QSystemTrayIcon>
#include <QSettings>
#include <ui_mainwindow.h>
#include <GithubAPI.hpp>

class BlueGlow : public QMainWindow
{
    Q_OBJECT
public:
    BlueGlow(QWidget *parent = nullptr);
    ~BlueGlow();
public Q_SLOTS:
    void init();
private Q_SLOTS:
    void toggleLoading(int);
    void handleAuthToken();
    void handleLogin(bool);
    void handleNotifications(qint64 , qint64);
    void handleAboutQt();
    void handleAbout();
    void handleDeleteToken();
    void handleSaveSettings();
Q_SIGNALS:
    void quit();
protected:
    bool eventFilter(QObject*,QEvent*);
private:
    /* Index for the MainStack. */
    enum : short {
	GITHUB_AUTH_TOKEN_PAGE,
	SETTINGS_PAGE,
	LOADING_PAGE
    };

    QString m_DEntry = QString::fromUtf8("[Desktop Entry]\n"
		                         "Name=AppImageUpdater\n"
					 "Type=Application\n"
					 "Exec=%1 --startup\n"
					 "Terminal=false");
    Ui::MainWindow m_Ui;
    QSettings m_Settings;
    QSystemTrayIcon *m_TIcon = nullptr;
    GithubAPI *m_API = nullptr; 
};

#endif // APPIMAGEUPDATER_HPP_INCLUDED
