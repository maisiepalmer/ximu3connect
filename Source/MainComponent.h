#pragma once

#include <JuceHeader.h>
#include <x-IMU3/Cpp/Ximu3.hpp>
#include "Ximu3DeviceManager.h"
#include "OSCManager.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void connectAndStrobe (const ximu3::ConnectionInfo& connectionInfo);

private:
    //==============================================================================
    // Your private member variables go here...
    TextButton buttonUsb {"Usb"};
    TextButton buttonWifi {"Wifi"};
    Ximu3DeviceManager ximu3DeviceManager;
    std::function<void(ximu3::XIMU3_InertialMessage)> inertialCallback;
    std::unique_ptr<ximu3::Connection> connection;
    
    OSCManager oscManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
