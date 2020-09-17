#ifndef TTWCA_NLEPTONSELECTOR_H
#define TTWCA_NLEPTONSELECTOR_H

#include "TopEventSelectionTools/SignValueSelector.h"

namespace top {
  class NLeptonSelector: public SignValueSelector {
  public:
    explicit NLeptonSelector(const std::string& params);
    
    bool apply(const top::Event& event) const override;
    std::string name() const override;
  };
}

#endif
