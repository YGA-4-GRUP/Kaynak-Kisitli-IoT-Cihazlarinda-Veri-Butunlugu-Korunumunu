#ifndef __IOT_MITM_ASCON128MODEL_H_
#define __IOT_MITM_ASCON128MODEL_H_

#include "SecurityAlgorithm.h"

class ASCON128Model : public SecurityAlgorithm {
  public:
    std::string name() const override;
    SecurityCost cost() const override;
    bool verifyTag(bool tampered, bool authEnabled) const override;
};

#endif
