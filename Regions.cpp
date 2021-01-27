#include "Regions.h"

#include <algorithm>

using namespace std;

Regions::Regions()
{
    m_regions = make_unique<vector<RegionInfo*>>();
}

Regions::~Regions()
{
    for_each(content()->begin(), content()->end(), [](RegionInfo* r) { delete r; });
    content()->clear();
}

void Regions::add(const string &nm, const string &nmtex, eRegionType tp) const
{
    m_regions->emplace_back(new RegionInfo(nm, nmtex, tp));
}
