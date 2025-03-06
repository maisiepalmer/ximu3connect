#pragma once

#include <JuceHeader.h>
#include <x-IMU3/Cpp/Ximu3.hpp>

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

private:
    //==============================================================================
    // Your private member variables go here...
    juce::TextButton button {"Click"};
    std::function<void(ximu3::XIMU3_InertialMessage)> inertialCallback;
    std::shared_ptr<ximu3::Connection> connection;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
