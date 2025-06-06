import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import writeMail 1.0
import "mail.js" as Controller
//写邮件
Item {
    property alias receiver:set_receiver
    property alias subject:set_subject
    property alias content:set_content
    height: 800
    width: 500
    //Layout.alignment:Qt.AlignHCenter
    RowLayout{
        Rectangle{
            width: 40
            height: 300
            color: "transparent"
        }

        ColumnLayout{
            //工具栏
            spacing: 20
            width: 400
            height: 800
            //上边距
            Rectangle{
                color:"transparent"
                radius: 10
                width: 400
                height: 10
            }
            //发送，附件等功能行
            RowLayout{
                //左边距
                Rectangle{
                    width: 30
                    height: 40
                }
                width: 500
                height: 200
                //发送
                Rectangle{
                    width: 80
                    height: 40
                    radius: 20
                    border.color: "lightsteelblue"
                    border.width: 1
                    Text{
                        text:qsTr("发送")
                        anchors.centerIn: parent
                    }
                    TapHandler{
                        onTapped: {
                            if (!write_mail.receiver){
                                write_mail.receiver=Controller.getreceiver()
                            }
                            write_mail.subject=Controller.getsubject()
                            write_mail.content=Controller.getcontent()
                            if(write_mail.attachments){
                                write_mail.sendWithAttachments(write_mail.receiver,write_mail.subject,write_mail.content,write_mail.attachments)
                            }
                            else write_mail.send(write_mail.receiver,write_mail.subject,write_mail.content)
                        }
                    }
                }
                //图片
                Rectangle{
                    width: 80
                    height: 40
                    radius: 20
                    /*border.color: "lightsteelblue"
                    border.width: 1
                    Text{
                        text:qsTr("图片")
                        anchors.centerIn: parent
                    }*/

                }
                //附件
                Rectangle{
                    width: 80
                    height: 40
                    radius: 20
                    border.color: "lightsteelblue"
                    border.width: 1
                    Text{
                        text:qsTr("附件")
                        anchors.centerIn: parent
                    }
                    TapHandler{
                        onTapped: {
                            file.open()
                        }
                    }
                    FileDialog{
                        id:file
                        onAccepted: {
                            var url=file.selectedFile
                            write_mail.attachments=url
                            if (!write_mail.receiver){
                                write_mail.receiver=Controller.getreceiver()
                            }
                            write_mail.subject=Controller.getsubject()
                            write_mail.content=Controller.getcontent()
                        }
                    }

                }
            }
            //收件人和输入框
            RowLayout{
                Text {
                    font.pointSize: 24
                    text: qsTr("收件人")
                }
                TextEdit{
                    id:set_receiver
                    text: write_mail.receiver ? write_mail.receiver : ""
                    width: 500
                    height: 200

                }
            }
            //边缘线条
            Rectangle{
                height: 2
                width: 500
                color:"lightsteelblue"
            }
            RowLayout{
                Text {
                    font.pointSize: 24
                    text: qsTr("主题")
                }
                TextEdit{
                    id:set_subject
                    width: 500
                    height: 200

                }
            }
            //边缘线条
            Rectangle{
                height: 2
                width: 500
                color:"lightsteelblue"
            }
            //正文
            ColumnLayout{
                Text {
                    font.pointSize: 24
                    text: qsTr("正文")
                }
                //边缘线条
                Rectangle{
                    height: 2
                    width: 500
                    color:"lightsteelblue"
                }
                Flickable {
                    width: 500
                    height: 400
                    contentWidth: set_content.width
                    contentHeight: set_content.height

                    TextEdit {
                        id: set_content
                        width: 500  // 固定宽度
                        height: Math.max(400, implicitHeight)  // 高度随内容增长，但至少400

                        wrapMode: TextEdit.Wrap  // 自动换行
                    }
                }
                //边缘线条
                Rectangle{
                    height: 2
                    width: 500
                    color:"lightsteelblue"
                }
            }
        }
    }
}

