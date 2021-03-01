#include "Utils.h"

using namespace std;

namespace Utils
{

void histAssign(TH1* h, ProcessInfo *p, RegionInfo* r, VariableInfo* v)
{
    TH1* h_clone = (TH1*)h->Clone();
    h_clone->SetDirectory(0);
    p->histogram = std::move(h_clone);
    p->current_region = r;
    p->current_variable = v;
}

void histAssignSyst(TH1* h, ProcessInfo *p, const std::string& systname)
{
    TH1* h_clone = (TH1*)h->Clone();
    h_clone->SetDirectory(0);
    p->systematic_histograms[systname] = std::move(h_clone);
}

string histString(const ProcessInfo* p, const RegionInfo* r, const VariableInfo* v)
{
    return p->name + "_" + r->name + "_" + v->name;
}

string systString(const SystematicInfo* s)
{
    return s->name;
}

string histStringSyst(const ProcessInfo* p, const RegionInfo* r, const VariableInfo* v, const SystematicInfo* s)
{
    return histString(p, r, v) + "_Sys" + s->name;
}

}
