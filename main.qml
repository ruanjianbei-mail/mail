import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Window {
    id:main_window
    width: 1040
    height: 980
    color: "grey"
    visible: true
    title: qsTr("Mail--")
    ListModel{
        id:tool_lm
        ListElement{ name: qsTr("Inbox")}
        ListElement{ name: qsTr("Starred") }
        ListElement{ name: qsTr("Sent") }
        ListElement{ name: qsTr("Drafts") }
    }
    Rectangle{
        id:window_bg
        width: main_window.width
        height: main_window.height
        //anchors.fill:parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "steelblue" }
        }
    }
    Rectangle{
        id:mail_message
        width: 200
        height: 150
        anchors.bottom: tool_lv.top
        //
        color: "red"
        Rectangle{
           id:msg_tx_bg
           width: 200
           height: 100
           anchors.bottom: parent.bottom
           Text {
               id:msg_text
               anchors.centerIn: parent
               text: qsTr("m19132023055@163.com")
           }
        }

    }
    Rectangle{
        color:"transparent"
        id:tool_lv
        width: 200
        height: main_window.height-mail_message.height
        anchors.top: mail_message.bottom
        anchors.left: main_window.left
        ListView{
            anchors.fill: parent
            opacity: 1
            visible: true
            model: tool_lm
            delegate:ColumnLayout{
                Rectangle{
                    id:tool
                    radius: 20
                    color:tool_ma.containsMouse?"lightslategrey":"transparent"
                    Behavior on color{
                        ColorAnimation {
                            duration: 200
                        }
                    }
                    width: 200
                    height: 50
                    Text{
                        anchors.centerIn: parent
                        text: name
                    }
                    MouseArea{
                        id:tool_ma
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }
            }
        }
    }
    Rectangle{
        id:content
        width: main_window.width-tool_lv.width
        height: main_window.height
        color: "white"
        anchors.left: tool_lv.right
        anchors.right: main_window.right
    }
    Rectangle{
        id:seeting
        Image {
            id: name
            source: "file"
        }
    }
}
