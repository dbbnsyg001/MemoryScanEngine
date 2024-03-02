#include "IWidgetUtil.h"
#include <QApplication>
#include <QWindow>
#include <QScreen>

bool IWidgetUtil::IsInQtDesigner()
{
    return qApp->applicationDisplayName() == "Qt Designer";
}

QString IWidgetUtil::FormatColorToString(const QColor& color, bool bAlpha /*= false*/)
{
    int rgb[3] = { color.red(), color.green(), color.blue() };
    float alpha = color.alpha() * 1.0 / 254;
    QString colorStr;

    if (color.isValid())
    {
        if (true == bAlpha)
        {
            colorStr = QString("rgba(%1, %2, %3, %4)").arg(rgb[0]).arg(rgb[1]).arg(rgb[2]).arg(QString::number(alpha, 'f', 1));
        }
        else
        {
            colorStr = QString("rgb(%1, %2, %3)").arg(rgb[0]).arg(rgb[1]).arg(rgb[2]);
        }
    }
    else
    {
        colorStr = "transparent";
    }

    return colorStr;
}

QScreen* IWidgetUtil::GetWindowsScreen()
{
    QScreen* screen = nullptr;
    QWindow* window = QGuiApplication::focusWindow();

    if (window != nullptr)
    {
        screen = window->screen();
    }

    if (screen == nullptr)
    {
        screen = QGuiApplication::primaryScreen();
    }

    return screen;
}
