#ifndef __IOT_MITM_SECURITYALGORITHM_H_
#define __IOT_MITM_SECURITYALGORITHM_H_

#include <string>

struct SecurityCost {
    double encryptDelayMs;
    double decryptDelayMs;
    int overheadBytes;
    double sensorEnergy;
    double acEnergy;
};

class SecurityAlgorithm {
  public:
    virtual ~SecurityAlgorithm() = default;
    virtual std::string name() const = 0;
    virtual SecurityCost cost() const = 0;
    virtual bool verifyTag(bool tampered, bool authEnabled) const = 0;
};

class SecurityFactory {
  public:
    static SecurityAlgorithm *create(const std::string& algorithmName);
};

#endif
