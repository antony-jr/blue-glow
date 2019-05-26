#ifndef BLUE_GLOW_HPP_INCLUDED
#define BLUE_GLOW_HPP_INCLUDED
#include <QMessageBox>
#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <ui_mainwindow.h>

class BlueGlow : public QMainWindow
{
    Q_OBJECT
public:
    BlueGlow(QWidget *parent = nullptr);
    ~BlueGlow();
public Q_SLOTS:
    void init();
Q_SIGNALS:
    void quit();
private:
    Ui::MainWindow m_Ui;
    QSystemTrayIcon *m_TIcon = nullptr;
};

#endif // APPIMAGEUPDATER_HPP_INCLUDED
