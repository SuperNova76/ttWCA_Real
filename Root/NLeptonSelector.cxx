#include "ttWCA/NLeptonSelector.h"

namespace top {
  NLeptonSelector::NLeptonSelector(std::string const & params) :
    SignValueSelector("LEP_N", params, true) {
    checkMultiplicityIsInteger();
  }

  bool NLeptonSelector::apply(top::Event const & event) const {
  
    int nLep(0);
    for(const auto el : event.m_electrons){ if(el->pt() >= value()) nLep++; }
    for(const auto mu : event.m_muons){ if(mu->pt() >= value()) nLep++; }

    return checkInt(nLep, multiplicity());
  }

  std::string NLeptonSelector::name() const { return "LEP_N"; }
}
