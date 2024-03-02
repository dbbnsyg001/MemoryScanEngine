#include "MainWidget.h"
#include <QRegExp>

MainWidget::MainWidget(QWidget* parent)
    : IDialogDragShadow(parent)
{
    ui.setupUi(this);
    this->SetMainLayout(ui.verticalLayout);
    InitUI();
}

MainWidget::~MainWidget()
{
}

void MainWidget::on_button_close_clicked()
{
    this->close();
}

void MainWidget::InitUI()
{
    auto SetRegexp = [&](QLineEdit * tempLineEdit, const QString & regexpStr)
    {
        QRegExp regex(regexpStr);

        if (!tempLineEdit->validator())
        {
            QRegExpValidator* pRep = new QRegExpValidator(regex, tempLineEdit);
            tempLineEdit->setValidator(pRep);
        }
    };
    // 设置IP地址/端口,正则表达式验证器
    ui.lineEdit_ip->setText("127.0.0.1");
    ui.lineEdit_port->setText("9898");
    SetRegexp(ui.lineEdit_ip, "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    SetRegexp(ui.lineEdit_port, "^([1-9]|[1-5][0-9]{1,4}|6[0-5][0-5][0-3][0-5])$");
}

void MainWidget::on_button_mini_clicked()
{
    this->showMinimized();
}
