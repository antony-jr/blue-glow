import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

Dialog {
    property ApplicationWindow mainWindow;
    width: 580
    height: 185
    Material.theme: Material.Light

    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height) / 2
    title: mainWindow.errorTitle
    visible: mainWindow.showErrorDialog
    contentItem: Text {
		Layout.preferredWidth: parent.width - 10
		text: mainWindow.errorMessage
		font.pixelSize: 14
		wrapMode: Text.WordWrap
    }
    standardButtons: StandardButton.Ok | StandardButton.Cancel
    onAccepted: {
	    mainWindow.showErrorDialog = false
    }
    onRejected: {
	    mainWindow.showErrorDialog = false
    }
}

 
