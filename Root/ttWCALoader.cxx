#include "ttWCA/ttWCALoader.h"
#include "ttWCA/NLeptonSelector.h"
#include "TopConfiguration/TopConfig.h"

#include <iostream>
#include <sstream>
#include "TFile.h"

namespace top{
  top::EventSelectorBase* ttWCALoader::initTool(const std::string& /*name*/, const std::string& line, TFile* /*outputFile*/, std::shared_ptr<top::TopConfig> /*config*/,EL::Worker* /*wk*/){
    std::istringstream iss(line);
    std::string toolname;
    getline(iss, toolname, ' ');

    std::string param;
    if (line.size() > toolname.size())
      param = line.substr(toolname.size() + 1);
    if(toolname == "LEP_N") 
      return new top::NLeptonSelector(param);
    return nullptr;
  }
}
