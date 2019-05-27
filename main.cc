#include <QApplication>
#include <BlueGlow.hpp>
#include <QTimer>

int main(int ac , char **av){
	QApplication::setApplicationName(QString::fromUtf8("Blue Glow"));
	QApplication::setOrganizationName(QString::fromUtf8("The Future Shell Laboratory"));
	QApplication app(ac,av);
	app.setQuitOnLastWindowClosed(false);

	if(ac > 1){
		if(!qstrcmp(av[1] , "--startup")){
			/* If this is on startup , do not start the 
			 * application right away since the system has to do
			 * some stuff to initialize the system tray.
			 * If we initialize our system tray icon before 
			 * the desktop environment initializes then it will 
			 * cause our notifications odd from the DE.
			*/
			QTimer timer;
			timer.setInterval(30 * 1000); /* Lets wait 30 seconds. */
			timer.setSingleShot(true);

			QObject::connect(&timer , &QTimer::timeout , &app , &QApplication::quit);
			timer.start();

			app.exec(); /* Lets pause the program till then. */
		}
	}

	BlueGlow guiMain;
	guiMain.init();
	QObject::connect(&guiMain , &BlueGlow::quit , &app , 
			 &QApplication::quit , Qt::QueuedConnection);
	return app.exec();
}
