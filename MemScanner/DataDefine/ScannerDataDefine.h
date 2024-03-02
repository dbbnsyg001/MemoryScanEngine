#pragma once
#include <vector>
//ֵ����
enum ValueType
{
    Int8,
    Int16,
    Int32,
    Int64,
    Float,
    Double,
    String
};

//ɨ������
enum CompareType
{
    Equal,
    Less,
    Greater,
    UnknownType,
    Increased,
    Decreased,
    Unchanged,
    Changed
};

//�ڴ�����
struct MemoryRegion
{
    //��ʼ����
    uint8_t* begin;
    //��������
    uint8_t* end;
    MemoryRegion(uint8_t* b, uint8_t* e) : begin(b), end(e) {}
};

//��������
union DataValue
{
    int8_t int8;
    int16_t int16;
    int32_t int32;
    int64_t int64;
    float float_value;
    double double_value;
    void* pointer;
};

//ɨ����
struct MemoryResult
{
    uint8_t* address;
    DataValue value;
    MemoryResult(uint8_t* adr) : address(adr) {}
    MemoryResult(uint8_t* adr, DataValue v) : address(adr), value(v) {}
    //���رȽ�
    friend bool operator<(const MemoryResult& element, uint8_t* value)
    {
        return element.address < value;
    }
};
