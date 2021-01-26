#include "Processes.h"

using namespace std;

Processes::Processes() noexcept
{
    m_procs = make_unique<vector<ProcessInfo*>>();
}

void Processes::add(const string& nm, const string& nmtex, eProcessType tp, 
                    eProcess proc, const string& nmproc, EColor col) const
{
   m_procs->emplace_back(new ProcessInfo(nm, nmtex, tp, proc, nmproc, col));
}
