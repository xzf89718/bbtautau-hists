#include "Config.h"

#include <iterator>
#include <iostream>

using namespace std;

BasicInfo::BasicInfo(const string& ecm, const string& lumi) noexcept
    : ecm(ecm), luminosity(lumi)
{

}

Config::Config(const BasicInfo* b, const Processes* ps, const Regions* rs, const Variables* vs) noexcept
    : basic(b), processes(ps), regions(rs), variables(vs)
{

}
