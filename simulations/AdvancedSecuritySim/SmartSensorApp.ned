import inet.applications.contract.IApp;

simple SmartSensorApp like IApp {
    parameters:
        int localPort = default(-1);
        int destPort = default(5000);
        string destAddress;
        
        volatile double sendInterval @unit(s) = default(1s);
        
        // Simülasyon Gecikmeleri (Task Queue Parametreleri)
        double cryptoCpuDelay @unit(ms) = default(5ms);
        double senseDelay @unit(ms) = default(5ms);
        double actuateDelay @unit(ms) = default(0ms); // Sadece aktuatorde
        
        int packetSize @unit(B) = default(256B);
        string cryptoType = default("None");
        
        bool isActuator = default(false);
        
        @display("i=block/app");
        
    gates:
        input socketIn;
        output socketOut;
}
