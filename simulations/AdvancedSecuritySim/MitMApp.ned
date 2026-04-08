import inet.applications.contract.IApp;

simple MitMApp like IApp {
    parameters:
        int localPort = default(5000); // Sensor'den gelen dinlenecek port
        int destPort = default(6000); // Actuator'a gonderilecek port
        string destAddress; // Actuator IP
        
        bool canHackCrypto = default(false); // Eger true ise sifrelemeyi kirar
        @display("i=block/attacker");
        
    gates:
        input socketIn;
        output socketOut;
}
