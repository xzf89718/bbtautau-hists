#ifndef PROCESSES_H
#define PROCESSES_H

#include <memory>
#include <string>
#include <vector>

#include "TColor.h"
#include "TH1.h"

using std::string;
using std::vector;
using std::unique_ptr;

// ENUMS
enum class eProcessType { DATA, BKG, SIG };

enum class eProcess {
    DATA,
    TTBAR, TTBARTRUE, TTBARFAKE, TTBARTT, TTBARTF, TTBARFT, TTBARFF,
    STOP, STOPT, STOPS, STOPWT,
    ZJETS, ZtautauJETS, ZtautauHF, ZtautauLF, ZllJETS, ZllLF, ZllHF,
    WJETS, WtauvJETS, WtauvHF, WtauvLF, WlvJETS, WlvLF, WlvHF,
    H, VH, WH, ZH, ttH, ggH, VBFH,
    DIBOSON, WW, WZ, ZZ, FAKE, QCD, MULTIJET,
    SMHH, HH, XtoHH, StoHH
};

// ENTRY
class ProcessInfo
{
public:
    ProcessInfo(const string& nm, const string& nmtex, eProcessType tp, 
                eProcess proc, const string& nmproc, EColor col) noexcept
        : name(nm), nameTeX(nmtex), type(tp)
        , process(proc), processName(nmproc), color(col) {}
public:
    string name; // same as histogram perfix
    string nameTeX;
    eProcessType type;
    eProcess process;
    string processName;
    EColor color;
    int rbg = 0xFFFFFF; // TODO: master of color platte
    TH1* histogram = nullptr; // depends on region and variable (will be set in Config)
    bool isMerged = false;

    bool operator< (const ProcessInfo& p) const 
    { 
        return (histogram && p.histogram) && (histogram->Integral() < p.histogram->Integral());
    }

    bool operator> (const ProcessInfo& p) const 
    { 
        return (histogram && p.histogram) && (histogram->Integral() > p.histogram->Integral());
    }
};


// COLLECTION
class Processes
{
public:
    Processes() noexcept;
    void add(const string& nm, const string& nmtex, eProcessType tp, 
             eProcess proc, const string& nmproc, EColor col) const;
    inline vector<ProcessInfo*>* content() const { return m_procs.get(); }
private:
    unique_ptr<vector<ProcessInfo*>> m_procs;
};

#endif // PROCESSES_H
