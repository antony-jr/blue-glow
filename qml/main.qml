import QtQuick 2.12
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import "./Components" as Components

ApplicationWindow {
    id: root
    title: qsTr("Blue Glow")
    width: 700
    height: 450
    minimumWidth: 600
    minimumHeight: 400
    visible: true
    Material.theme: Material.Light 

    Component.onCompleted: {
	if(darkMode){
		Material.theme = Material.Dark;
	}

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
    property bool showAuthPage: true // Default page
    property bool showSettingsPage: false
    property bool showAboutPage: false
    property bool showErrorDialog: false;
    property bool darkMode: false;
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
    Components.AboutPage {
	id: aboutPage
	mainWindow: root
    } 
    
    Components.AuthPage {
        id: mainPage
	mainWindow: root
    }
    // -- End All Pages -- 
}
