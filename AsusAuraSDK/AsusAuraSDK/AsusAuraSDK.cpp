// AuraTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "stdafx.h"
#import "libid:F1AA5209-5217-4B82-BA7E-A68198999AFA"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

using namespace AuraServiceLib;

int main()
{
    HRESULT hr;

    // Initialize COM
    hr = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (SUCCEEDED(hr))
    {
        IAuraSdk2Ptr sdk = nullptr;
        hr = sdk.CreateInstance(__uuidof(AuraSdk), nullptr, CLSCTX_INPROC_SERVER);

        // Acquire control
        sdk->SwitchMode();

        // Enumerate all devices
        IAuraSyncDeviceCollectionPtr devices;
        //const unsigned long enumType = 0;
        devices = sdk->Enumerate(0);

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, 0xFFFFFF); // define the range

        // Traverse all devices
        const int DEVICE_COUNT = devices->Count;
        for (int k = 0; k < 1000; k++)
        {
            for (int i = 0; i < DEVICE_COUNT; i++)
            {
                IAuraSyncDevicePtr dev = devices->Item[i];
                IAuraRgbLightCollectionPtr lights = dev->Lights;

                const int LIGHT_COUNT = lights->Count;
                for (int j = 0; j < LIGHT_COUNT; j++)
                {
                    IAuraRgbLightPtr light = lights->Item[j];

                    // Set all LED's to random
                    light->Color = distr(gen);
                    printf("i:%d j:%d \n", i, j);
                }
                // Apply colors that we have just set
                dev->Apply();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        sdk->ReleaseControl(0);
    }

    // Uninitialize COM
    ::CoUninitialize();

    return 0;

}