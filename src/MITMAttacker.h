#ifndef __IOT_MITM_MITMATTACKER_H_
#define __IOT_MITM_MITMATTACKER_H_

#include <omnetpp.h>
#include <deque>

#include "Packet_m.h"

using namespace omnetpp;

class MITMAttacker : public cSimpleModule {
  private:
    cMessage *attackTick = nullptr;
    std::deque<IoTPacket *> capturedPackets;
    double forgedTemperature = 18.0;
    int maxCaptured = 16;

    void launchAttack();
    IoTPacket *makeReplayPacket();
    IoTPacket *makeSpoofPacket();
    IoTPacket *makeTamperedPacket();

  protected:
    void initialize() override;
    void handleMessage(cMessage *msg) override;
    void finish() override;
};

#endif
