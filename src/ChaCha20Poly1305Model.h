#ifndef __IOT_MITM_CHACHA20POLY1305MODEL_H_
#define __IOT_MITM_CHACHA20POLY1305MODEL_H_

#include "SecurityAlgorithm.h"

class ChaCha20Poly1305Model : public SecurityAlgorithm {
  public:
    std::string name() const override;
    SecurityCost cost() const override;
    bool verifyTag(bool tampered, bool authEnabled) const override;
};

#endif
