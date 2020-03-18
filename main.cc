#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>

#include <SettingsManager.hpp>
#include <Backend.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setOrganizationName("BlueGlow");
    QApplication::setApplicationName("BlueGlow");

    qmlRegisterType<SettingsManager>("Core.SettingsManager", 1, 0, "SettingsManager");
    qmlRegisterType<Backend>("Core.Backend", 1, 0, "Backend");

    app.setQuitOnLastWindowClosed(false);
    app.setWindowIcon(QIcon(QString::fromUtf8(":/logo.png")));
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
