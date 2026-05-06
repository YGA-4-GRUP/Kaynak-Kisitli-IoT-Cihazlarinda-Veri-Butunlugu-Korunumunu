#include "TemperatureSensor.h"

#include <sstream>

Define_Module(TemperatureSensor);

void TemperatureSensor::initialize() {
    maxMessages = par("maxMessages");
    baseTemperature = par("baseTemperature");
    sourceId = par("sourceId").stdstringValue();
    destinationId = par("destinationId").stdstringValue();
    algorithm.reset(SecurityFactory::create(par("algorithm").stdstringValue()));

    metrics = check_and_cast<MetricsCollector *>(getParentModule()->getSubmodule("metrics"));

    tick = new cMessage("sensorTick");
    scheduleAt(simTime() + par("sendInterval").doubleValue(), tick);
}

IoTPacket *TemperatureSensor::buildPacket() {
    auto *packet = new IoTPacket("sensorData");
    packet->setSourceId(sourceId.c_str());
    packet->setDestinationId(destinationId.c_str());
    packet->setSequenceNumber(sequenceNumber++);
    packet->setTimestamp(simTime());
    packet->setTemperatureValue(baseTemperature + uniform(-1.5, 1.5));
    packet->setNonce((sourceId + "-" + std::to_string(packet->getSequenceNumber())).c_str());
    packet->setEncryptionAlgorithm(algorithm->name().c_str());
    packet->setPayloadSize(par("payloadSize"));
    packet->setAuthenticationTag(("tag-" + std::to_string(int(simTime().dbl() * 1000))).c_str());
    packet->setIsEncrypted(true);
    packet->setIsTampered(false);
    packet->setIsReplay(false);
    packet->setIsAccepted(false);
    packet->setAttackType("None");
    return packet;
}

void TemperatureSensor::handleMessage(cMessage *msg) {
    if (msg != tick) {
        delete msg;
        return;
    }

    if (sequenceNumber >= maxMessages) {
        return;
    }

    auto *packet = buildPacket();
    send(packet, "out");
    scheduleAt(simTime() + par("sendInterval").doubleValue(), tick);
}

void TemperatureSensor::finish() {
    cancelAndDelete(tick);
}
