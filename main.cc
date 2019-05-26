#include <QApplication>
#include <BlueGlow.hpp>

int main(int ac , char **av){
	QApplication app(ac,av);
	BlueGlow guiMain;
	guiMain.init();
	QObject::connect(&guiMain , &BlueGlow::quit , &app , 
			 &QApplication::quit , Qt::QueuedConnection);
	return app.exec();
}
