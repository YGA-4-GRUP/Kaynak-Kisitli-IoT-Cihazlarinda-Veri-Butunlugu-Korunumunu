package simulations;

import inet.node.inet.SensorNode;
import inet.node.inet.WirelessHost;
import inet.networklayer.configurator.ipv6.Ipv6FlatNetworkConfigurator;
import inet.physicallayer.wireless.common.medium.RadioMedium;
import inet.visualizer.common.IntegratedVisualizer;

network BLEKriptoAgi
{
    parameters:
        @display("bgb=650,450");

    submodules:
        visualizer: IntegratedVisualizer {
            @display("p=50,50");
        }
        configurator: Ipv6FlatNetworkConfigurator { 
            @display("p=50,120");
        }

        radioMedium: RadioMedium {
            @display("p=50,190");
        }
        sensorNode: SensorNode {
            @display("p=200,250;i=device/sensor");
        } 
        gateway: WirelessHost {
            @display("p=400,250;i=device/router");
        }
        receiverNode: WirelessHost {
            @display("p=600,250;i=device/server");
        }
}