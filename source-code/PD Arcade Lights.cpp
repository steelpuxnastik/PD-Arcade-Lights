//#include "pch.h"
#include <Windows.h>
#include "PluginConfigApi.h"
#include "framework.h"
#include <atomic>
#include <thread>
#include <iostream>
#include <fstream>

#define offset 0xc9

using namespace System;
using namespace System::IO::Ports;

uint32_t Rate;
uint8_t Selected_Port, Delay;
std::atomic<bool> g_running(false);
std::thread g_workerThread;
bool ignorePartitionLights = false;

void createDefaultConfig()
{
    WritePrivateProfileStringW(L"general", L"Selected_Port", L"8", CONFIG_FILE);
    WritePrivateProfileStringW(L"general", L"Rate", L"38400", CONFIG_FILE);
    WritePrivateProfileStringW(L"general", L"Delay", L"1", CONFIG_FILE);
    WritePrivateProfileStringW(L"general", L"ignorePartitionLights", L"0", CONFIG_FILE);
}

void loadConfig()
{
    std::ifstream file(CONFIG_FILE);
    if (!file.good())
    {
        std::wcout << L"Config file not found. Creating default config file." << std::endl;
        createDefaultConfig();
    }
    file.close();

    Selected_Port = GetPrivateProfileIntW(L"general", L"Selected_Port", 8, CONFIG_FILE);
    Rate = GetPrivateProfileIntW(L"general", L"Rate", 38400, CONFIG_FILE);
    Delay = GetPrivateProfileIntW(L"general", L"Delay", 1, CONFIG_FILE);
    ignorePartitionLights = GetPrivateProfileIntW(L"general", L"ignorePartitionLights", 0, CONFIG_FILE) > 0 ? true : false;
}

void workerThread()
{
    int sides, buttons, ob = 0, os = 0;
    HWND hwnd = nullptr;
    SerialPort^ g_port = nullptr;

    while (!hwnd && g_running)
    {
        hwnd = FindWindowA(NULL, "Hatsune Miku Project DIVA Arcade Future Tone");
        if (!hwnd) std::this_thread::sleep_for(std::chrono::seconds(6));
    }

    if (!g_running) return;

    try
    {
        String^ portName = String::Format("COM{0}", Selected_Port);
        g_port = gcnew SerialPort(portName, Rate);
        g_port->Open();
        Console::WriteLine("[PD Arcade Lights] connected");

        const int BUFFER_SIZE = 2;
        array<System::Byte>^ buffer = gcnew array<System::Byte>(BUFFER_SIZE);
        int bufferIndex = 0;
        array<System::Byte>^ outOn = { 0b00111111,0b11111111 };
        g_port->Write(outOn, 0, 2);

        while (g_running && g_port->IsOpen)
        {
            if (!g_running) {
                return;
            }
            memcpy((void*)&buttons, (LPVOID)0x00014119B950, sizeof(buttons));
            memcpy((void*)&sides, (LPVOID)0x000140EDA330, sizeof(sides));
            sides = sides + offset;
            memcpy((void*)&sides, (LPVOID)sides, sizeof(sides));

            sides = (sides & 0b00111111);
            buttons = (buttons | 0b11000000);

            if (os != sides)
            {
                buffer[bufferIndex++] = (System::Byte)sides;
                os = sides;
            }
            if (ob != buttons)
            {
                buffer[bufferIndex++] = (System::Byte)buttons;
                ob = buttons;
            }
            if (bufferIndex > 0)
            {
                g_port->Write(buffer, 0, bufferIndex);
                bufferIndex = 0;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(Delay));
        }
    }
    catch (Exception^ ex)
    {
        Console::WriteLine("[PD Arcade Lights] Error: {0}", ex->Message);
    }
    finally
    {
        if (g_port != nullptr && g_port->IsOpen)
        {
            array<System::Byte>^ outOff = { 0b00000000,0b11000000 };
            g_port->Write(outOff, 0, 2);
            g_port->Close();
            delete g_port;
            g_port = nullptr;
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        loadConfig();
        g_running = true;
        g_workerThread = std::thread(workerThread);
        break;
    case DLL_PROCESS_DETACH:
        g_running = false;
        if (g_workerThread.joinable())
        {
            g_workerThread.join();
        }
        break;
    }
    return TRUE;
}

void OpenWiki()
{
    ShellExecuteW(NULL, L"open", L"https://github.com/steelpuxnastik/PD-Arcade-Lights", NULL, NULL, SW_SHOW);
}

PluginConfig::PluginConfigOption config[] = {
    { PluginConfig::CONFIG_STRING, new PluginConfig::PluginConfigStringData{ L"", L"", L"", L"This plugin is used to control LEDs in buttons and \nthe LED strip on sides of the arcade cabinet \nby the game itself to implement such feature into your \narcade controller. \nDo not use this plugin if you do not have such functionality.\nPress Help button for more info." } },
    { PluginConfig::CONFIG_SPACER, new PluginConfig::PluginConfigSpacerData{ 60 } },
    { PluginConfig::CONFIG_GROUP_START, new PluginConfig::PluginConfigGroupData{ L"Settings", 120 } },
    { PluginConfig::CONFIG_NUMERIC, new PluginConfig::PluginConfigNumericData{ L"Selected_Port", L"general", CONFIG_FILE, L"COM Port", L"set port number of your receiver", 8, 1, 256} },
    { PluginConfig::CONFIG_DROPDOWN_NUMBER, new PluginConfig::PluginConfigDropdownNumberData{ L"Rate", L"general", CONFIG_FILE, L"Data rate", L"COM Port data rate", 38400, std::vector<int>({ 9600, 38400, 115200 }), false } },
    { PluginConfig::CONFIG_NUMERIC, new PluginConfig::PluginConfigNumericData{ L"Delay", L"general", CONFIG_FILE, L"Delay(ms)", L"Delay between scans (0-100ms)\n0 is not recommended", 1, 0, 100} },
    { PluginConfig::CONFIG_BOOLEAN, new PluginConfig::PluginConfigBooleanData{ L"ignorePartitionLights", L"general", CONFIG_FILE, L"Ignore partition lights output", L"Ignore partition lights output.\nOnly buttons light enabled if checked.", false } },
    { PluginConfig::CONFIG_GROUP_END, NULL },
    { PluginConfig::CONFIG_SPACER, new PluginConfig::PluginConfigSpacerData{ 10 } },
    { PluginConfig::CONFIG_BUTTON, new PluginConfig::PluginConfigButtonData{ L"Help", L"Get help on the ArcadeLight wiki.", OpenWiki } },
};

extern "C" __declspec(dllexport) LPCWSTR GetPluginName(void)
{
    return L"ArcadeLights";
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginDescription(void)
{
    return  L"ArcadeLights Plugin by steelpuxnastik and toha-cat\n\nEnables sending arcade cabinet lights data by COM-port to receiver that will show you lights";
}

extern "C" __declspec(dllexport) PluginConfig::PluginConfigArray GetPluginOptions(void)
{
    return PluginConfig::PluginConfigArray{ _countof(config), config };
}