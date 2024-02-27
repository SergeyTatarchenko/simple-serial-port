/**
 * @file serial_port.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 *
 */

#include "../inc/serial_port.hpp"
#if defined(PLATFORM_LINUX)
#include <sys/types.h>
#include <dirent.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#include "serial_port.hpp"
#else
#error "target platform not defined."
#endif

SerialPort::SerialPort(std::string name)
{
    (void)open(name);
}

SerialPort::SerialPort(std::string name, sp::PortConfig config)
{
    (void)open(name);
    (void)setup(config);
}

sp::PortErrors SerialPort::open(const std::string name)
{
    sp::PortErrors error = sp::PortErrors::no_error;
    try
    {
        port.openPort(name);
        state = sp::PortState::Open;
        path  = name;
    }
    catch(const std::system_error& e)
    {
        error = static_cast<sp::PortErrors>(e.code().value());
    }
    return error;
}

sp::PortErrors SerialPort::setup(sp::PortConfig config)
{
    sp::PortErrors error = sp::PortErrors::no_error;
    try
    {
        port.setupPort(config);
        this->config = config;
    }
    catch(const std::system_error& e)
    {
        error = static_cast<sp::PortErrors>(e.code().value());
    }
    return error;
}

void SerialDevice::updateAvailableDevices()
{
    devices.clear();

    #if defined(PLATFORM_WINDOWS)
    
    #if defined(UNICODE)
    wchar_t dev_path[256];
    #else
    char dev_path[256];
    #endif

    for (auto i = 0; i < 255; ++i)
    {
        std::string device = "COM" + std::to_string(i);
        #if defined(UNICODE)
        std::wstring converted_device =  std::wstring(device.begin(),device.end());
        DWORD result = QueryDosDevice(converted_device.c_str(),dev_path, 256);
        #else
        DWORD result = QueryDosDevice(device.c_str(),dev_path, 256);
        #endif
        if (result != 0)
        {
            devices.push_back(device);
        }else{continue;}
    }
    #elif defined(PLATFORM_LINUX) 
    const char path[] = {"/dev/"};
    const std::string dev_template[] = {"ttyUSB","ttyACM"};
    dirent *dp;
    DIR *dirp;
    dirp = opendir(path);
    while ((dp = readdir(dirp)) != NULL)
    {
        size_t      tab_size = sizeof(dev_template)/sizeof(std::string);
        std::string device   = dp->d_name;
        
        for(size_t i = 0; i < tab_size; ++i)
        {
            int result = device.compare(0, dev_template[i].size(), dev_template[i]);
            if(result == 0)
            {
                devices.push_back(device);
            }else{continue;}
        }
    }
    (void)closedir(dirp);
    #endif
}
