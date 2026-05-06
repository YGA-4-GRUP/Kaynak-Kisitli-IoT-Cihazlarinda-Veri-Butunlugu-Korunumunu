package simulations;

import inet.node.inet.WirelessHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.wireless.common.medium.RadioMedium;

network WPANKriptoAgi
{
    parameters:
        @display("bgb=600,400");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=100,50");
        }
        radioMedium: RadioMedium {
            @display("p=100,150");
        }
        sensorNode: WirelessHost {
            @display("p=250,200;i=device/sensor");
        }
        gateway: WirelessHost{
            @display("p=450,200;i=device/router");}
}