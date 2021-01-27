#ifndef HISTTOOL_H
#define HISTTOOL_H

#include "Config.h"
#include "TFile.h"

using std::string;


// Use histograms
class HistTool
{
public:
    HistTool() noexcept {}
    virtual ~HistTool() noexcept {};

public:
    virtual bool check(const Config* c) const;
    virtual void manipulate(const Config* c);
    virtual void makeYield(const Config* c, const string& fn) const;

public:
    virtual void paint(const Config* c) const = 0;
    virtual void run(const Config* c) const = 0;

public:
    string output_path;
};

#endif // HISTTOOL_H