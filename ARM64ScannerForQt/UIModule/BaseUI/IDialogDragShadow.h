#pragma once
#include <QDialog>
#include "IWidgetDragShadow.h"

class IDialogDragShadow : public QDialog, public IWidgetDragShadow
{
    Q_OBJECT

public:
    IDialogDragShadow(QWidget* parent = Q_NULLPTR);
    ~IDialogDragShadow();
    /// <summary>
    /// 设置布局
    /// </summary>
    /// <param name="layout">The layout.</param>
    void SetMainLayout(QLayout* layout);
protected:
    /// <summary>
    /// window事件（拖拽 修改大小）
    /// </summary>
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result)override;
    /// <summary>
    /// 鼠标事件（窗体拖拽）
    /// </summary>
    virtual void mousePressEvent(QMouseEvent* event) override;
    /// <summary>
    /// 控制其他事件
    /// 1. darken widget 显隐
    /// </summary>
    /// <param name="event"></param>
    /// <returns></returns>
    virtual bool event(QEvent* event) override;
    /// <summary>
    /// 绘制事件
    /// 1. 阴影
    /// </summary>
    /// <param name="event"></param>
    /// <returns></returns>
    virtual void paintEvent(QPaintEvent* event) override;
};

