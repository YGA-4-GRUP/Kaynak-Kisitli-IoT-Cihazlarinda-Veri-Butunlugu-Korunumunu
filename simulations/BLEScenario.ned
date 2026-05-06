network BLEScenario
{
    parameters:
        string algorithm = default("ChaCha20-Poly1305");
    types:
        channel BLEChannel extends ned.DatarateChannel
        {
            delay = 5ms;
            datarate = 1Mbps;
            ber = 0.000005;
        }
    submodules:
        sensor: TemperatureSensor {
            parameters:
                algorithm = parent.algorithm;
                sendInterval = 0.08s;
        }
        attacker: MITMAttacker {
            parameters:
                attackInterval = 0.20s;
        }
        ac: SmartAC {
            parameters:
                algorithm = parent.algorithm;
                networkType = "BLE";
                maxDelayMs = 180;
        }
        metrics: MetricsCollector;
    connections allowunconnected:
        sensor.out --> BLEChannel --> attacker.in;
        attacker.out --> BLEChannel --> ac.in;
}
