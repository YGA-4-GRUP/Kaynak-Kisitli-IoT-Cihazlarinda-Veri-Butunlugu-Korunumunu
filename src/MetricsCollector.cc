#include "MetricsCollector.h"

#include <filesystem>
#include <iomanip>

Define_Module(MetricsCollector);

void MetricsCollector::initialize() {
    resultDir = par("resultDir").stdstringValue();
    std::filesystem::create_directories(resultDir + "/figures");
    const auto rawPath = resultDir + "/raw_results.csv";
    const bool exists = std::filesystem::exists(rawPath);
    rawFile.open(rawPath, std::ios::out | std::ios::app);
    if (!exists) {
        rawFile << "simTime,network,algorithm,accepted,isAttack,attackSucceeded,isReplay,isSpoof,isTampered,e2eDelayMs,encMs,decMs,sensorEnergy,acEnergy,overhead,authEnabled,nonceCheckEnabled,timestampCheckEnabled,sequenceCheckEnabled\n";
    }
}

void MetricsCollector::handleMessage(cMessage *msg) {
    delete msg;
}

void MetricsCollector::recordRaw(const std::string& row) {
    rawFile << row << "\n";
}

std::string MetricsCollector::key(const std::string& networkType, const std::string& algorithm) const {
    return networkType + "|" + algorithm;
}

void MetricsCollector::recordPacket(const std::string& networkType, const std::string& algorithm, bool accepted, bool isAttack, bool attackSucceeded,
                                    bool isReplay, bool isSpoof, bool isTampered, double e2eDelayMs, double encMs, double decMs,
                                    double sensorEnergyInc, double acEnergyInc, int overhead, simtime_t now) {
    const auto k = key(networkType, algorithm);
    auto& s = scenarioStats[k];

    if (accepted) s.accepted++; else s.rejected++;
    if (!accepted && isReplay) s.replayRejected++;
    if (!accepted && isSpoof) s.spoofRejected++;
    if (!accepted && isTampered) s.tamperRejected++;

    if (isAttack) {
        s.attackAttempts++;
        if (attackSucceeded) {
            s.attackSuccess++;
            if (s.firstAttackSuccessTime < 0.0) {
                s.firstAttackSuccessTime = now.dbl();
            }
        }
    }

    s.totalE2eDelay += e2eDelayMs;
    s.e2eSamples++;
    s.totalEncryptDelay += encMs;
    s.totalDecryptDelay += decMs;
    s.sensorEnergy += sensorEnergyInc;
    s.acEnergy += acEnergyInc;
    s.totalOverhead += overhead;
}

void MetricsCollector::writeSummary() {
    std::ofstream out(resultDir + "/summary_results.csv", std::ios::out);
    out << "network,algorithm,accepted,rejected,mitmSuccessRate,avgE2eDelayMs,avgEncMs,avgDecMs,totalSensorEnergy,totalAcEnergy,avgOverhead\n";
    for (const auto& [k, s] : scenarioStats) {
        const auto sep = k.find('|');
        const auto network = k.substr(0, sep);
        const auto algo = k.substr(sep + 1);
        const double mitmRate = s.attackAttempts > 0 ? (double)s.attackSuccess / s.attackAttempts : 0.0;
        const double avgDelay = s.e2eSamples > 0 ? s.totalE2eDelay / s.e2eSamples : 0.0;
        const double avgEnc = s.e2eSamples > 0 ? s.totalEncryptDelay / s.e2eSamples : 0.0;
        const double avgDec = s.e2eSamples > 0 ? s.totalDecryptDelay / s.e2eSamples : 0.0;
        const double avgOv = s.e2eSamples > 0 ? s.totalOverhead / s.e2eSamples : 0.0;
        out << network << "," << algo << "," << s.accepted << "," << s.rejected << "," << mitmRate << "," << avgDelay << ","
            << avgEnc << "," << avgDec << "," << s.sensorEnergy << "," << s.acEnergy << "," << avgOv << "\n";
    }
}

void MetricsCollector::writeAttackSummary() {
    std::ofstream out(resultDir + "/attack_success.csv", std::ios::out);
    out << "network,algorithm,attackAttempts,attackSuccess,mitmSuccessRate,firstSuccessTime,replayRejected,spoofRejected,tamperRejected\n";
    for (const auto& [k, s] : scenarioStats) {
        const auto sep = k.find('|');
        const auto network = k.substr(0, sep);
        const auto algo = k.substr(sep + 1);
        const double mitmRate = s.attackAttempts > 0 ? (double)s.attackSuccess / s.attackAttempts : 0.0;
        out << network << "," << algo << "," << s.attackAttempts << "," << s.attackSuccess << "," << mitmRate << ","
            << s.firstAttackSuccessTime << "," << s.replayRejected << "," << s.spoofRejected << "," << s.tamperRejected << "\n";
    }
}

void MetricsCollector::writePerformanceSummary() {
    std::ofstream out(resultDir + "/performance_metrics.csv", std::ios::out);
    out << "network,algorithm,avgE2eDelayMs,avgEncMs,avgDecMs,totalEnergy,avgOverhead\n";
    for (const auto& [k, s] : scenarioStats) {
        const auto sep = k.find('|');
        const auto network = k.substr(0, sep);
        const auto algo = k.substr(sep + 1);
        const double avgDelay = s.e2eSamples > 0 ? s.totalE2eDelay / s.e2eSamples : 0.0;
        const double avgEnc = s.e2eSamples > 0 ? s.totalEncryptDelay / s.e2eSamples : 0.0;
        const double avgDec = s.e2eSamples > 0 ? s.totalDecryptDelay / s.e2eSamples : 0.0;
        const double avgOv = s.e2eSamples > 0 ? s.totalOverhead / s.e2eSamples : 0.0;
        out << network << "," << algo << "," << avgDelay << "," << avgEnc << "," << avgDec << "," << (s.sensorEnergy + s.acEnergy) << "," << avgOv << "\n";
    }
}

void MetricsCollector::finish() {
    writeSummary();
    writeAttackSummary();
    writePerformanceSummary();
    rawFile.close();
}
