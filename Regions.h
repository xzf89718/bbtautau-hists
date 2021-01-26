#ifndef REGIONS_H
#define REGIONS_H

#include <memory>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::unique_ptr;

// EUMS
enum class eRegionType { SR, CR };


// ELEM
class RegionInfo
{
public:
    RegionInfo(const string& nm, const string& nmtex, eRegionType tp) noexcept
        : name(nm), nameTeX(nmtex), type(tp) {}

public:
    string name;
    string nameTeX;
    eRegionType type;
};


// COLLECTION
class Regions
{
public:
    Regions();
    void add(const string& nm, const string& nmtex, eRegionType tp) const;
    inline vector<RegionInfo*>* content() const { return m_regions.get(); }
private:
    unique_ptr<vector<RegionInfo*>> m_regions;
};

#endif // REGIONS_H
