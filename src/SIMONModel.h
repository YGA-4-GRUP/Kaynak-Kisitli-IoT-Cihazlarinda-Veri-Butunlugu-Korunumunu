#ifndef __IOT_MITM_SIMONMODEL_H_
#define __IOT_MITM_SIMONMODEL_H_

#include "SecurityAlgorithm.h"

class SIMONModel : public SecurityAlgorithm {
  public:
    std::string name() const override;
    SecurityCost cost() const override;
    bool verifyTag(bool tampered, bool authEnabled) const override;
};

#endif
