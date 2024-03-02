#include "IDialogDragShadow.h"
#include <windows.h>

IDialogDragShadow::IDialogDragShadow(QWidget* parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

IDialogDragShadow::~IDialogDragShadow()
{
}

void IDialogDragShadow::SetMainLayout(QLayout* layout)
{
    IWidgetDragShadow::setThisWidget(this, layout);//阴影
    this->setShadow(true);
    this->setAllowDrag(true);
}

bool IDialogDragShadow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    //Workaround for known bug -> check Qt forum : https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
    MSG* msg = *reinterpret_cast<MSG**>(message);
#else
    MSG* msg = reinterpret_cast<MSG*>(message);
#endif

    switch (msg->message)
    {
        case WM_NCHITTEST:
            return IWidgetDragShadow::draggableNativeEvent(eventType, message, result);

        default:
            return QDialog::nativeEvent(eventType, message, result);
    }
}

void IDialogDragShadow::mousePressEvent(QMouseEvent* event)
{
    QDialog::mousePressEvent(event);
    IWidgetDragShadow::draggableMousePressEvent(event);
}

bool IDialogDragShadow::event(QEvent* event)
{
    return QDialog::event(event);
}

void IDialogDragShadow::paintEvent(QPaintEvent* event)
{
    IWidgetDragShadow::shadowPaintEvent(event);
    return QDialog::paintEvent(event);
}
