#include "TimeWatch.h"

TimeWatch::TimeWatch(std::string&& m_name) :
    m_name(std::move(m_name)),
    m_start(std::chrono::high_resolution_clock::now())
{
}

TimeWatch::~TimeWatch()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - m_start;

    if (duration > std::chrono::seconds(1))
    {
        std::cout << m_name << ": " << std::chrono::duration_cast<std::chrono::duration<double>>(duration).count() << " s\n";
    }
    else if (duration > std::chrono::milliseconds(1))
    {
        std::cout << m_name << ": " << std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration).count() << " ms\n";
    }
    else if (duration > std::chrono::microseconds(1))
    {
        std::cout << m_name << ": " << std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(duration).count() << " us\n";
    }
    else
    {
        std::cout << m_name << ": " << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() << " ns\n";
    }
}
