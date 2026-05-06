#ifndef __IOT_MITM_METRICSCOLLECTOR_H_
#define __IOT_MITM_METRICSCOLLECTOR_H_

#include <omnetpp.h>
#include <fstream>
#include <map>
#include <string>

using namespace omnetpp;

struct ScenarioStats {
    int accepted = 0;
    int rejected = 0;
    int replayRejected = 0;
    int spoofRejected = 0;
    int tamperRejected = 0;
    int attackAttempts = 0;
    int attackSuccess = 0;
    double totalE2eDelay = 0.0;
    int e2eSamples = 0;
    double totalEncryptDelay = 0.0;
    double totalDecryptDelay = 0.0;
    double sensorEnergy = 0.0;
    double acEnergy = 0.0;
    double totalOverhead = 0.0;
    double firstAttackSuccessTime = -1.0;
};

class MetricsCollector : public cSimpleModule {
  private:
    std::ofstream rawFile;
    std::string resultDir;
    std::map<std::string, ScenarioStats> scenarioStats;

    std::string key(const std::string& networkType, const std::string& algorithm) const;
    void writeSummary();
    void writeAttackSummary();
    void writePerformanceSummary();

  protected:
    void initialize() override;
    void handleMessage(cMessage *msg) override;
    void finish() override;

  public:
    void recordRaw(const std::string& row);
    void recordPacket(const std::string& networkType, const std::string& algorithm, bool accepted, bool isAttack, bool attackSucceeded,
                      bool isReplay, bool isSpoof, bool isTampered, double e2eDelayMs, double encMs, double decMs,
                      double sensorEnergyInc, double acEnergyInc, int overhead, simtime_t now);
};

#endif
