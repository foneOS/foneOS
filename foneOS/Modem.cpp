#include "stdafx.h"
#include "Modem.h"

static bool _enabled = false;

void Modem_Fake::Init()
{
    _enabled = true;
}

bool Modem_Fake::GetEnabled()
{
    return _enabled;
}

FoneOSString Modem_Fake::GetOperator()
{
    return STR("Fake Carrier");
}

int Modem_Fake::GetRSSI()
{
    return 20;
}

FoneOSString Modem_Fake::GetFWRev()
{
    return STR("FAKECO-1.2.3-456");
}

FoneOSString Modem_Fake::GetIMEI()
{
    return STR("123456789012345");
}

FoneOSString Modem_Fake::GetCCID()
{
    return STR("12345678901234567890");
}

void Modem_Fake::Cleanup()
{
    _enabled = false;
}

#ifdef PRODUCTION_BUILD
static bool _senabled = false;

void Modem_SIM800::Init()
{
    _enabled = true;
    this->_serial = mraa_uart_init(0);
    mraa_uart_set_baudrate(this->_serial, 9600);

    // TODO: Error handling
    while(mraa_uart_data_available(this->_serial, 0)) {this->Read();} // clear anything the modem sent on startup

    // get the auto-bauder started
    this->WriteLine("ATE0"); this->Read(); // disable echo of commands

    if (mraa_uart_data_available(this->_serial, 100))
    {
         this->Read(); // read extra echo
    }

    this->WriteLine("AT"); this->Read();

    this->WriteLine("AT"); this->Read();

    this->WriteLine("AT"); this->Read();


    while(mraa_uart_data_available(this->_serial, 100))
    {
        char deadBuf[2] = {0};
        mraa_uart_read(this->_serial, deadBuf, 1);
    } // clear responses

    if(!this->CheckReply("ATE0", "OK"))
    {
        // TODO: error handle
    }

    Logging::LogMessage(this->GetOperator());
}

void Modem_SIM800::WriteLine(std::string line)
{
    while(mraa_uart_data_available(this->_serial, 100))
    {
        char deadBuf[2] = {0};
        mraa_uart_read(this->_serial, deadBuf, 1);
    } // clear responses
    mraa_uart_write(this->_serial, line.c_str(), strlen(line.c_str()));
    mraa_uart_write(this->_serial, "\r\n", strlen("\r\n"));
    Logging::LogMessage(std::string("--> ") + line);
}

std::string Modem_SIM800::Read()
{
    std::string returnString = "";
    int ni = 0;
    while (true)
    {
        char miniBuf[2] = {0};
        mraa_uart_read(this->_serial, miniBuf, 1);
        if (miniBuf[0] == '\n')
        {
            if (ni >= 3)
            {
                break;
            }
            else
            {
                ni++; // Ignore the first 3 newlines.
                continue;
            }
        }
        if (miniBuf[0] != '\r')
        {
            returnString = returnString + std::string(miniBuf);
        }
        Utils::Delay(1);
    }
    Logging::LogMessage(std::string("<-- ") + returnString);
    return returnString;
}

bool Modem_SIM800::CheckReply(std::string line, std::string reply)
{
    this->WriteLine(line);
    return (this->Read() == reply);
}

bool Modem_SIM800::GetEnabled()
{
    return _enabled;
}

FoneOSString Modem_SIM800::GetOperator()
{
    this->WriteLine("AT+COPS?");
    FoneOSString op = FoneOSString(STR(this->Read().c_str()));
    this->Read(); // extra OK
    op = op.substr(op.find('"') + 1, op.length()-op.find('"')-2);
    return op;
}

int Modem_SIM800::GetRSSI()
{
    this->WriteLine("AT+CSQ");
    FoneOSString rssi = FoneOSString(STR(this->Read().c_str()));
    rssi = rssi.substr(rssi.find(' ') + 1, rssi.find(',')-rssi.find(' '));
    return atoi(rssi.c_str());
}

FoneOSString Modem_SIM800::GetFWRev()
{
    return STR("FAKECO-1.2.3-456");
}

FoneOSString Modem_SIM800::GetIMEI()
{
    return STR("123456789012345");
}

FoneOSString Modem_SIM800::GetCCID()
{
    return STR("12345678901234567890");
}

void Modem_SIM800::Cleanup()
{
    _senabled = false;
}

#endif
