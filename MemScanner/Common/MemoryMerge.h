#pragma once
#include "ScannerDataDefine.h"
//内存通用操作类
class MemoryOperate
{
public:
    //合并内存区域
    static void MergeMemoryRegions(std::vector<MemoryRegion>& regions);
};
