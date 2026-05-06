packet IoTPacket
{
    string sourceId;
    string destinationId;
    int sequenceNumber;
    simtime_t timestamp;
    double temperatureValue;
    string nonce;
    int payloadSize;
    string encryptionAlgorithm;
    string authenticationTag;
    bool isEncrypted;
    bool isTampered;
    bool isReplay;
    bool isAccepted;
    string attackType;
}
