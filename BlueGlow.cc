#include <BlueGlow.hpp>
#include <QScreen>

BlueGlow::BlueGlow(QWidget *parent)
    : QMainWindow(parent)
{
    m_Ui.setupUi(this);
    this->move(QGuiApplication::primaryScreen()->geometry().center() - this->rect().center());
    
    m_TIcon = new QSystemTrayIcon(this);
    m_TIcon->setIcon(QIcon(QPixmap(QString::fromUtf8(":/logo.png"))));
    m_TIcon->show();

    return;
}

BlueGlow::~BlueGlow()
{
    return;
}

void BlueGlow::init()
{
    this->show();
    return;
}
