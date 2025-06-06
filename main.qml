import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import writeMail 1.0
import mailUser 1.0
import "mail.js" as Controller
//主界面
ApplicationWindow {
    property alias content_loader:content_loader
    property alias write_mail:write_mail
    id:main_window
    WriteMail{
        id:write_mail
        username: "m19132023052@163.com"
        password: "CPdNrjbGEqnh43rd"
        receiver: "2448392399@qq.com"
        Component.onCompleted: console.log("succssed")
    }
    MailUser{
        id:mail_user
        username:write_mail.username
        password:write_mail.password
    }

    width: 1040
    height: 980
    color: "grey"
    visible: true
    title: qsTr("Mail")
    //背景颜色
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
    //整个窗口布局
    RowLayout{
        anchors.fill: parent
        width: 1040
        height:980
        spacing: 0
        //侧边栏
        ColumnLayout{
            Layout.margins: 10
            width: 180
            height: 980
            //图标
            Image {
                width: 180
                source: "logo.png"
                fillMode: Image.PreserveAspectFit
                //Component.onCompleted: console.log("Actual size:", width, height)
            }
            //写信功能
            Rectangle{
                id:mail_message
                width: 180
                height: 80
                color:"transparent"
                Rectangle{
                   id:msg_tx_bg
                   width: 180
                   height: 60
                   radius: 20
                   Text {
                       id:msg_text
                       anchors.centerIn: parent
                       text: qsTr("Compose")
                   }
                   MouseArea{
                       anchors.fill: parent
                       hoverEnabled: true
                       onClicked: {
                               content_loader.source="writeMail.qml"
                       }
                   }
                }
            }
            //工具栏模型
            ListModel{
                id:tool_lm
                ListElement{ name: qsTr("Inbox")}
                ListElement{ name: qsTr("Starred") }
                ListElement{ name: qsTr("Sent") }
                ListElement{ name: qsTr("Drafts") }
            }
            //工具栏视图
            Rectangle{
                color:"transparent"
                id:tool_lv
                width: 180
                height: 980-mail_message.height
                ListView{
                    anchors.fill: parent
                    opacity: 1
                    visible: true
                    spacing: 10
                    model: tool_lm
                    delegate:ColumnLayout{
                        Rectangle{
                            id:tool
                            width: 180
                            height: 50
                            radius: 20
                            color:tool_ma.containsMouse?"lightslategrey":"transparent"
                            Behavior on color{
                                ColorAnimation {
                                    duration: 200
                                }
                            }
                            Text{
                                anchors.centerIn: parent
                                text: name
                            }
                            MouseArea{
                                id:tool_ma
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    if(index==0)
                                        content_loader.source="mailMsgs.qml"
                                    if(index==1)
                                        content_loader.source="mailMsg.qml"

                                }
                            }
                        }
                    }
                }
            }
        }
        //邮件内容,登陆
        ColumnLayout{
            id:content_bg
            Layout.fillWidth:true
            height: main_window.height
            //
            RowLayout{
                Layout.preferredWidth: main_window.width-tool_lv.width
                height: 48
                //搜索框
                Rectangle{
                    width: 400
                    height: 35
                    radius: 50
                    //color: "transparent"
                }
                RowLayout{
                    width: 200
                    height: 48
                    Rectangle{
                        width: 45
                        height: 45
                        radius: 50
                        color: "grey"
                    }
                    Text {
                        id:username
                        width: 100
                        text: write_mail.username ? write_mail.username : qsTr("点击登录")
                    }
                    TapHandler{
                        onTapped: {
                            content_loader.source="signIn.qml"
                        }
                    }
                }
                Rectangle{
                    width: 70
                    height: 45
                    radius: 50
                    color: "transparent"
                }

            }

            Rectangle{
                id:content
                Layout.fillWidth:true
                radius: 20
                Layout.preferredWidth: main_window.width-tool_lv.width
                height: main_window.height
                color: "white"
                Loader {
                    id:content_loader
                    source: "mailMsgs.qml"
                }
            }
        }
        Rectangle{
            height: main_window.height
            width: 10
            color:"transparent"
        }
  }
}
