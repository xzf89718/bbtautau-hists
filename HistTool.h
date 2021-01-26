#ifndef HISTTOOL_H
#define HISTTOOL_H

#include "Config.h"
#include "TFile.h"

using std::string;

class HistTool
{
public:
    HistTool() noexcept;

public:
    bool check(const Config* c) const;
    void merge(const Config* c) const;
    void makeYield(const Config* c, const string& fn) const;
    virtual void paint(const Config* c) const {};

};

#endif // HISTTOOL_H