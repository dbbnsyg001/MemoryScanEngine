#pragma once
#include "SystemMemory.h"
#include <string>
//ɨ�账����
class Scanner
{
public:
    //��������������
    struct Settings
    {
        int alignment;
        ValueType value_type;
        CompareType compare_type;
    };
public:
    Scanner(size_t process_id);
    ~Scanner();
    //��ս��
    void Clear();
    // ���տռ�
    void Shrink();
    //�������
    size_t Size();
    //����
    void Find(const std::string& value);
    //��ȡ����������
    Settings& GetSettings();
    //��ȡɨ����
    const std::vector<MemoryResult>& Getresults();
private:
    void FindFirst(const std::string& value);
    void FindNext(const std::string& value);
private:
    //ɨ����
    std::vector<MemoryResult> m_results;
    //��������������
    Settings  m_settings;
};