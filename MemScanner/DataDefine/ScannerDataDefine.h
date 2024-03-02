#pragma once
#include <vector>
//值类型
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

//扫描类型
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

//内存区域
struct MemoryRegion
{
    //开始区域
    uint8_t* begin;
    //结束区域
    uint8_t* end;
    MemoryRegion(uint8_t* b, uint8_t* e) : begin(b), end(e) {}
};

//数据类型
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

//扫描结果
struct MemoryResult
{
    uint8_t* address;
    DataValue value;
    MemoryResult(uint8_t* adr) : address(adr) {}
    MemoryResult(uint8_t* adr, DataValue v) : address(adr), value(v) {}
    //重载比较
    friend bool operator<(const MemoryResult& element, uint8_t* value)
    {
        return element.address < value;
    }
};
