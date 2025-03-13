#include "MainComponent.h"
#include "Helpers.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible (buttonUsb);
    addAndMakeVisible (buttonWifi);
    
    inertialCallback = [this](auto message)
    {

        const std::array<float, 3> gyro = {-message.gyroscope_x, -message.gyroscope_y, message.gyroscope_z};
        const std::array<float, 3> acc = {message.accelerometer_x, message.accelerometer_y, message.accelerometer_z};
        
        // send osc message
        oscManager.sendMessage(acc, gyro);
        
        //std::cout << "message.gyroscope_x" << message.gyroscope_x << std::endl;
    };
    
    buttonUsb.onClick = [this]()
    {
        connection.reset();
        
        const auto devices = ximu3::PortScanner::scan();
        if (devices.size() == 0)
        {
            std::cout << "No devices found" << std::endl;
            return;
        }
        if (devices.size() > 1)
        {
            std::cout << "Error expecting to find only 1 device" << std::endl;
        }
        
        const ximu3::ConnectionInfo& connectionInfo = ximu3::UsbConnectionInfo (devices[0].usb_connection_info);
        
        connectAndStrobe (connectionInfo);
    };
    
    buttonWifi.onClick = [this]()
    {
        connection.reset();
        
        ximu3::NetworkAnnouncement networkAnnouncement;
        if (networkAnnouncement.getResult() != ximu3::XIMU3_ResultOk)
        {
            std::cout << "Unable to open network announcement socket" << std::endl;
            return;
        }
        
        const auto messages = networkAnnouncement.getMessagesAfterShortDelay();
        
        if (messages.size() == 0)
        {
            std::cout << "No devices found" << std::endl;
            return;
        }
        
        if (messages.size() > 1)
        {
            std::cout << "Error expecting to find only 1 device" << std::endl;
            return;
        }
        
        const auto& udpInfo = ximu3::XIMU3_network_announcement_message_to_udp_connection_info(messages[0]);
        const auto& connectionInfo = ximu3::UdpConnectionInfo (ximu3::UdpConnectionInfo (udpInfo));
        connectAndStrobe (connectionInfo);
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
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto r = getLocalBounds().reduced(20);
    buttonUsb.setBounds(r.removeFromLeft(r.getWidth()/2));
    buttonWifi.setBounds(r);
}

void MainComponent::connectAndStrobe (const ximu3::ConnectionInfo& connectionInfo)
{
    std::cout << "Conecting to " << connectionInfo.toString() << std::endl;
    
    connection = std::make_unique<ximu3::Connection>(connectionInfo);
    connection->addInertialCallback (inertialCallback);
    
    if (connection->open() != ximu3::XIMU3_ResultOk)
    {
        std::cout << "Unable to open " << connectionInfo.toString() << std::endl;
        return;
    }
    connection->sendCommands ({R"({"strobe":null})"}, 2, 500);
}
