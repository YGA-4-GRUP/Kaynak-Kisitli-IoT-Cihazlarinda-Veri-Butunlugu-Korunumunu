#include "SmartSensorApp.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/BytesChunk.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/Protocol.h"

using namespace inet;

Define_Module(SmartSensorApp);

void SmartSensorApp::initialize(int stage)
{
    if (stage == inet::INITSTAGE_LOCAL) {
        localPort = par("localPort");
        destPort = par("destPort");
        destAddress = par("destAddress").stdstringValue();
        sendInterval = par("sendInterval");
        cryptoCpuDelay = par("cryptoCpuDelay").doubleValue();
        senseDelay = par("senseDelay").doubleValue();
        actuateDelay = par("actuateDelay").doubleValue();
        packetSize = par("packetSize");
        cryptoType = par("cryptoType").stdstringValue();
        isActuator = par("isActuator").boolValue();
        
        isCpuBusy = false;
        currentTemp = 28; // Sabit 28 derece okuduğunu varsayalım
        
        numGenerated = 0;
        numSent = 0;
        numReceived = 0;
        numDropped = 0;
        numActuated = 0;
        numManipulated = 0;
        numSecurityVulnerabilities = 0;
        
        dropSignal = registerSignal("droppedTasks");
        taskDelaySignal = registerSignal("taskDelay");
        actuateErrorSignal = registerSignal("actuateError");
        
        senseEvent = new cMessage("senseEvent");
        cryptoEvent = new cMessage("cryptoEvent");
        actuateEvent = new cMessage("actuateEvent");
        
        if (!isActuator) {
            scheduleAt(simTime() + sendInterval, senseEvent);
        }
    }
    else if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
        // UDP Bind islemi
        if (localPort != -1) {
            socket.setOutputGate(gate("socketOut"));
            socket.bind(localPort);
        }
    }
}

void SmartSensorApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == senseEvent) {
            triggerSensing();
            scheduleAt(simTime() + sendInterval, senseEvent);
        }
        else if (msg == cryptoEvent) {
            triggerCrypto();
        }
        else if (msg == actuateEvent) {
            triggerActuation();
        }
    }
    else {
        // Ag uzerinden UDP paketi geldi -> Gelen paket `inet::Packet` olmalidir.
        cPacket *pkt = check_and_cast<cPacket*>(msg);
        processNetworkPacket(pkt);
    }
}

void SmartSensorApp::triggerSensing()
{
    numGenerated++;
    
    // Sensör fiziksel olarak ortamı okur: Gecikme "senseDelay"
    if (taskQueue.size() >= maxQueueSize) {
        numDropped++;
        emit(dropSignal, 1);
        EV << "CPU Yetersiz! Gorev (Task) Atildı.\n";
        return;
    }
    
    // Fake packet representing task
    cPacket *task = new cPacket("DataTask");
    task->setTimestamp(simTime());
    // Kendi yarattığımız veriyi 28 derece olarak sabitliyoruz
    task->addPar("temp") = currentTemp; 
    task->addPar("isEncrypted") = (cryptoType != "None");
    
    taskQueue.push(task);
    
    if (!isCpuBusy) {
        isCpuBusy = true;
        // Sensing görevini isleme al
        scheduleAt(simTime() + senseDelay, cryptoEvent); 
    }
}

void SmartSensorApp::triggerCrypto()
{
    if (taskQueue.empty()) {
        isCpuBusy = false;
        return;
    }
    
    cPacket *task = taskQueue.front();
    
    // Eger Actuator isek: crypto bitince Actuate eventine yolla
    // Eger Sensor isek: crypto bitince Ağa yolla
    if (!isActuator) {
        // Crypto delay uygulanıyor
        task->setTimestamp(simTime()); // Crypto baslama guncellemesi (yeni task stage)
        sendToUDP(task); 
        taskQueue.pop();
        
        simtime_t delay = cryptoCpuDelay;
        if (delay > 0) scheduleAt(simTime() + delay, cryptoEvent); // cpu'yu mesgul tut
        else if (!taskQueue.empty()) scheduleAt(simTime(), cryptoEvent); // hemen digerine gec
        else isCpuBusy = false;
    }
    else {
        // Actuator
        simtime_t delay = cryptoCpuDelay; // Decryption delay
        scheduleAt(simTime() + delay, actuateEvent);
    }
}

void SmartSensorApp::triggerActuation()
{
    if (taskQueue.empty()) {
        isCpuBusy = false;
        return;
    }
    
    cPacket *task = taskQueue.front();
    taskQueue.pop();
    
    int temp = task->par("temp").longValue();
    bool manipulated = task->par("manipulated").boolValue();
    
    // Actuation gecikmesi
    simtime_t e2eTaskDelay = simTime() - task->getTimestamp();
    emit(taskDelaySignal, e2eTaskDelay.dbl());
    
    numActuated++;
    
    if (temp != 28) {
        numSecurityVulnerabilities++;
        emit(actuateErrorSignal, 1);
        EV << "\n[!] GUVENLIK IHLALI: Klima yanlis bilgi aldi (" << temp << ") ve isitmaya basladi!\n";
    }

    delete task;
    
    if (!taskQueue.empty()) scheduleAt(simTime() + actuateDelay, cryptoEvent);
    else isCpuBusy = false;
}

void SmartSensorApp::processNetworkPacket(cPacket *msg)
{
    numReceived++;
    
    if (taskQueue.size() >= maxQueueSize) {
        numDropped++;
        delete msg;
        return;
    }
    
    // INET packetinden bilgileri cikar (Basitce OMNeT parametrelerini aktarıyoruz PoC icin)
    cPacket *task = new cPacket("RecvTask");
    task->setTimestamp(msg->getTimestamp()); // İlk çıkış zamanını koru Jitter için
    task->addPar("temp") = (msg->findPar("temp") != -1) ? msg->par("temp").longValue() : 0;
    task->addPar("manipulated") = (msg->findPar("manipulated") != -1) ? msg->par("manipulated").boolValue() : false;
    task->addPar("isEncrypted") = (msg->findPar("isEncrypted") != -1) ? msg->par("isEncrypted").boolValue() : false;
    
    delete msg;
    
    taskQueue.push(task);
    if (!isCpuBusy) {
        isCpuBusy = true;
        // Alinan paket hemem decrypt kuyruguna girer
        scheduleAt(simTime(), cryptoEvent);
    }
}

void SmartSensorApp::sendToUDP(cPacket *task)
{
    // C++11 auto veya standart kullanim
    auto inetPacket = new inet::Packet("DataPacket");
    
    auto chunk = inet::makeShared<inet::BytesChunk>();
    std::vector<uint8_t> data(packetSize, 'D'); // Dummy data
    chunk->setBytes(data);
    inetPacket->insertAtBack(chunk);
    
    inetPacket->addPar("temp") = task->par("temp").longValue();
    inetPacket->addPar("isEncrypted") = task->par("isEncrypted").boolValue();
    inetPacket->addPar("manipulated") = false;
    inetPacket->setTimestamp(task->getTimestamp()); // Jitter takibi
    
    numSent++;
    
    inet::L3Address destAddr;
    inet::L3AddressResolver().tryResolve(destAddress.c_str(), destAddr);
    socket.sendTo(inetPacket, destAddr, destPort);
    delete task; // Gonderilen is bitti
}

void SmartSensorApp::finish()
{
    recordScalar("NumGenerated", numGenerated);
    recordScalar("NumSent", numSent);
    recordScalar("NumReceived", numReceived);
    recordScalar("NumDropped", numDropped);
    recordScalar("NumActuated", numActuated);
    recordScalar("NumSecurityVulnerabilities", numSecurityVulnerabilities);
}
