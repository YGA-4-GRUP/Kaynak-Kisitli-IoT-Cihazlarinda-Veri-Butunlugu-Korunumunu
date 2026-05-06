#ifndef __IOT_MITM_SMARTAC_H_
#define __IOT_MITM_SMARTAC_H_

#include <omnetpp.h>
#include <memory>
#include <set>

#include "MetricsCollector.h"
#include "Packet_m.h"
#include "SecurityAlgorithm.h"

using namespace omnetpp;

class SmartAC : public cSimpleModule {
  private:
    std::string expectedSourceId;
    double maxDelayMs = 0.0;
    double timestampWindowSec = 0.0;
    bool authEnabled = true;
    bool nonceCheckEnabled = true;
    bool timestampCheckEnabled = true;
    bool sequenceCheckEnabled = true;

    std::set<int> seenSeq;
    std::set<std::string> seenNonces;
    std::unique_ptr<SecurityAlgorithm> algorithm;
    MetricsCollector *metrics = nullptr;

    bool verify(IoTPacket *packet, bool& replayDetected, bool& spoofDetected, bool& tamperedDetected, double& e2eDelayMs);

  protected:
    void initialize() override;
    void handleMessage(cMessage *msg) override;
};

#endif
