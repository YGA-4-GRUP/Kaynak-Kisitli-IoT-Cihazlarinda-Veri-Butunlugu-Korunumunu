#include "MITMAttacker.h"

#include <string>

Define_Module(MITMAttacker);

void MITMAttacker::initialize() {
    forgedTemperature = par("forgedTemperature");
    maxCaptured = par("maxCaptured");
    attackTick = new cMessage("attackTick");
    scheduleAt(simTime() + par("attackInterval").doubleValue(), attackTick);
}

void MITMAttacker::handleMessage(cMessage *msg) {
    if (msg == attackTick) {
        launchAttack();
        scheduleAt(simTime() + par("attackInterval").doubleValue(), attackTick);
        return;
    }

    auto *packet = check_and_cast<IoTPacket *>(msg);

    if ((int)capturedPackets.size() >= maxCaptured) {
        delete capturedPackets.front();
        capturedPackets.pop_front();
    }
    capturedPackets.push_back(packet->dup());

    send(packet, "out");
}

IoTPacket *MITMAttacker::makeReplayPacket() {
    if (capturedPackets.empty()) return nullptr;
    auto *packet = capturedPackets.back()->dup();
    packet->setName("replayPacket");
    packet->setIsReplay(true);
    packet->setAttackType("Replay");
    return packet;
}

IoTPacket *MITMAttacker::makeSpoofPacket() {
    if (capturedPackets.empty()) return nullptr;
    auto *packet = capturedPackets.back()->dup();
    packet->setName("spoofPacket");
    packet->setSourceId("MITM-Spoofed-Sensor");
    packet->setIsReplay(false);
    packet->setAttackType("Spoofing");
    return packet;
}

IoTPacket *MITMAttacker::makeTamperedPacket() {
    if (capturedPackets.empty()) return nullptr;
    auto *packet = capturedPackets.back()->dup();
    packet->setName("tamperedPacket");
    packet->setTemperatureValue(forgedTemperature);
    packet->setTimestamp(simTime());
    packet->setSequenceNumber(packet->getSequenceNumber() + 1000000);
    packet->setNonce(("mitm-nonce-" + std::to_string((int)(simTime().dbl() * 1000000))).c_str());
    packet->setIsTampered(true);
    packet->setIsReplay(false);
    packet->setAttackType("PayloadManipulation");
    return packet;
}

void MITMAttacker::launchAttack() {
    const int attackType = intuniform(0, 2);
    IoTPacket *attackPacket = nullptr;
    if (attackType == 0) attackPacket = makeReplayPacket();
    if (attackType == 1) attackPacket = makeSpoofPacket();
    if (attackType == 2) attackPacket = makeTamperedPacket();
    if (attackPacket != nullptr) {
        send(attackPacket, "out");
    }
}

void MITMAttacker::finish() {
    cancelAndDelete(attackTick);
    for (auto *p : capturedPackets) {
        delete p;
    }
}
