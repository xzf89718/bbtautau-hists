#include "Variables.h"

#include <algorithm>

using namespace std;

Variables::Variables()
{
    m_vars = make_unique<vector<VariableInfo*>>();
}

Variables::~Variables()
{
    for_each(content()->begin(), content()->end(), [](VariableInfo* v) { delete v; });
    content()->clear();
}

void Variables::add(const string &nm, const string &nmtex, unsigned rb, double *bing, size_t n) const
{
    m_vars->emplace_back(new VariableInfo(nm, nmtex, rb, bing, n));
}
