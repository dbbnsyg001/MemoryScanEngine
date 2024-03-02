#pragma once
#include "SystemMemory.h"
#include <string>
//扫描处理类
class Scanner
{
public:
    //单次搜索的设置
    struct Settings
    {
        int alignment;
        ValueType value_type;
        CompareType compare_type;
    };
public:
    Scanner(size_t process_id);
    ~Scanner();
    //清空结果
    void Clear();
    // 回收空间
    void Shrink();
    //结果条数
    size_t Size();
    //搜索
    void Find(const std::string& value);
    //获取搜索的设置
    Settings& GetSettings();
    //获取扫描结果
    const std::vector<MemoryResult>& Getresults();
private:
    void FindFirst(const std::string& value);
    void FindNext(const std::string& value);
private:
    //扫描结果
    std::vector<MemoryResult> m_results;
    //单次搜索的设置
    Settings  m_settings;
};