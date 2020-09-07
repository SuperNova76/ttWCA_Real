#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>  
#include <numeric>
#include <stdexcept>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TEnv.h"
#include "TH1.h"
#include "TChain.h"
#include "TString.h"
#include "TSystem.h"

static bool debug = 0;
static char* name;

void INFO(std::string msg){ std::cout << name << "\t INFO \t" << msg << std::endl; }
void DEBUG(std::string msg){ if(debug) std::cout << name << "\t DEBUG \t" << msg << std::endl; }
void ERROR(std::string msg){ std::cout << name << "\t ERROR \t" << msg << std::endl; exit(1); }

int main(int argc, char* argv[]){
  if(argc < 2) throw std::invalid_argument("Too few arguments");
  name = argv[0];
  auto start = std::time(nullptr);

  INFO("Initializing");
  INFO(std::ctime(&start));

  auto end = std::time(nullptr);
  INFO("Finalizing");
  INFO(std::ctime(&end));
}

