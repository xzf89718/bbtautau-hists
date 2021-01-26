#ifndef UTILS_H
#define UTILS_H

#include "Processes.h"
#include "Regions.h"
#include "Variables.h"

#include "TH1.h"
#include "TFile.h"
#include "TDirectory.h"

#include <string>

using std::string;

namespace Utils {

    void updateHistogram(TH1* h, ProcessInfo& p);

    string histString(const ProcessInfo& p, const RegionsInfo& r, const VariableInfo& v);
}

#endif // UTILS_H
