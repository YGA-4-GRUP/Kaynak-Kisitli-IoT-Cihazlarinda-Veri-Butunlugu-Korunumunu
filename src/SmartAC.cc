#include "SmartAC.h"

#include <cmath>
#include <sstream>

Define_Module(SmartAC);

void SmartAC::initialize() {
    expectedSourceId = par("expectedSourceId").stdstringValue();
    maxDelayMs = par("maxDelayMs");
    timestampWindowSec = par("timestampWindowSec");
    authEnabled = par("authEnabled");
    nonceCheckEnabled = par("nonceCheckEnabled");
    timestampCheckEnabled = par("timestampCheckEnabled");
    sequenceCheckEnabled = par("sequenceCheckEnabled");

    algorithm.reset(SecurityFactory::create(par("algorithm").stdstringValue()));
    metrics = check_and_cast<MetricsCollector *>(getParentModule()->getSubmodule("metrics"));
}

bool SmartAC::verify(IoTPacket *packet, bool& replayDetected, bool& spoofDetected, bool& tamperedDetected, double& e2eDelayMs) {
    replayDetected = false;
    spoofDetected = false;
    tamperedDetected = false;
    e2eDelayMs = (simTime() - packet->getTimestamp()).dbl() * 1000.0;

    if (packet->getSourceId() != expectedSourceId) {
        spoofDetected = true;
        return false;
    }

    if (sequenceCheckEnabled && seenSeq.count(packet->getSequenceNumber())) {
        replayDetected = true;
        return false;
    }

    if (nonceCheckEnabled && seenNonces.count(packet->getNonce())) {
        replayDetected = true;
        return false;
    }

    if (timestampCheckEnabled && fabs((simTime() - packet->getTimestamp()).dbl()) > timestampWindowSec) {
        return false;
    }

    if (e2eDelayMs > maxDelayMs) {
        return false;
    }

    if (!algorithm->verifyTag(packet->isTampered(), authEnabled)) {
        tamperedDetected = packet->isTampered();
        return false;
    }

    seenSeq.insert(packet->getSequenceNumber());
    seenNonces.insert(packet->getNonce());
    return true;
}

void SmartAC::handleMessage(cMessage *msg) {
    auto *packet = check_and_cast<IoTPacket *>(msg);
    bool replayDetected, spoofDetected, tamperedDetected;
    double e2eDelayMs = 0.0;

    const bool accepted = verify(packet, replayDetected, spoofDetected, tamperedDetected, e2eDelayMs);
    packet->setIsAccepted(accepted);

    auto cost = algorithm->cost();
    const std::string networkType = par("networkType").stdstringValue();
    const bool isAttack = std::string(packet->getAttackType()) != "None";
    const bool attackSucceeded = isAttack && accepted;

    std::ostringstream row;
    row << simTime() << "," << networkType << "," << algorithm->name() << "," << (accepted ? 1 : 0) << ","
        << (isAttack ? 1 : 0) << "," << (attackSucceeded ? 1 : 0) << "," << (replayDetected ? 1 : 0) << ","
        << (spoofDetected ? 1 : 0) << "," << (tamperedDetected ? 1 : 0) << "," << e2eDelayMs << ","
        << cost.encryptDelayMs << "," << cost.decryptDelayMs << "," << cost.sensorEnergy << "," << cost.acEnergy << "," << cost.overheadBytes << ","
        << (authEnabled ? 1 : 0) << "," << (nonceCheckEnabled ? 1 : 0) << "," << (timestampCheckEnabled ? 1 : 0) << "," << (sequenceCheckEnabled ? 1 : 0);
    metrics->recordRaw(row.str());
    metrics->recordPacket(networkType, algorithm->name(), accepted, isAttack, attackSucceeded, replayDetected, spoofDetected, tamperedDetected,
                          e2eDelayMs, cost.encryptDelayMs, cost.decryptDelayMs, cost.sensorEnergy, cost.acEnergy, cost.overheadBytes, simTime());

    delete packet;
}
