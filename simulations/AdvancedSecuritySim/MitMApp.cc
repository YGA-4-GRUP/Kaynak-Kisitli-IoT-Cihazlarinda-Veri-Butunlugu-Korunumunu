#include "MitMApp.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/BytesChunk.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/Protocol.h"

using namespace inet;

Define_Module(MitMApp);

void MitMApp::initialize(int stage)
{
    if (stage == inet::INITSTAGE_LOCAL) {
        localPort = par("localPort");
        destPort = par("destPort");
        destAddress = par("destAddress").stdstringValue();
        canHackCrypto = par("canHackCrypto").boolValue();
        
        numReceived = 0;
        numManipulated = 0;
    }
    else if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
        if (localPort != -1) {
            socket.setOutputGate(gate("socketOut"));
            socket.bind(localPort);
        }
    }
}

void MitMApp::handleMessage(cMessage *msg)
{
    if (!msg->isSelfMessage()) {
        cPacket *pkt = check_and_cast<cPacket*>(msg);
        injectManipulatedPacket(pkt);
    }
}

void MitMApp::injectManipulatedPacket(cPacket *msg)
{
    numReceived++;
    
    auto inetPacketOriginal = check_and_cast<inet::Packet*>(msg);
    auto newInetPacket = new inet::Packet("MitM_Packet");
    
    // Chunk'ları kopyala
    newInetPacket->insertAtBack(inetPacketOriginal->peekData());
    
    // C++ Tag'larından değerleri taklit ederek alıyoruz
    long temp = (inetPacketOriginal->findPar("temp") != -1) ? inetPacketOriginal->par("temp").longValue() : 28;
    bool isEncrypted = (inetPacketOriginal->findPar("isEncrypted") != -1) ? inetPacketOriginal->par("isEncrypted").boolValue() : false;
    simtime_t creationTime = inetPacketOriginal->getTimestamp();
    
    bool isManipulated = false;
    // Eğer şifre yoksa veya saldırgan kırabiliyorsa manipüle et
    if (!isEncrypted || canHackCrypto) {
        temp = 13; // Ortam sicakligini 13 derece olarak degistir (Hedef yanilgi)
        isManipulated = true;
        numManipulated++;
        EV << "MitM: Sifresiz paket yakalandi! Sicaklik 13 derece olarak degistirildi.\n";
    } else {
        EV << "MitM: Paket sifreli, icerige dokunulmadan iletiliyor.\n";
    }
    
    newInetPacket->addPar("temp") = temp;
    newInetPacket->addPar("isEncrypted") = isEncrypted;
    newInetPacket->addPar("manipulated") = isManipulated;
    newInetPacket->setTimestamp(creationTime);
    
    delete msg; // Orjinal paketi yokedin (Caldik)
    
    // Actuator'a gonder:
    inet::L3Address destAddr;
    inet::L3AddressResolver().tryResolve(destAddress.c_str(), destAddr);
    socket.sendTo(newInetPacket, destAddr, destPort);
}

void MitMApp::finish()
{
    recordScalar("MitM_Received", numReceived);
    recordScalar("MitM_Manipulated", numManipulated);
}
