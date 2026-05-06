#include "SecurityAlgorithm.h"
#include "ChaCha20Poly1305Model.h"
#include "ASCON128Model.h"
#include "SIMONModel.h"

#include <stdexcept>

std::string ChaCha20Poly1305Model::name() const { return "ChaCha20-Poly1305"; }
SecurityCost ChaCha20Poly1305Model::cost() const { return {0.45, 0.40, 16, 0.19, 0.17}; }
bool ChaCha20Poly1305Model::verifyTag(bool tampered, bool authEnabled) const { return authEnabled && !tampered; }

std::string ASCON128Model::name() const { return "ASCON-128"; }
SecurityCost ASCON128Model::cost() const { return {0.52, 0.48, 16, 0.17, 0.16}; }
bool ASCON128Model::verifyTag(bool tampered, bool authEnabled) const { return authEnabled && !tampered; }

std::string SIMONModel::name() const { return "SIMON"; }
SecurityCost SIMONModel::cost() const { return {0.30, 0.28, 8, 0.12, 0.10}; }
bool SIMONModel::verifyTag(bool tampered, bool authEnabled) const { return authEnabled && !tampered; }

SecurityAlgorithm *SecurityFactory::create(const std::string& algorithmName) {
    if (algorithmName == "ChaCha20-Poly1305") {
        return new ChaCha20Poly1305Model();
    }
    if (algorithmName == "ASCON-128") {
        return new ASCON128Model();
    }
    if (algorithmName == "SIMON") {
        return new SIMONModel();
    }
    throw std::runtime_error("Unknown algorithm: " + algorithmName);
}
