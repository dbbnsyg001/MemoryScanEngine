#pragma once

#include <QString>
#include <QScreen>
#include <QColor>

class IWidgetUtil
{
public:
    /// <summary>
    ///  判断是否在QtDesigner中
    /// </summary>
    /// <returns>是否在QtDesigner中</returns>
    static bool IsInQtDesigner();

    /// <summary>
    /// 格式化QColor转换成qss的字符串
    /// </summary>
    /// <param name="color">QColor</param>
    /// <param name="bAlpha">是否处理透明度</param>
    /// <returns></returns>
    static QString FormatColorToString(const QColor& color, bool bEnableAlpha = true);

    /// <summary>
    /// 返回QGuiApplication::allWindows()[0]->screen()，如果没有，返回QGuiApplication::primaryScreen()
    /// </summary>
    /// <returns></returns>
    static QScreen* GetWindowsScreen();
};

