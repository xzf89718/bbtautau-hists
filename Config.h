#ifndef CONFIG_H
#define CONFIG_H

#include "Processes.h"
#include "Regions.h"
#include "Variables.h"

#include <string>
#include <memory>

using std::string;

class BasicInfo
{
public:
    BasicInfo(const string& ecm, const string& lumi) noexcept;

public:
    string ecm;
    string luminosity;
};


class Config
{
public:
    Config(const BasicInfo* b, const Processes* ps, const Regions* rs, const Variables* vs) noexcept;

public:
    const BasicInfo* basic;
    const Processes* processes;
    const Regions* regions;
    const Variables* variables;
};

#endif // CONFIG_H
