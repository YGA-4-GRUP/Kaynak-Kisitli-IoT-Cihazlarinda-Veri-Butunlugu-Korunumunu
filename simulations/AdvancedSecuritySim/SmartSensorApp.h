#ifndef __SMARTSENSORAPP_H_
#define __SMARTSENSORAPP_H_

#include <omnetpp.h>
#include <queue>
#include "inet/transportlayer/contract/udp/UdpSocket.h"

using namespace omnetpp;

class SmartSensorApp : public cSimpleModule
{
  protected:
    int localPort;
    int destPort;
    std::string destAddress;
    
    double sendInterval;
    double cryptoCpuDelay;
    double senseDelay;
    double actuateDelay;
    int packetSize;
    std::string cryptoType;
    bool isActuator;

    inet::UdpSocket socket;

    cMessage *senseEvent;
    cMessage *cryptoEvent;
    cMessage *actuateEvent;
    
    // Uygulama çalışma durumu
    bool isCpuBusy;
    int currentTemp; // Simüle edilen sicaklik

    // Istatistikler
    long numGenerated;
    long numSent;
    long numReceived;
    long numDropped;
    long numActuated;
    long numManipulated;
    long numSecurityVulnerabilities;
    
    simsignal_t dropSignal;
    simsignal_t taskDelaySignal;
    simsignal_t actuateErrorSignal;

    std::queue<cPacket*> taskQueue;
    int maxQueueSize = 50;

    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; } // INET applications use multiple init stages
    virtual void handleMessage(cMessage *msg) override;
    
    virtual void processNetworkPacket(cPacket *msg);
    virtual void triggerSensing();
    virtual void triggerCrypto();
    virtual void triggerActuation();
    virtual void sendToUDP(cPacket *pkt);
    virtual void finish() override;
};

#endif
