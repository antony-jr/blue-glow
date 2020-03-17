#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>

//#include <MongooseBackend.hpp>
//#include <Clipper.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setOrganizationName("BlueGlow");
    QApplication::setApplicationName("BlueGlow");
    
    //qmlRegisterType<MongooseBackend>("Core.MongooseBackend", 1, 0, "MongooseBackend");
    //qmlRegisterType<Clipper>("Core.Clipper", 1, 0, "Clipper");

    app.setWindowIcon(QIcon(QString::fromUtf8(":/logo.png")));
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
