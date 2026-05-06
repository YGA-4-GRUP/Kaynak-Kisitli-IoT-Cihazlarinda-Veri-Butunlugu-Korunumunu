simple TemperatureSensor
{
    parameters:
        string sourceId = default("TemperatureSensor-1");
        string destinationId = default("SmartAC-1");
        string algorithm = default("ChaCha20-Poly1305");
        int maxMessages = default(1000);
        int payloadSize = default(32);
        double sendInterval @unit(s) = default(0.1s);
        double baseTemperature = default(32);
    gates:
        output out;
}

simple SmartAC
{
    parameters:
        string expectedSourceId = default("TemperatureSensor-1");
        string algorithm = default("ChaCha20-Poly1305");
        string networkType = default("WPAN");
        bool authEnabled = default(true);
        bool nonceCheckEnabled = default(true);
        bool timestampCheckEnabled = default(true);
        bool sequenceCheckEnabled = default(true);
        double maxDelayMs = default(250);
        double timestampWindowSec = default(1.0);
    gates:
        input in;
}

simple MITMAttacker
{
    parameters:
        double forgedTemperature = default(18);
        int maxCaptured = default(16);
        double attackInterval @unit(s) = default(0.25s);
    gates:
        input in;
        output out;
}

simple MetricsCollector
{
    parameters:
        string resultDir = default("results");
}
