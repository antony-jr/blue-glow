import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3


GridLayout {
	property ApplicationWindow mainWindow;

	visible: mainWindow.showAboutPage
	anchors.fill: parent
	anchors.top: parent.top
	anchors.left: parent.left
	anchors.right: parent.right
	anchors.bottom: parent.bottom
	columns: 1
	rows: 1
	ColumnLayout {
	      	Layout.preferredWidth: parent.width - 100
	    	Layout.preferredHeight: parent.height - 50
	    	Layout.row: 0
	    	Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
	    	Layout.topMargin: 10
		Button {
			id: backBtn
			objectName: "back"
			highlighted: true	
			
			Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
			Layout.preferredWidth: 185
			Layout.preferredHeight: 50
			text: qsTr("Go Back")
			Material.background: Material.Teal
			onClicked: {
				mainWindow.showSettingsPage = mainWindow.showAuthPage = mainWindow.showAboutPage = false;
				if(mainWindow.isAuthenticated){
					mainWindow.showSettingsPage = true;
				}else{
					mainWindow.showAuthPage = true;
				}
			}
		}
	
		RowLayout{
				Layout.alignment: Qt.AlignHCenter
				Image {
					cache: true
					fillMode: Image.PreserveAspectFit
					Layout.preferredWidth: 175
					Layout.preferredHeight: 175
					source: "qrc:/logo.png"
				}

		}

		Label {
			Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
			Layout.preferredWidth: parent.width - 100
			horizontalAlignment: Qt.AlignHCenter
			verticalAlignment: Qt.AlignTop
			text: qsTr("This program is licensed under the MIT License.<br>") +
			      qsTr("Copyright \u00A9 <b>Antony Jr</b>.<br>") +
			      qsTr("All Graphics Design by <b><a href=https://github.com/estatra>Atra</a></b>.<br>") + 
			      qsTr("This program uses modified version of bell logo from <b>Feather Icons</b>.<br>")
			font.pixelSize: 15
			wrapMode: Text.WordWrap
			textFormat: Text.RichText
			onLinkActivated: Qt.openUrlExternally(link)
		}
	   }
}
