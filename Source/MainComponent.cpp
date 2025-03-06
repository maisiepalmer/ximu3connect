#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible (button);
    
    inertialCallback = [this](auto message)
    {

        const std::array<float, 3> gyro = {-message.gyroscope_x, -message.gyroscope_y, message.gyroscope_z};
        const std::array<float, 3> acc = {message.accelerometer_x, message.accelerometer_y, message.accelerometer_z};
        const std::array<float, 3> mag = {0.f, 0.f, 0.f}; // We don't have the magnetometer for these gloves
        
        std::cout << "message.gyroscope_x" << message.gyroscope_x << std::endl;
    };
    
    button.onClick = [this]()
    {
        const auto devices = ximu3::PortScanner::scanFilter(ximu3::XIMU3_ConnectionTypeUsb);
        if (devices.size() > 0)
        {
            std::cout << devices[0].device_name << " " << devices[0].serial_number << std::endl;
            
            const ximu3::ConnectionInfo& connectionInfo =  ximu3::UsbConnectionInfo(devices[0].usb_connection_info);
            
            connection = std::make_shared<ximu3::Connection>(connectionInfo);
            connection->addInertialCallback (inertialCallback);
            
            if (connection->open() != ximu3::XIMU3_ResultOk)
            {
                std::cout << "Unable to open " << connectionInfo.toString() << std::endl;
            }
        }
        else
        {
            std::cout << "No device connected" << std::endl;
        }
    };
    setSize (600, 400);
    
    
}

MainComponent::~MainComponent()
{
    if (connection != nullptr)
        connection->close();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto r = getLocalBounds();
    button.setBounds(r.reduced (20));
}
