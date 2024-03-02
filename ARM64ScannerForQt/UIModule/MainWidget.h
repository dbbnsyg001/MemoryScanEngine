#pragma once

#include <QWidget>
#include "ui_MainWidget.h"
#include "IDialogDragShadow.h"

class MainWidget : public IDialogDragShadow
{
    Q_OBJECT

public:
    MainWidget(QWidget* parent = Q_NULLPTR);
    ~MainWidget();
private:
    /// <summary>
    /// 初始化UI
    /// </summary>
    void InitUI();
private slots:
    void on_button_close_clicked();
    void on_button_mini_clicked();
private:
    Ui::MainWidget ui;
};
