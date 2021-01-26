#include "Utils.h"

using namespace std;

namespace Utils
{

void histAssign(TH1* h, ProcessInfo *p)
{
    TH1* h_clone = (TH1*)h->Clone();
    h_clone->SetDirectory(0);
    p->histogram = std::move(h_clone);
}

string histString(const ProcessInfo* p, const RegionInfo* r, const VariableInfo* v)
{
    return p->name + "_" + r->name + "_" + v->name;
}

}
