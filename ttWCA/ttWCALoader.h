#ifndef TTWCA_TTWCALOADER_H
#define TTWCA_TTWCALOADER_H

#include "TopEventSelectionTools/ToolLoaderBase.h"

namespace top{
  class ttWCALoader : public ToolLoaderBase {
  public:
    top::EventSelectorBase* initTool(const std::string& name, const std::string& line, TFile* outputFile, std::shared_ptr<top::TopConfig> config,EL::Worker* wk = nullptr);

    ClassDef(top::ttWCALoader, 0)
      };
}
#endif
