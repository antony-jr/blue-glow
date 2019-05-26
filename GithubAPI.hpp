#ifndef GITHUB_API_HPP_INCLUDED
#define GITHUB_API_HPP_INCLUDED
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QScopedPointer>
#include <QStringList>
#include <QTimer>

class GithubAPI : public QObject {
	Q_OBJECT
public:
	GithubAPI(QSettings * , QObject *parent = nullptr);
	~GithubAPI();
public Q_SLOTS:
	void setInterval(qint64);
	void init();
private Q_SLOTS:
	void requestNotifications();
	void appendToBuffer(qint64 , qint64);
	void handleLogin();
	void handleNotification();
	void handleNetworkError(QNetworkReply::NetworkError);
Q_SIGNALS:
	void logged(bool);
	void notification(qint64 , qint64);
	void error(QNetworkReply::NetworkError);
private:
	const QString m_APIUrlTemplate = 
		QString::fromUtf8("https://api.github.com/notifications?access_token=%1");
	QUrl m_GithubNotificationUrl;
	bool b_Logged = false;
	qint64 n_Interval;
	QTimer m_Timer;
	QNetworkAccessManager *m_Manager;	
	QSettings *m_Settings;
	QScopedPointer<QStringList> m_KnownNotifications;
	QScopedPointer<QByteArray> m_Buffer;
};

#endif // GITHUB_API_HPP_INCLUDED
