#include "Processes.h"

using namespace std;

Processes::Processes() noexcept
{
    m_procs = make_unique<vector<ProcessInfo>>();
}

void Processes::add(const string& nm, const string& nmtex, eProcessType tp, eProcess proc, EColor col) const
{
   m_procs->emplace_back(nm, nmtex, tp, proc, col);
}
