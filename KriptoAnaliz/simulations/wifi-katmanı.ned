package simulations;

import inet.node.inet.WirelessHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;

network KriptoAgi
{
    parameters:
        @display("bgb=600,400");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=100,50");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=100,150");
        }
        sensor: WirelessHost {
            @display("p=250,200;i=device/sensor");
        }
        gateway: WirelessHost {
            @display("p=450,200;i=device/router");
        }
}