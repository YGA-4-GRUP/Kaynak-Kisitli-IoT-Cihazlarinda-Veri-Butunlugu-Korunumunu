network WPANScenario
{
    parameters:
        string algorithm = default("ChaCha20-Poly1305");
    types:
        channel WPANChannel extends ned.DatarateChannel
        {
            delay = 12ms;
            datarate = 250kbps;
            ber = 0.00001;
        }
    submodules:
        sensor: TemperatureSensor {
            parameters:
                algorithm = parent.algorithm;
        }
        attacker: MITMAttacker;
        ac: SmartAC {
            parameters:
                algorithm = parent.algorithm;
                networkType = "WPAN";
                maxDelayMs = 300;
        }
        metrics: MetricsCollector;
    connections allowunconnected:
        sensor.out --> WPANChannel --> attacker.in;
        attacker.out --> WPANChannel --> ac.in;
}
