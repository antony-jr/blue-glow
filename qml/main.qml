import QtQuick 2.12
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import Core.SettingsManager 1.0
import Core.Backend 1.0

import "./Components" as Components

ApplicationWindow {
    id: root
    title: qsTr("Blue Glow")
    width: 700
    height: 440
    minimumWidth: 600
    minimumHeight: 430
    visible: false
    Material.theme: Material.Light 

    Component.onCompleted: {
	settingsManager.getSettings(); // get settings.
	
	backend.init();

	setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
    
    
    /* Components.MainMenu is a MenuBar QML Object.
     * This is a Customized Object which takes a ApplicationWindow.
     * This expects all the properties on changing pages to be 
     * present. */
    menuBar: Components.MainMenu {
        id: mainMenu
        mainWindow: root
    }
    
    /*
     * These properties are used by some other 
     * components to navigate through different 
     * screens in the Application.
    */

    property bool isDarkMode: false;
    property bool isRunOnStartup: false;
    property bool isShowNotifications: false;
    property bool isUseSoundAlert: false;
    property bool isAuthenticated: false;
    property bool loading: false;

    property bool showAuthPage: false;
    property bool showSettingsPage: false;
    property bool showAboutPage: false;
    property bool showErrorDialog: false;
    property string bannerImage: "qrc:/banner-dark.png";
    property string errorTitle;
    property string errorMessage;


    /* 
     * This component is to show all kinds
     * of error dialogs.
     *
    */
    Components.ErrorDialog {
	id: errorDialog
	mainWindow: root
    }
    /* -- End Components.ErrorDialog -- */


    /* All the Pages. */
    Components.SettingsPage {
	id: settingsPage
	mainWindow: root
	manager: settingsManager
    }

    Components.AboutPage {
	id: aboutPage
	mainWindow: root
    } 
    
    Components.AuthPage {
        id: mainPage
	mainWindow: root
	backend: backend
    }
    // -- End All Pages -- 

    Backend {
	    id: backend
	    onShowApp: {
		    if(visible){
			    root.show();
		    }else{
			    root.hide();
		    }
	    }
	    onShowAuthPage: {
		    root.showAuthPage = visible;
	    }
	    onShowSettingsPage: {
		    root.showSettingsPage = visible;
	    }
	    onShowLoader: {
		    root.loading = visible;	    
	    }
	    onError: {
		    root.errorTitle = title;
		    root.errorMessage = message;
		    root.showErrorDialog = true;
	    }
	    onFinished: {
		    settingsManager.getSettings();
	    }
    }
    
    SettingsManager {
	id: settingsManager
	onFinished: {
		settingsManager.getSettings();
		backend.updateSettings()
	}

	onDeletedToken: {
		settingsManager.getSettings();
		backend.updateSettings();
		backend.init();
	}

	onSettings: {
		root.isDarkMode = darkMode;
		root.isRunOnStartup = startOnStartup;
		root.isShowNotifications = showPopup;
		root.isUseSoundAlert = soundAlert;
		root.isAuthenticated = authenticated;

		if(root.isDarkMode){
			root.Material.theme = Material.Dark;
			root.bannerImage = "qrc:/banner-light.png";
		}else{
			root.Material.theme = Material.Light;
			root.bannerImage = "qrc:/banner-dark.png";
		}

		root.showAuthPage = root.showSettingsPage = root.showAboutPage = false;
		if(root.isAuthenticated){
			root.showSettingsPage = true;
		}else{
			root.showAuthPage = true;
		}
	}
    }
}
