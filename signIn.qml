import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import writeMail 1.0
import "mail.js" as Controller
Item {
    property alias qqid:set_username
    property alias password:set_password
    width: 400
    height: 400
    id:sign_in
    RowLayout{
        Rectangle{
            width: 40
            height: 400
            color: "transparent"
            }
        Rectangle{
            width: 400
            height: 400
            color: "transparent"
            //border.width: 2
            //border.color: "lightsteelblue"
            ColumnLayout{
                Rectangle{
                    width: 400
                    height: 40
                    color: "transparent"
                    }
                Text {
                    text: qsTr("邮箱")
                    font.pointSize: 24
                }
                TextField{
                    id:set_username
                    //placeholderText: qsTr("text")
                    validator: RegularExpressionValidator {
                            regularExpression: /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/
                        }
                }
                // 错误提示
                Label{
                    opacity: !set_username.acceptableInput
                    text: set_username.text ? "邮箱格式不正确" : "请输入账号"
                    color: "red"
                }
                Text {
                    text: qsTr("授权码")
                    font.pointSize: 24
                }
                TextField{
                    id:set_password
                    //placeholderText: qsTr("text")
                }
                // 错误提示
                Label{
                    //空白
                    opacity: !set_password.text
                    text:"请输入密码"
                    color: "red"
                }
                Label{
                    //空白
                }
                //提交
                Button{
                    text: qsTr("commit")
                    onClicked: {
                        if(qqid.text!==""){
                           write_mail.username=Controller.getusername()
                           write_mail.password=Controller.getpassword()
                           username.text=qqid.text}
                        content_loader.source="mailMsg.qml"
                    }
                }
            }
        }
    }
}
