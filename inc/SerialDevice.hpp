/**
 * @file SerialDevice.hpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 *
 */

#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include <cstdint>
#include <string>
#include <vector>

enum class PortState
{
    Open,
    Close
};

enum class PortBaudRate
{
    BD_9600,
    BD_19200,
    BD_38400,
    BD_57600,
    BD_115200
};

enum class PortDataBits
{
    Five,
    Six,
    Seven,
    Eight
};

enum class PortParity 
{
    None,
    Even,
    Odd
};

enum class PortStopBits
{
    One,
    Two
};

struct PortConfig
{
    PortBaudRate baudrate   = PortBaudRate::BD_9600;
    PortDataBits data_bits  = PortDataBits::Eight;
    PortParity   parity     = PortParity::None;
    PortStopBits stop_bits  = PortStopBits::One;
    int          timeout_ms = 0;
};

class SerialPort
{
    public:
        /// @brief open port
        /// @param path string with path to device
        /// @retval actual port state
        virtual PortState openPort(const std::string& path)  = 0;
        /// @brief close actual port if opened
        virtual void closePort() = 0;
        /// @brief setup port with new configuration
        /// @param config struct with port configuration
        virtual void setup(const PortConfig& config) = 0;
        /// @brief write string data to actual port
        /// @param data string object with data to send
        virtual void writeString(const std::string& data) = 0;
        /// @brief write raw data to actual port
        /// @param data string object with data to send
        virtual void writeBinary(const std::vector<uint8_t>& data) = 0;
        /// @brief read raw data from port 
        /// @param data reference to vector with buffer for data
        /// @param length how many bytes we expect to read during timeout
        /// @returns how many bytes we read actually
        virtual size_t readBinary(std::vector<uint8_t>& data, size_t length) = 0;
        /// @brief get actual port state
        /// @returns enum class PortState 
        PortState getState() const {return state;}
    
    protected:
        /// @brief actual port state
        PortState state = PortState::Close;
};

class SerialDevice
{
    public:
        SerialDevice(){};
        ~SerialDevice(){};
        
        /// @brief request for all available serial port devices in system
        /// @param devices vector into which ports will be written (if exist)
        void GetListOfAvailableDevices(std::vector<std::string>& devices);
        /// @brief get list of created devices
        /// @param devices vector into which names will be written (if exist)
        void GetListOfCreatedDevices(std::vector<std::string>& devices);
        /// @brief create port instance
        /// @param dev string with port name (can be obtained in GetListOfAvailableDevices)
        /// @param config struct with port configuration
        /// @return pointer to created port in case of success, nullptr in case of fault
        SerialPort* CreatePortInstance(const std::string dev, const PortConfig& config);
    private:
        /// @brief vertor with actual list of created ports
        std::vector<std::string> devices;
        /// @brief vertor with pointers to created ports
        std::vector<SerialPort*> ports;
};

#endif /*SERIAL_DEVICE_H*/
