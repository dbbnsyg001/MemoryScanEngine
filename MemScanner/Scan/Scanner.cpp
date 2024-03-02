#include "Scanner.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <cstring>
#include <cmath>

const size_t kBufferSize = 4096 * 32;

Scanner::Scanner(size_t process_id) : m_settings{4, ValueType::Int32, CompareType::Equal}
{
    if (!sys_open_process(process_id))
    {
        throw std::runtime_error("failed to open process");
    }

    m_results.reserve(10000000);
}

Scanner::~Scanner()
{
    sys_close_process();
}

const std::vector<MemoryResult>& Scanner::Getresults()
{
    return m_results;
}

void Scanner::Clear()
{
    m_results.clear();
}

void Scanner::Shrink()
{
    m_results.shrink_to_fit();
}

size_t Scanner::Size()
{
    return m_results.size();
}

struct UnknownValue
{
    template<typename T>
    constexpr bool operator()(const T&, const T&) const
    {
        return true;
    }
};

struct IncreasedValue
{
    template<typename T>
    constexpr bool operator()(const T& left, const T& right) const
    {
        return std::greater<>()(left, right);
    }
};

struct DecreasedValue
{
    template<typename T>
    constexpr bool operator()(const T& left, const T& right) const
    {
        return std::less<>()(left, right);
    }
};

struct UnchangedValue
{
    template<typename T>
    constexpr bool operator()(const T& left, const T& right) const
    {
        return std::equal_to<>()(left, right);
    }
};

struct ChangedValue
{
    template<typename T>
    constexpr bool operator()(const T& left, const T& right) const
    {
        return !std::equal_to<>()(left, right);
    }
};

template<typename Type, typename Compare = std::equal_to<>>
struct FindInBuffer
{
    static void find_in_buffer(uint8_t* begin, uint8_t* buffer_begin, uint8_t* buffer_end, int alignment, void* value, std::vector<MemoryResult>& m_results)
    {
        Type val = *reinterpret_cast<Type*>(value);

        for (uint8_t* i = buffer_begin; i < buffer_end; i += alignment)
        {
            if (Compare()(*reinterpret_cast<Type*>(i), val))
            {
                m_results.emplace_back(begin + (i - buffer_begin), *reinterpret_cast<DataValue*>(i));
            }
        }
    }
    static void find_in_buffer_prev(uint8_t*, uint8_t*, uint8_t*, int, void*, std::vector<MemoryResult>&) {}
};

template<>
struct FindInBuffer<std::string>
{
    static void find_in_buffer(uint8_t* begin, uint8_t* buffer_begin, uint8_t* buffer_end, int alignment, void* value, std::vector<MemoryResult>& m_results)
    {
        std::string& val = *reinterpret_cast<std::string*>(value);
        auto data = val.data();
        size_t size = val.size();

        for (uint8_t* i = buffer_begin; i < buffer_end; i += alignment)
        {
            if (std::memcmp(i, data, size) == 0)
            {
                m_results.emplace_back(begin + (i - buffer_begin));
            }
        }
    }
};

template<typename Type, typename Compare = std::equal_to<>>
struct FindNextInBuffer
{
    static std::vector<MemoryResult>::iterator find_in_buffer(uint8_t* begin, uint8_t* buffer, size_t buffer_size, std::vector<MemoryResult>::iterator result_it, std::vector<MemoryResult>::iterator result_end, void* value)
    {
        Type val = *reinterpret_cast<Type*>(value);

        for (; result_it != result_end; ++result_it)
        {
            size_t position = result_it->address - begin;

            if (position >= buffer_size)
            {
                break;
            }

            Type newval = *reinterpret_cast<Type*>(buffer + position);

            if (Compare()(newval, val))
            {
                result_it->value = *reinterpret_cast<DataValue*>(buffer + position);
            }
            else
            {
                result_it->address = nullptr;
            }
        }

        return result_it;
    }

    static std::vector<MemoryResult>::iterator find_in_buffer_prev(uint8_t* begin, uint8_t* buffer, size_t buffer_size, std::vector<MemoryResult>::iterator result_it, std::vector<MemoryResult>::iterator result_end, void* /*value*/)
    {
        for (; result_it != result_end; ++result_it)
        {
            size_t position = result_it->address - begin;

            if (position >= buffer_size)
            {
                break;
            }

            Type newval = *reinterpret_cast<Type*>(buffer + position);

            if (Compare()(newval, *reinterpret_cast<Type*>(&result_it->value)))
            {
                result_it->value = *reinterpret_cast<DataValue*>(buffer + position);
            }
            else
            {
                result_it->address = nullptr;
            }
        }

        return result_it;
    }
};

template<>
struct FindNextInBuffer<std::string>
{
    static std::vector<MemoryResult>::iterator find_in_buffer(uint8_t* begin, uint8_t* buffer, size_t buffer_size, std::vector<MemoryResult>::iterator result_it, std::vector<MemoryResult>::iterator result_end, void* value)
    {
        std::string& val = *reinterpret_cast<std::string*>(value);
        auto data = val.data();
        size_t size = val.size();

        for (; result_it != result_end; ++result_it)
        {
            size_t position = result_it->address - begin;

            if (position >= buffer_size)
            {
                break;
            }

            if (std::memcmp(buffer + position, data, size) != 0)
            {
                result_it->address = nullptr;
            }
        }

        return result_it;
    }
};

template<typename T, template<typename Type, typename Compare> class Func>
auto get_compare_method(CompareType compare_type)
{
    switch (compare_type)
    {
        case CompareType::Equal:
            return &Func<T, std::equal_to<>>::find_in_buffer;

        case CompareType::Less:
            return &Func<T, std::less<>>::find_in_buffer;

        case CompareType::Greater:
            return &Func<T, std::greater<>>::find_in_buffer;

        case CompareType::UnknownType:
        default:
            return &Func<int, UnknownValue>::find_in_buffer;

        case CompareType::Increased:
            return &Func<T, IncreasedValue>::find_in_buffer_prev;

        case CompareType::Decreased:
            return &Func<T, DecreasedValue>::find_in_buffer_prev;

        case CompareType::Unchanged:
            return &Func<T, UnchangedValue>::find_in_buffer_prev;

        case CompareType::Changed:
            return &Func<T, ChangedValue>::find_in_buffer_prev;
    }
}

template<template<typename Type, typename Compare> class Func>
auto parse_input(const std::string& input, DataValue& value, const Scanner::Settings& m_settings)
{
    switch (m_settings.value_type)
    {
        case ValueType::Int8:
            value.int8 = static_cast<int8_t>(std::stoi(input));
            return get_compare_method<int8_t, Func>(m_settings.compare_type);

        case ValueType::Int16:
            value.int16 = static_cast<int16_t>(std::stoi(input));
            return get_compare_method<int16_t, Func>(m_settings.compare_type);

        default:
        case ValueType::Int32:
            value.int32 = static_cast<int32_t>(std::stoi(input));
            return get_compare_method<int32_t, Func>(m_settings.compare_type);

        case ValueType::Int64:
            value.int64 = static_cast<int64_t>(std::stoll(input));
            return get_compare_method<int64_t, Func>(m_settings.compare_type);

        case ValueType::Float:
            value.float_value = std::stof(input);
            return get_compare_method<float, Func>(m_settings.compare_type);

        case ValueType::Double:
            value.double_value = std::stod(input);
            return get_compare_method<double, Func>(m_settings.compare_type);

        case ValueType::String:
            value.pointer = &const_cast<std::string&>(input);
            return &Func<std::string, std::equal_to<>>::find_in_buffer;
    }
}

size_t get_value_size(const std::string& input, const Scanner::Settings& m_settings)
{
    int alignment = m_settings.alignment;

    switch (m_settings.value_type)
    {
        case ValueType::Int8:
            return sizeof(int8_t) - alignment;

        case ValueType::Int16:
            return sizeof(int16_t) - alignment;

        default:
        case ValueType::Int32:
            return sizeof(int32_t) - alignment;

        case ValueType::Int64:
            return sizeof(int64_t) - alignment;

        case ValueType::Float:
            return sizeof(float) - alignment;

        case ValueType::Double:
            return sizeof(double) - alignment;

        case ValueType::String:
            return static_cast<size_t>(std::ceil(static_cast<float>(input.size()) / alignment) * alignment) - alignment;
    }
}

Scanner::Settings& Scanner::GetSettings()
{
    return m_settings;
}

void Scanner::FindFirst(const std::string& value)
{
    void(*compare_method)(uint8_t*, uint8_t*, uint8_t*, int, void*, std::vector<MemoryResult>&);
    DataValue dv;
    void* val = &dv;
    compare_method = parse_input<FindInBuffer>(value, dv, m_settings);

    if (m_settings.value_type == ValueType::String)
    {
        val = dv.pointer;
    }

    size_t value_size = get_value_size(value, m_settings);
    size_t buffer_step = kBufferSize - value_size;
    std::vector<uint8_t> buffer(kBufferSize);
    auto buffer_ptr = buffer.data();

    for (const auto& region : sys_memory_regions())
    {
        uint8_t* begin = region.begin;

        if (!sys_seek_memory(begin))
        {
            std::cout << "Unable to seek memory location " << static_cast<void*>(begin) << ", error " << sys_get_error() << '\n';
            continue;
        }

        while (begin < region.end)
        {
            size_t read, bytesToRead = std::min(kBufferSize, static_cast<size_t>(region.end - begin));
            bool success = sys_read_memory(begin, buffer_ptr, bytesToRead, &read);

            if (!success)
            {
                std::cout << "Failed to read value at " << static_cast<void*>(begin) << ", read " << read << '/' << bytesToRead << ", error " << sys_get_error() << '\n';
                break;
            }

            compare_method(begin, buffer_ptr, buffer_ptr + bytesToRead - value_size, m_settings.alignment, val, m_results);
            begin += buffer_step;
        }
    }
}

void Scanner::FindNext(const std::string& value)
{
    std::vector<MemoryResult>::iterator(*compare_method)(uint8_t*, uint8_t*, size_t, std::vector<MemoryResult>::iterator, std::vector<MemoryResult>::iterator, void*);
    DataValue dv;
    void* val = &dv;
    compare_method = parse_input<FindNextInBuffer>(value, dv, m_settings);

    if (m_settings.value_type == ValueType::String)
    {
        val = dv.pointer;
    }

    size_t value_size = get_value_size(value, m_settings);
    size_t buffer_step = kBufferSize - value_size;
    auto result_it = m_results.begin();
    auto result_last = m_results.end();
    std::vector<uint8_t> buffer(kBufferSize);
    auto buffer_ptr = buffer.data();

    for (const auto& region : sys_memory_regions())
    {
        result_it = std::lower_bound(result_it, result_last, region.begin);

        if (result_it == result_last)  // reached the end
        {
            break;
        }

        auto result_end = std::lower_bound(result_it, result_last, region.end);

        if (result_it == result_end)  // no m_results in current region
        {
            continue;
        }

        uint8_t* begin = result_it->address;

        if (!sys_seek_memory(begin))
        {
            std::cout << "Unable to seek memory location " << static_cast<void*>(begin) << ", error " << sys_get_error() << '\n';
            continue;
        }

        while (begin < region.end && result_it != result_end)
        {
            size_t read, bytesToRead = std::min(kBufferSize, static_cast<size_t>(region.end - begin));

            if (!sys_read_memory(begin, buffer_ptr, bytesToRead, &read))
            {
                std::cout << "Failed to read value at " << static_cast<void*>(begin) << ", read " << read << '/' << bytesToRead << ", error " << sys_get_error() << '\n';
                break;
            }

            result_it = compare_method(begin, buffer_ptr, buffer_step, result_it, result_end, val);
            begin += buffer_step;
        }
    }

    m_results.erase(std::remove_if(m_results.begin(), m_results.end(), [](auto & x)
    {
        return x.address == nullptr;
    }), m_results.end());
}

void Scanner::Find(const std::string& value)
{
    if (m_results.size() == 0)
    {
        FindFirst(value);
    }
    else
    {
        FindNext(value);
    }
}
