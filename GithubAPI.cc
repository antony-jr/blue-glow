#include <GithubAPI.hpp>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

GithubAPI::GithubAPI(QObject *parent)
    : QObject(parent),
      n_Interval(3000),
      m_Manager(new QNetworkAccessManager(parent)) {
    Q_ASSERT(m_Manager != nullptr);

    m_Buffer.reset(new QByteArray);
    m_KnownNotifications.reset(new QStringList);

    connect(&m_Timer, &QTimer::timeout, this, &GithubAPI::requestNotifications);
    return;
}

GithubAPI::~GithubAPI() {
    m_Timer.stop();
    m_Manager->deleteLater();
}

void GithubAPI::clear() {
    m_Timer.stop();
    m_Token.clear();
    b_Logged = false;
    m_KnownNotifications->clear();
    m_Buffer->clear();
}

void GithubAPI::setToken(const QString &token) {
    m_Token = token;
}

void GithubAPI::setInterval(qint64 msecs) {
    n_Interval = msecs;
    return;
}

void GithubAPI::init() {
    if(b_Logged) {
        return;
    }
    QNetworkRequest req(QUrl("https://api.github.com/notifications"));
    req.setRawHeader(QByteArray("Authorization"), ("token " + m_Token).toUtf8());
    auto reply = m_Manager->get(req);
    connect(reply, &QNetworkReply::downloadProgress, this, &GithubAPI::appendToBuffer);
    connect(reply, &QNetworkReply::finished, this, &GithubAPI::handleLogin);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this,  SLOT(handleNetworkError(QNetworkReply::NetworkError)));
}

/* Private Slots. */

void GithubAPI::appendToBuffer(qint64 bytesReceived, qint64 bytesTotal) {
    Q_UNUSED(bytesReceived);
    Q_UNUSED(bytesTotal);
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(!reply ||
            !reply->isOpen() ||
            !reply->isReadable()) {
        return;
    }
    m_Buffer->append(reply->readAll());
}

void GithubAPI::handleNetworkError(QNetworkReply::NetworkError code) {
    /* Self-Note:
     * 	Do not arise error when the error is about network
     * 	connectivity. */
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(!reply ||
            reply->manager()->networkAccessible() != QNetworkAccessManager::Accessible) {
        if(!b_Logged) {
            QTimer::singleShot(3000, [&]() {
                init();
            });
        } else {
            m_Timer.start();
        }
        return;
    }
    emit error(code);
    return;
}

void GithubAPI::handleLogin() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(!reply) {
        return;
    }
    m_Buffer->append(reply->readAll());

    auto json = QJsonDocument::fromJson(*(m_Buffer.data()));
    if(QJsonValue::Undefined == ((json.object()).value("message")).type() ||
            (json.array()).size()) {
        m_Buffer->clear();
        reply->deleteLater();

        // Check if the login was successful.
        if((json.object()).value("message").toString() == "Bad credentials") {
            emit logged((b_Logged = false));
            return;
        }
        emit logged((b_Logged = true));

        /* Start the notification request loop. */
        m_Timer.setInterval(n_Interval);
        m_Timer.start();
        return;
    }
    emit logged((b_Logged = false));
    m_Buffer->clear();
    reply->deleteLater();
    return;
}

void GithubAPI::handleNotification() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(!reply) {
        return;
    }
    m_Buffer->append(reply->readAll());

    qint64 newNotifications = 0;
    auto json = (QJsonDocument::fromJson(*(m_Buffer.data()))).array();

    /* clear the m_KnownNotifications if there is no notifications to work
     * on , this is to save space. */
    if(json.size() == 0) {
        m_KnownNotifications->clear();
    } else {
        foreach(const QJsonValue &value, json) {
            auto nId = ((value.toObject())["id"]).toString() +
                       ((value.toObject())["last_read_at"]).toString();
            if(!m_KnownNotifications->contains(nId)) {
                m_KnownNotifications->append(nId);
                ++newNotifications;
            }
        }
    }

    emit notification(newNotifications, /* total notifications = */json.size());

    m_Buffer->clear();
    reply->deleteLater();
    m_Timer.start();
}

void GithubAPI::requestNotifications() {
    m_Timer.stop();
    m_Buffer->clear();
    QNetworkRequest req(QUrl("https://api.github.com/notifications"));
    req.setRawHeader(QByteArray("Authorization"), ("token " + m_Token).toUtf8());
    auto reply = m_Manager->get(req);
    connect(reply, &QNetworkReply::downloadProgress, this, &GithubAPI::appendToBuffer);
    connect(reply, &QNetworkReply::finished, this, &GithubAPI::handleNotification);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this,  SLOT(handleNetworkError(QNetworkReply::NetworkError)));
}
