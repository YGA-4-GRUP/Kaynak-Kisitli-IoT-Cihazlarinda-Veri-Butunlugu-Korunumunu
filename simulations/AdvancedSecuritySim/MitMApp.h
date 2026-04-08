#ifndef __MITMAPP_H_
#define __MITMAPP_H_

#include <omnetpp.h>
#include "inet/transportlayer/contract/udp/UdpSocket.h"

using namespace omnetpp;

class MitMApp : public cSimpleModule
{
  protected:
    int localPort;
    int destPort;
    std::string destAddress;
    bool canHackCrypto;
    
    inet::UdpSocket socket;

    long numReceived;
    long numManipulated;

    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual void handleMessage(cMessage *msg) override;
    virtual void injectManipulatedPacket(cPacket *msg);
    virtual void finish() override;
};

#endif
