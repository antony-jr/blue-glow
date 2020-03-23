import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3

import Core.Backend 1.0

GridLayout {
	property ApplicationWindow mainWindow;
	property Button controlButton : finishBtn;
	property Backend backend;

	visible: mainWindow.showAuthPage
	anchors.fill: parent
	columns: 2
	ColumnLayout {
		Layout.column: 0
		Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
		Image {
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter 
			Layout.preferredWidth: 230
			Layout.preferredHeight: 230
			cache: true
			fillMode: Image.PreserveAspectFit
			source: mainWindow.bannerImage
	
		}
		BusyIndicator {
			Layout.alignment: Qt.AlignHCenter
			visible: mainWindow.loading
			running: mainWindow.loading
		}
		TextField {
			visible: !mainWindow.loading
			id: tokenText
			objectName: "tokenValue"
			placeholderText: "GITHUB AUTHENTICATION TOKEN"
			Layout.preferredWidth: mainWindow.width - 100
			Layout.alignment: Qt.AlignHCenter
			horizontalAlignment: TextInput.AlignHCenter
			font.pixelSize: 20
			selectByMouse: true
		}
		
		Button {
			visible: !mainWindow.loading
			id: finishBtn
			objectName: "authFinish"
			highlighted: true	
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			Layout.preferredWidth: 200
			Layout.preferredHeight: 80
			text: qsTr("Finish")
			Material.background: Material.Green
			onClicked: {
				backend.setAuthToken(tokenText.text);
			}
		}
	} // Close ColumnLayout
}

