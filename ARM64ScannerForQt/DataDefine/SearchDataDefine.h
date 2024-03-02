#pragma once
#include <QString>
enum EM_ScanType
{
    Unkown = 0,//未知数值
    Exact,//精确数值
    GreaterThan,//比..大的数值
    LessThan,//比..小的数值
    Between,//数值位于...之间
    Increased,//增加的数值
    Reduced,//减少的数值
    Change,//变动的数值
    UnChange,//未变动的数值
};
enum EM_ScanDataType
{
    FourBytes = 0,//4字节
    EightBytes,//8字节
    Float,//单浮点
    Double,//双浮点
};
/// <summary>
/// 搜索时的UI数据
/// </summary>
struct ST_ScanUIData
{
    bool isFirstScan = true; //是否第一次搜索
    bool isHexScan = false; //搜索是否为16进制
    EM_ScanType scanType;   //搜索类型
    EM_ScanDataType scanDataType;//数据类型
    QString scanMemTarget = "All";
    int64_t scanMemStart = 0x0000000000000000;
    int64_t scanMemEnd = 0x7fffffffffffffff;
};