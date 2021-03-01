#include "Processes.h"

#include <algorithm>

using namespace std;

Processes::Processes() noexcept
{
    m_procs = make_unique<vector<ProcessInfo*>>();
}

Processes::~Processes()
{
    for_each(content()->begin(), content()->end(), [](ProcessInfo* p) { delete p; });
    content()->clear();
}

void Processes::add(const string& nm, const string& nmtex, eProcessType tp, 
                    eProcess proc, const string& nmproc, int col) const
{
    m_procs->emplace_back(new ProcessInfo(nm, nmtex, tp, proc, nmproc, col));
    if (assign_norm_factors) 
    {
        m_procs->back()->norm_factor = normFactors(m_procs->back());
    }
}

double Processes::normFactors(ProcessInfo* p) const
{
    switch (p->process)
    {
    case eProcess::ZllHF:
    case eProcess::ZtautauHF:
        return 1.34;
    case eProcess::ZJETSMG:
    case eProcess::ZllLFMG:
    case eProcess::ZllHFMG:
        return 2085.6 / 2097.184; // cross section corr to compare with Sherpa
    case eProcess::TTBAR:
    case eProcess::TTBARTRUE:
    case eProcess::TTBARTT:
        return 1.00;
    case eProcess::HHKL3FROM1:
        return 3.00;
    case eProcess::HHKL0FROM1:
        return 0.50;
    default:
        return 1.00;
    }
}
