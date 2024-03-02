#pragma once
#include <iostream>
#include <chrono>
#include <string>
//��¼ɨ��ʱ��
class TimeWatch
{
public:
    TimeWatch(std::string&& name = "StopWatch");
    ~TimeWatch();
private:
    //scan name
    std::string m_name;
    //start time
    std::chrono::high_resolution_clock::time_point m_start;
};
