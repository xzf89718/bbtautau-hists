#include "Variables.h"

using namespace std;

Variables::Variables()
{
    m_vars = make_unique<vector<VariableInfo*>>();
}

void Variables::add(const string &nm, const string &nmtex, unsigned rb, float *bing) const
{
    m_vars->emplace_back(new VariableInfo(nm, nmtex, rb, bing));
}
