import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

// import Core.Clipper 1.0

GridLayout {
	property ApplicationWindow mainWindow;
	property Button controlButton : finishBtn;

	visible: mainWindow.showAuthPage
	anchors.fill: parent
	columns: 2
	ColumnLayout {
		Layout.column: 0
		Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
		Image {
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter 
			cache: true
			fillMode: Image.PreserveAspectFit
			source: mainWindow.bannerImage
		}
	
		TextField {	
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
			id: finishBtn
			objectName: "authFinish"
			highlighted: true	
			property string loadText: qsTr("Authenticating... ")
			property string beforeText: qsTr("Finish")
			
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			Layout.preferredWidth: 200
			Layout.preferredHeight: 80
			text: beforeText
			Material.background: Material.Green
			onClicked: {
				finishBtn.text = loadText
			}
		}
	} // Close ColumnLayout
}

