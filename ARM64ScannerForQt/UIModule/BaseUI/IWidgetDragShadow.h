#pragma once
#include <QWidget>
#include <QLayout>

/// <summary>
/// 带阴影拖拽弹窗
/// </summary>
class IWidgetDragShadow
{
public:
    IWidgetDragShadow();
    virtual ~IWidgetDragShadow();

    /// <summary>
    /// 设置拖拽
    /// </summary>
    void setAllowDrag(bool set);
    bool allowDrag() const;

    /// <summary>
    /// 设置窗体阴影，增加阴影会增加边距
    /// </summary>
    /// <param name="shadow"></param>
    void setShadow(bool shadow);
    bool shadow()const;

    /// <summary>
    /// 设置拖拽边框大小
    /// </summary>
    /// <param name="resizeable"></param>
    void setResizeable(bool resizeable);
    bool resizeable()const;

    /// <summary>
    /// 设置阴影边距
    /// </summary>
    /// <returns></returns>
    int shadowMargin()const;
    void setShadowMargin(int shadowMargin);

    /// <summary>
    /// 设置阴影在哪个方向可见
    /// </summary>
    /// <returns></returns>
    Qt::Edges shadowVisibleEdges()const;
    void setShadowVisibleEdges(Qt::Edges shadowVisibleEdges);
protected:
    /// <summary>
    /// 构造函数中进行设置，用于shadow展示
    /// setThisWidget(this,mainLayout);
    /// </summary>
    /// <param name="widget"></param>
    /// <param name="layout"></param>
    void setThisWidget(QWidget* widget, QLayout* layout);

    /// <summary>
    /// MousePressEvent
    /// 鼠标拖拽事件中引用该函数
    /// </summary>
    /// <param name="event"></param>
    void draggableMousePressEvent(QMouseEvent* event);

    /// <summary>
    /// NativeEvent
    /// 鼠标拖拽边框进行resize
    /// </summary>
    /// <param name="eventType"></param>
    /// <param name="message"></param>
    /// <param name="result"></param>
    bool draggableNativeEvent(const QByteArray& eventType, void* message, long* result);

    /// <summary>
    /// 根据m_refreshShadow和m_invokeRefreshFixedSize 去修正m_shadowMargin 导致的大小改变
    /// </summary>
    void refreshFixedSize();

    /// <summary>
    /// PaintEvent阴影绘制事件
    /// </summary>
    void shadowPaintEvent(QPaintEvent* event);

    /// <summary>3
    /// MouseReleaseEvent
    /// 鼠标点击阴影区域，那么该弹窗需隐藏
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="event"></param>
    /// <returns></returns>
    void shadowMouseReleaseEvent(QMouseEvent* event);
protected:
    QWidget* m_thisWidget = nullptr;
    QLayout* m_mainLayout = nullptr;
    bool m_allowDrag = false; // 是否可拖拽窗体
    //size
    bool m_resizeable = false;// 是否可拖拽边框
    int m_borderWidth = 5;    // 拖拽边框时的borde
    //shadow
    int m_shadowMargin = 30;  // 阴影边距
    bool m_shadow = false;    // 阴影
    bool m_refreshShadow = false; // 是否更新了m_shadow值
    int m_shadowFixedWidth = 0;    // 阴影宽度
    int m_shadowFixedHeight = 0;   // 阴影高度
    bool m_invokeRefreshFixedSize = true; //手动调用FixedSize
    Qt::Edges m_shadowVisibleEdges = { Qt::LeftEdge | Qt::TopEdge | Qt::RightEdge | Qt::BottomEdge }; // 阴影的显示方向
};

