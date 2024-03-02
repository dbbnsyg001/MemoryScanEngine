#include "IWidgetDragShadow.h"
#include <QMouseEvent>
#include <QEvent>
#include <QPainter>
//
#include <windows.h>
#include <windowsx.h>

IWidgetDragShadow::IWidgetDragShadow()
{
}

IWidgetDragShadow::~IWidgetDragShadow()
{
}

void IWidgetDragShadow::setAllowDrag(bool set)
{
    this->m_allowDrag = set;
}

bool IWidgetDragShadow::allowDrag() const
{
    return m_allowDrag;
}

void IWidgetDragShadow::setShadow(bool shadow)
{
    if (m_shadow == shadow)
    {
        return;
    }

    m_refreshShadow = true;
    m_shadow = shadow;
    refreshFixedSize();
    m_thisWidget->repaint();
}

bool IWidgetDragShadow::shadow() const
{
    return m_shadow;
}

void IWidgetDragShadow::setResizeable(bool resizeable)
{
    this->m_resizeable = resizeable;
}

bool IWidgetDragShadow::resizeable() const
{
    return m_resizeable;
}

void IWidgetDragShadow::setThisWidget(QWidget* widget, QLayout* layout)
{
    this->m_thisWidget = widget;
    this->m_mainLayout = layout;
}

void IWidgetDragShadow::draggableMousePressEvent(QMouseEvent* event)
{
    if (m_allowDrag) // 允许拖拽
    {
        if (!event->isAccepted())
        {
            Q_OS_WIN
            ReleaseCapture();
            SendMessage(reinterpret_cast<HWND>(m_thisWidget->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
}


bool IWidgetDragShadow::draggableNativeEvent(const QByteArray& eventType, void* message, long* result)
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
        {
            *result = 0;
            // border宽度如果设置了shadow应该加上 m_shadowMargin
            LONG borderm_widthm_left = m_borderWidth, borderm_widthm_top = m_borderWidth, borderm_widthm_right = m_borderWidth, borderm_widthm_bottom = m_borderWidth;

            if (m_shadowVisibleEdges & Qt::TopEdge)
            {
                borderm_widthm_top = m_borderWidth + ((m_shadow && m_thisWidget->windowState() != Qt::WindowMaximized) ? m_shadowMargin : 0);
            }

            if (m_shadowVisibleEdges & Qt::BottomEdge)
            {
                borderm_widthm_bottom = m_borderWidth + ((m_shadow && m_thisWidget->windowState() != Qt::WindowMaximized) ? m_shadowMargin : 0);
            }

            if (m_shadowVisibleEdges & Qt::LeftEdge)
            {
                borderm_widthm_left = m_borderWidth + ((m_shadow && m_thisWidget->windowState() != Qt::WindowMaximized) ? m_shadowMargin : 0);
            }

            if (m_shadowVisibleEdges & Qt::RightEdge)
            {
                borderm_widthm_right = m_borderWidth + ((m_shadow && m_thisWidget->windowState() != Qt::WindowMaximized) ? m_shadowMargin : 0);
            }

            RECT winrect;
            GetWindowRect(HWND(m_thisWidget->winId()), &winrect);
            long x = GET_X_LPARAM(msg->lParam);
            long y = GET_Y_LPARAM(msg->lParam);

            if (m_resizeable)
            {
                bool resizeWidth = m_thisWidget->minimumWidth() != m_thisWidget->maximumWidth();
                bool resizeHeight = m_thisWidget->minimumHeight() != m_thisWidget->maximumHeight();

                if (resizeWidth)
                {
                    //left border
                    if (x >= winrect.left && x < winrect.left + borderm_widthm_left)
                    {
                        *result = HTLEFT;
                    }

                    //right border
                    if (x < winrect.right && x >= winrect.right - borderm_widthm_right)
                    {
                        *result = HTRIGHT;
                    }
                }

                if (resizeHeight)
                {
                    //bottom border
                    if (y < winrect.bottom && y >= winrect.bottom - borderm_widthm_bottom)
                    {
                        *result = HTBOTTOM;
                    }

                    //top border
                    if (y >= winrect.top && y < winrect.top + borderm_widthm_top)
                    {
                        *result = HTTOP;
                    }
                }

                if (resizeWidth && resizeHeight)
                {
                    //bottomLeftEdge corner
                    if (x >= winrect.left && x < winrect.left + borderm_widthm_left &&
                        y < winrect.bottom && y >= winrect.bottom - borderm_widthm_bottom)
                    {
                        *result = HTBOTTOMLEFT;
                    }

                    //bottomRightEdge corner
                    if (x < winrect.right && x >= winrect.right - borderm_widthm_right &&
                        y < winrect.bottom && y >= winrect.bottom - borderm_widthm_bottom)
                    {
                        *result = HTBOTTOMRIGHT;
                    }

                    //topLeftEdge corner
                    if (x >= winrect.left && x < winrect.left + borderm_widthm_left &&
                        y >= winrect.top && y < winrect.top + borderm_widthm_top)
                    {
                        *result = HTTOPLEFT;
                    }

                    //topRightEdge corner
                    if (x < winrect.right && x >= winrect.right - borderm_widthm_right &&
                        y >= winrect.top && y < winrect.top + borderm_widthm_top)
                    {
                        *result = HTTOPRIGHT;
                    }
                }
            }

            //MTm_QDEBUG << "[result]" << *result;

            if (0 != *result) return true;

            return false;
        } //end case WMm_NCHITTEST
    }

    return false;
}

void IWidgetDragShadow::refreshFixedSize()
{
    // 重置为默认大小
    if ((!m_invokeRefreshFixedSize) || (m_invokeRefreshFixedSize && !m_shadow))
    {
        m_mainLayout->setContentsMargins(0, 0, 0, 0);

        if (m_shadowFixedHeight != 0)
        {
            m_thisWidget->setFixedHeight(m_shadowFixedHeight);
        }

        if (m_shadowFixedWidth != 0)
        {
            m_thisWidget->setFixedWidth(m_shadowFixedWidth);
        }
    }
    else
    {
        int leftMargin = 0, topMargin = 0, rightMargin = 0, bottomMargin = 0;

        if (m_shadowVisibleEdges & Qt::TopEdge)
        {
            topMargin = m_shadowMargin;
        }

        if (m_shadowVisibleEdges & Qt::BottomEdge)
        {
            bottomMargin = m_shadowMargin;
        }

        if (m_shadowVisibleEdges & Qt::LeftEdge)
        {
            leftMargin = m_shadowMargin;
        }

        if (m_shadowVisibleEdges & Qt::RightEdge)
        {
            rightMargin = m_shadowMargin;
        }

        m_mainLayout->setContentsMargins(leftMargin, topMargin, rightMargin, bottomMargin);
        //
        int resizeH = 0, reisizeW = 0;

        if (m_shadowVisibleEdges & Qt::TopEdge)
        {
            resizeH += m_shadowMargin;
        }

        if (m_shadowVisibleEdges & Qt::BottomEdge )
        {
            resizeH += m_shadowMargin;
        }

        if (m_shadowVisibleEdges & Qt::LeftEdge)
        {
            reisizeW += m_shadowMargin;
        }

        if (m_shadowVisibleEdges & Qt::RightEdge)
        {
            reisizeW += m_shadowMargin;
        }

        if (m_shadowFixedHeight != 0)
        {
            m_thisWidget->setFixedHeight(m_shadowFixedHeight + resizeH);
        }

        if (m_shadowFixedWidth != 0)
        {
            m_thisWidget->setFixedWidth(m_shadowFixedWidth + reisizeW);
        }
    }
}

void IWidgetDragShadow::shadowPaintEvent(QPaintEvent* event)
{
    if (!m_shadow)
    {
        return;
    }

    QPainter painter(m_thisWidget);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    int borderm_radius = 4;
    QColor colorBegin(Qt::transparent);
    QColor colorEnd(0, 0, 0, 60);
    double colorBeginPos = 0.3;

    //绘制矩形
    if (m_shadowVisibleEdges & Qt::TopEdge)
    {
        QLinearGradient linearGradient(0, 0, 0, m_shadowMargin + borderm_radius);
        linearGradient.setColorAt(colorBeginPos, colorBegin);
        linearGradient.setColorAt(1, colorEnd);
        painter.setBrush(QBrush(linearGradient));
        int padding = ((m_shadowVisibleEdges & Qt::LeftEdge) ? (m_shadowMargin + borderm_radius) : 0) +
                      ((m_shadowVisibleEdges & Qt::RightEdge) ? (m_shadowMargin + borderm_radius) : 0);
        painter.drawRect(m_shadowMargin + borderm_radius, 0,
                         m_thisWidget->width() - padding, m_shadowMargin + borderm_radius);
    }

    if (m_shadowVisibleEdges & Qt::BottomEdge)
    {
        QLinearGradient linearGradient( 0, m_thisWidget->height(), 0, m_thisWidget->height() - m_shadowMargin - borderm_radius);
        linearGradient.setColorAt(colorBeginPos, colorBegin);
        linearGradient.setColorAt(1, colorEnd);
        painter.setBrush(QBrush(linearGradient));
        int padding = ((m_shadowVisibleEdges & Qt::LeftEdge) ? (m_shadowMargin + borderm_radius) : 0) +
                      ((m_shadowVisibleEdges & Qt::RightEdge) ? (m_shadowMargin + borderm_radius) : 0);
        painter.drawRect(m_shadowMargin + borderm_radius, m_thisWidget->height() - m_shadowMargin - borderm_radius,
                         m_thisWidget->width() - padding, m_thisWidget->height());
    }

    if (m_shadowVisibleEdges & Qt::LeftEdge)
    {
        QLinearGradient linearGradient(0, 0, m_shadowMargin + borderm_radius, 0);
        linearGradient.setColorAt(colorBeginPos, colorBegin);
        linearGradient.setColorAt(1, colorEnd);
        painter.setBrush(QBrush(linearGradient));
        int padding = ((m_shadowVisibleEdges & Qt::TopEdge) ? (m_shadowMargin + borderm_radius) : 0) +
                      ((m_shadowVisibleEdges & Qt::BottomEdge) ? (m_shadowMargin + borderm_radius) : 0);
        painter.drawRect(0, m_shadowMargin + borderm_radius,
                         m_shadowMargin + borderm_radius, m_thisWidget->height() - padding);
    }

    if (m_shadowVisibleEdges & Qt::RightEdge)
    {
        QLinearGradient linearGradient(m_thisWidget->width(), 0, m_thisWidget->width() - m_shadowMargin - borderm_radius, 0);
        linearGradient.setColorAt(colorBeginPos, colorBegin);
        linearGradient.setColorAt(1, colorEnd);
        painter.setBrush(QBrush(linearGradient));
        int padding = ((m_shadowVisibleEdges & Qt::TopEdge) ? (m_shadowMargin + borderm_radius) : 0) +
                      ((m_shadowVisibleEdges & Qt::BottomEdge) ? (m_shadowMargin + borderm_radius) : 0);
        painter.drawRect(m_thisWidget->width() - m_shadowMargin - borderm_radius, m_shadowMargin + borderm_radius,
                         m_thisWidget->width(), m_thisWidget->height() - padding);
    }

    // 绘制边框扇形
    if ((m_shadowVisibleEdges & (Qt::TopEdge | Qt::LeftEdge)) == (Qt::TopEdge | Qt::LeftEdge))
    {
        QRadialGradient radialGradient(m_shadowMargin + borderm_radius, m_shadowMargin + borderm_radius, m_shadowMargin + borderm_radius, m_shadowMargin + borderm_radius, m_shadowMargin + borderm_radius);
        radialGradient.setColorAt(1 - colorBeginPos, colorBegin);
        radialGradient.setColorAt(0, colorEnd);
        painter.setBrush(QBrush(radialGradient));
        painter.drawPie(0, 0,
                        (m_shadowMargin + borderm_radius) * 2, (m_shadowMargin + borderm_radius) * 2, 16 * 90, 16 * 90);
    }

    if ((m_shadowVisibleEdges & (Qt::BottomEdge | Qt::LeftEdge)) == (Qt::BottomEdge | Qt::LeftEdge))
    {
        QRadialGradient radialGradient(m_shadowMargin + borderm_radius, m_thisWidget->height() - m_shadowMargin - borderm_radius, m_shadowMargin + borderm_radius, m_shadowMargin + borderm_radius, m_thisWidget->height() - m_shadowMargin - borderm_radius);
        radialGradient.setColorAt(1 - colorBeginPos, colorBegin);
        radialGradient.setColorAt(0, colorEnd);
        painter.setBrush(QBrush(radialGradient));
        painter.drawPie(0, m_thisWidget->height() - (m_shadowMargin + borderm_radius) * 2,
                        (m_shadowMargin + borderm_radius) * 2, (m_shadowMargin + borderm_radius) * 2, 16 * 180, 16 * 90);
    }

    if ((m_shadowVisibleEdges & (Qt::TopEdge | Qt::RightEdge)) == (Qt::TopEdge | Qt::RightEdge))
    {
        QRadialGradient radialGradient(m_thisWidget->width() - m_shadowMargin - borderm_radius, m_shadowMargin + borderm_radius, m_shadowMargin + borderm_radius, m_thisWidget->width() - m_shadowMargin - borderm_radius, m_shadowMargin + borderm_radius);
        radialGradient.setColorAt(1 - colorBeginPos, colorBegin);
        radialGradient.setColorAt(0, colorEnd);
        painter.setBrush(QBrush(radialGradient));
        painter.drawPie(m_thisWidget->width() - (m_shadowMargin + borderm_radius) * 2, 0,
                        (m_shadowMargin + borderm_radius) * 2, (m_shadowMargin + borderm_radius) * 2, 16 * 0, 16 * 90);
    }

    if ((m_shadowVisibleEdges & (Qt::BottomEdge | Qt::RightEdge)) == (Qt::BottomEdge | Qt::RightEdge))
    {
        QRadialGradient radialGradient(m_thisWidget->width() - m_shadowMargin - borderm_radius, m_thisWidget->height() - m_shadowMargin - borderm_radius, m_shadowMargin + borderm_radius, m_thisWidget->width() - m_shadowMargin - borderm_radius, m_thisWidget->height() - m_shadowMargin - borderm_radius);
        radialGradient.setColorAt(1 - colorBeginPos, colorBegin);
        radialGradient.setColorAt(0, colorEnd);
        painter.setBrush(QBrush(radialGradient));
        painter.drawPie(m_thisWidget->width() - (m_shadowMargin + borderm_radius) * 2, m_thisWidget->height() - (m_shadowMargin + borderm_radius) * 2,
                        (m_shadowMargin + borderm_radius) * 2, (m_shadowMargin + borderm_radius) * 2, 16 * 270, 16 * 90);
    }
}

void IWidgetDragShadow::shadowMouseReleaseEvent(QMouseEvent* event)
{
    if (!m_thisWidget->isVisible())
    {
        return ;
    }

    QRect rect = m_thisWidget->geometry();
    rect.adjust(m_shadowMargin, m_shadowMargin, -m_shadowMargin, -m_shadowMargin);

    // 如果在阴影区域，隐藏该控件
    if (!rect.contains(event->globalPos()))
    {
        m_thisWidget->hide();
    }
}

int IWidgetDragShadow::shadowMargin() const
{
    return m_shadowMargin;
}

void IWidgetDragShadow::setShadowMargin(int shadowMargin)
{
    m_shadowMargin = shadowMargin;

    if (m_shadow = true)
    {
        m_shadow = false;
        this->setShadow(true);
    }
}

Qt::Edges IWidgetDragShadow::shadowVisibleEdges() const
{
    return m_shadowVisibleEdges;
}

void IWidgetDragShadow::setShadowVisibleEdges(Qt::Edges shadowVisibleEdges)
{
    m_shadowVisibleEdges = shadowVisibleEdges;
    refreshFixedSize();
}
