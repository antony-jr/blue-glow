#include <Backend.hpp>
#include <QMenu>
#include <QSound>
#include <QProcess>

/* Useful macros to convert bool to Qt::Checked and Qt::Unchecked and vice-versa */
#define IS_CHECKED(checkbox) (checkbox->checkState() == Qt::Checked) ? true : false
#define BOOL_CHECKED(B) (B == true) ? Qt::Checked : Qt::Unchecked

Backend::Backend(QObject *parent)
    : QObject(parent),
      m_API(new GithubAPI(this)) {
    // System Tray Icon.
    m_TIcon = new QSystemTrayIcon(this);
    m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/no-notification.png"))));

    // Menu context for the tray icon
    QMenu *menu = new QMenu;
    menu->addAction(QString::fromUtf8("Show / Hide"),
                    this,
                    &Backend::showOrHide);
    menu->addAction(QString::fromUtf8("Quit"),
                    this,
                    &Backend::quit);

    m_TIcon->setContextMenu(menu);
    m_TIcon->show();
    connect(m_TIcon, &QSystemTrayIcon::activated, this, &Backend::showOrHide);
    // ----

    // Cache Settings
    b_ShowNotifications = m_Settings.value("showNotifications").toBool();
    b_UseSoundAlert = m_Settings.value("useSoundAlert").toBool();
    b_UseNotifySend = m_Settings.value("useNotifySend").toBool();
    // ---

    // Logic
    connect(m_API, &GithubAPI::logged, this, &Backend::handleLogin);
    return;
}

Backend::~Backend() {
    return;
}

void Backend::init() {
    emit showLoader(true);
    m_API->clear();
    if(m_Settings.value("GithubToken").toString().isEmpty()) {
        resetPages();
        emit showLoader(false);
        emit showApp(true);
        emit showAuthPage(true);
    } else {
        resetPages();
        emit showApp(false);
        emit showAuthPage(true);
        emit showLoader(true);
        m_API->setToken(m_Settings.value("GithubToken").toString());
        m_API->init();
    }
    return;
}

void Backend::handleNotifications(qint64 newNotifications, qint64 n) {
    if(!n) {
        m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/no-notification.png"))));
        return;
    }
    m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/has-notification.png"))));
    if(!newNotifications) {
        return;
    }

    if(b_ShowNotifications) {
        int ret = -1;
        if(b_UseNotifySend) {
            ret = QProcess::execute(QString::fromUtf8("notify-send"),
                                    QStringList() << QString::fromUtf8("-u")
                                    << QString::fromUtf8("critical")
                                    << QString::fromUtf8("-a")
                                    << QString::fromUtf8("Github")
                                    <<
                                    QString("%1 new notification(s), from a total of %2 notifications.")
                                    .arg(newNotifications).arg(n));
        }

        if(ret == -1 || ret == -2) {
            // Fallback
            m_TIcon->showMessage(QString("%1 new notification(s) on Github").arg(newNotifications),
                                 QString("You have a total of %1 notifications on Github.").arg(n));
        }
    }

    if(b_UseSoundAlert) {
        QSound::play(":/sound.wav");
    }
}

void Backend::handleLogin(bool logged) {
    if(!logged) {
        resetPages();
        m_Settings.setValue("GithubToken", "");
        emit showLoader(false);
        emit showAuthPage(true);
        emit showApp(true);
        emit error("Cannot Login",
                   "Blue Glow cannot login with the given access token, please "
                   "make sure that you have given the permission to read notification.");
        return;
    }

    if(!m_Token.isEmpty()) {
        m_Settings.setValue("GithubToken", m_Token);
    }
    resetPages();
    emit showLoader(false);
    emit showSettingsPage(true);
    emit finished();
    connect(m_API, &GithubAPI::notification, this, &Backend::handleNotifications);
}

void Backend::setAuthToken(const QString &token) {
    /* Check if its a valid token. */
    if(token.isEmpty()) {
        emit error("Invalid Token",
                   "You have given an invalid github token, please enter "
                   "a valid github token.");
        return;
    }

    resetPages();
    emit showAuthPage(true);
    emit showLoader(true);

    m_API->clear();
    m_API->setToken((m_Token = token));
    m_API->init(); /* try logging in and start the notification loop. */
}

void Backend::updateSettings() {
    b_ShowNotifications = m_Settings.value("showNotifications").toBool();
    b_UseSoundAlert = m_Settings.value("useSoundAlert").toBool();
    b_UseNotifySend = m_Settings.value("useNotifySend").toBool();
    emit settingsUpdated();
}

void Backend::resetPages() {
    emit showLoader(false);
    emit showAuthPage(false);
    emit showSettingsPage(false);
    emit showApp(false);
    return;
}
