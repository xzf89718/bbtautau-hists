#include "Regions.h"

using namespace std;

Regions::Regions()
{
    m_regions = make_unique<vector<RegionInfo*>>();
}

void Regions::add(const string &nm, const string &nmtex, eRegionType tp) const
{
    m_regions->emplace_back(new RegionInfo(nm, nmtex, tp));
}
