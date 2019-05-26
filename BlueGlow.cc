#include <QDir>
#include <QMovie>
#include <QDebug>
#include <BlueGlow.hpp>
#include <QScreen>
#include <QSound>

/* Useful macros to convert bool to Qt::Checked and Qt::Unchecked and vice-versa */
#define IS_CHECKED(checkbox) (checkbox->checkState() == Qt::Checked) ? true : false
#define BOOL_CHECKED(B) (B == true) ? Qt::Checked : Qt::Unchecked

BlueGlow::BlueGlow(QWidget *parent)
    : QMainWindow(parent),
      m_API(new GithubAPI(&m_Settings , this))
{
    m_Ui.setupUi(this);
    QMovie *movie = new QMovie(":/loading.gif");
    (m_Ui.loadingGif)->setMovie(movie);
    this->move(QGuiApplication::primaryScreen()->geometry().center() - this->rect().center());

    auto arguments = QCoreApplication::arguments();
    m_DEntry = m_DEntry.arg(QFileInfo(arguments.at(0)).absolutePath() +
                            QString::fromUtf8("/") +
                            QFileInfo(arguments.at(0)).fileName());


    m_TIcon = new QSystemTrayIcon(this);
    m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/no-notification.png"))));
    m_TIcon->show();
    connect(m_TIcon, &QSystemTrayIcon::activated, this, &BlueGlow::show);

    (m_Ui.AuthToken)->installEventFilter(this);

    /* Set the UI to match the settings. */
    (m_Ui.showPopupMsg)->setCheckState(BOOL_CHECKED(m_Settings.value("showPopupMsg").toBool()));
    (m_Ui.notifyWithSound)->setCheckState(BOOL_CHECKED(m_Settings.value("notifyWithSound").toBool()));
    (m_Ui.startOnStartup)->setCheckState(BOOL_CHECKED(m_Settings.value("runOnStartup").toBool()));

    /* Program logic */
    connect(m_Ui.cancelBtn , &QPushButton::clicked , this , &BlueGlow::hide);
    connect(m_Ui.SetAuthToken , &QPushButton::clicked , this , &BlueGlow::handleAuthToken);
    connect(m_Ui.delGithubTokenBtn , &QPushButton::clicked , this , &BlueGlow::handleDeleteToken);
    connect(m_Ui.okBtn , &QPushButton::clicked , this , &BlueGlow::handleSaveSettings);
    connect(m_Ui.actionQuit , &QAction::triggered , this , &BlueGlow::quit , Qt::DirectConnection);
    connect(m_Ui.actionAbout_Qt , &QAction::triggered , this , &BlueGlow::handleAboutQt);
    connect(m_Ui.actionAbout , &QAction::triggered , this , &BlueGlow::handleAbout);
    connect(m_API , &GithubAPI::logged , this , &BlueGlow::handleLogin);	
    return;
}

BlueGlow::~BlueGlow()
{
    return;
}

void BlueGlow::init()
{
    /* Get Auth Token if not set. */ 
    if(m_Settings.value("GithubToken").toString().isEmpty()){
	    m_Settings.setValue("showPopupMsg" , true);
	    (m_Ui.showPopupMsg)->setCheckState(BOOL_CHECKED(m_Settings.value("showPopupMsg").toBool()));
	    (m_Ui.MainStack)->setCurrentIndex(GITHUB_AUTH_TOKEN_PAGE);
	    this->show();
    }else{
	    (m_Ui.MainStack)->setCurrentIndex(SETTINGS_PAGE);
	    this->hide();

	    toggleLoading(LOADING_PAGE);
	    m_API->init();
    }
    return;
}

/* Protected Methods */
bool BlueGlow::eventFilter(QObject *watched , QEvent *event){
	if(watched == m_Ui.AuthToken){
		if(event->type() == QKeyEvent::KeyPress){
			QKeyEvent *k = static_cast<QKeyEvent*>(event);	
			if(k->key() == Qt::Key_Return || k->key() == Qt::Key_Enter){
				QTimer::singleShot(0 , this , &BlueGlow::handleAuthToken);
				return true;
			}
		}
		return false;
	}else{
		return QMainWindow::eventFilter(watched,event);
	}
}

/* Private slots.*/

void BlueGlow::handleNotifications(qint64 newNotifications , qint64 n){
    if(!n){
	    m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/no-notification.png"))));
	    return;
    }
    m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/has-notification.png"))));
    if(!newNotifications){
	    return;
    }

    if(m_Settings.value("showPopupMsg").toBool()){
	    m_TIcon->showMessage(QString("%1 new notification(s) on Github").arg(newNotifications),
                         QString("You have a total of %1 notifications on Github.").arg(n));
    }

    if(m_Settings.value("notifyWithSound").toBool()){
	    QSound::play(":/sound.wav");
    }
}

void BlueGlow::handleLogin(bool logged){
	if(!logged){
		this->show();
		toggleLoading(GITHUB_AUTH_TOKEN_PAGE);
		QMessageBox::critical(this , "Cannot login" , 
		                      "Blue Glow cannot login with the given access token , please "
		                      "make sure that you have given the permission to read notification.");
		return;
	}
	toggleLoading(SETTINGS_PAGE);
	connect(m_API , &GithubAPI::notification , this , &BlueGlow::handleNotifications);
}	

void BlueGlow::handleAuthToken(){
	auto token = (m_Ui.AuthToken)->text();
	
	/* Check if its a valid token. */
	if(token.isEmpty()){
		QMessageBox::critical(this , "Invalid Token" , 
		                      "You have given an invalid github token , please enter "
		                      "a valid github token.");
		return;
	}

	this->hide();
	toggleLoading(LOADING_PAGE);

	m_Settings.setValue("GithubToken" , token);
	m_API->init(); /* try logging in and start the notification loop. */
}

void BlueGlow::handleDeleteToken(){
	auto pressed =  QMessageBox::question(this , "Are you sure ?" , 
		                      "Deleting your github token will also delete your settings and you will"
				      " not get any notifications until you provide a github token.");
	if(pressed != QMessageBox::Yes){
		return;
	}
	
	toggleLoading(LOADING_PAGE);
	m_Settings.setValue("showPopupMsg" , true);
	m_Settings.setValue("notifyWithSound"  , false);

	if(m_Settings.value("runOnStartup").toBool()){
		QFile::remove(QDir::homePath() + QString::fromUtf8("/.config/autostart/BlueGlow.desktop"));
	}
	m_Settings.setValue("runOnStartup" ,false);
	m_Settings.setValue("GithubToken" , "");
	toggleLoading(GITHUB_AUTH_TOKEN_PAGE);
}

void BlueGlow::handleSaveSettings(){
    m_Settings.setValue("showPopupMsg", IS_CHECKED(m_Ui.showPopupMsg));
    m_Settings.setValue("notifyWithSound" , IS_CHECKED(m_Ui.notifyWithSound));

    if(IS_CHECKED(m_Ui.startOnStartup)) {
        QFile entryFile(QDir::homePath() + QString::fromUtf8("/.config/autostart/BlueGlow.desktop"));
        if(!entryFile.open(QIODevice::WriteOnly)) {
            return;
        }
        entryFile.write(m_DEntry.toLatin1());
        entryFile.close();
        m_Settings.setValue("runOnStartup", true);
    } else {
        QFile::remove(QDir::homePath() + QString::fromUtf8("/.config/autostart/BlueGlow.desktop"));
        m_Settings.setValue("runOnStartup", false);
    } 
}

void BlueGlow::handleAbout(){
	QMessageBox::about(this , 
			   QString::fromUtf8("About Blue Glow"), 
			   "Blue Glow , Copyright (C) 2019 , Antony Jr.\n"
			   "\n\n" 
			   "Notification Bell by https://www.flaticon.com/authors/pixel-perfect\n"
			   "Application Logo by https://www.flaticon.com/authors/chanut\n" 
			   );
	return;
}

void BlueGlow::handleAboutQt(){
	QMessageBox::aboutQt(this);
	return;
}

void BlueGlow::toggleLoading(int nextIndex){
	auto movie = (m_Ui.loadingGif)->movie();
	auto stack = (m_Ui.MainStack);
	if(QMovie::Running == movie->state()){
		movie->stop();
		stack->setCurrentIndex(nextIndex);
		return;
	}
	(void)nextIndex;
	movie->start();
	stack->setCurrentIndex(LOADING_PAGE);
	return;
}
