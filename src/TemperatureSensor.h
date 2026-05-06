#ifndef __IOT_MITM_TEMPERATURESENSOR_H_
#define __IOT_MITM_TEMPERATURESENSOR_H_

#include <omnetpp.h>
#include <memory>

#include "MetricsCollector.h"
#include "SecurityAlgorithm.h"
#include "Packet_m.h"

using namespace omnetpp;

class TemperatureSensor : public cSimpleModule {
  private:
    cMessage *tick = nullptr;
    int sequenceNumber = 0;
    int maxMessages = 0;
    double baseTemperature = 0.0;
    std::string sourceId;
    std::string destinationId;
    std::unique_ptr<SecurityAlgorithm> algorithm;
    MetricsCollector *metrics = nullptr;

    IoTPacket *buildPacket();

  protected:
    void initialize() override;
    void handleMessage(cMessage *msg) override;
    void finish() override;
};

#endif
