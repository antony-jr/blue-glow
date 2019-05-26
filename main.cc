#include <QApplication>
#include <BlueGlow.hpp>

int main(int ac , char **av){
	QApplication::setApplicationName(QString::fromUtf8("Blue Glow"));
	QApplication::setOrganizationName(QString::fromUtf8("The Future Shell Laboratory"));
	QApplication app(ac,av);
	BlueGlow guiMain;
	guiMain.init();
	QObject::connect(&guiMain , &BlueGlow::quit , &app , 
			 &QApplication::quit , Qt::QueuedConnection);
	return app.exec();
}
