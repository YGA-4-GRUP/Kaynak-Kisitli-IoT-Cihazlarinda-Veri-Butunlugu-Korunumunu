import inet.node.inet.WirelessHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;

network AdvancedSecurityNetwork
{
    parameters:
        @display("bgb=800,400");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=100,50");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=100,150");
        }
        sensor: WirelessHost {
            @display("p=200,200;i=device/sensor");
        }
        mitm: WirelessHost {
            @display("p=400,200;i=device/attacker");
        }
        actuator: WirelessHost {
            @display("p=600,200;i=device/server");
        }
}
