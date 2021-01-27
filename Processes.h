#ifndef PROCESSES_H
#define PROCESSES_H

#include "Regions.h"
#include "Variables.h"

#include "TColor.h"
#include "TH1.h"

#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::unique_ptr;

// ENUMS
enum class eProcessType { DATA, BKG, SIG };

enum class eProcess {
    DATA, OTHERS, BKG, 
    TTBAR, TTBARTRUE, TTBARFAKE, TTBARTT, TTBARTF, TTBARFT, TTBARFF,
    STOP, STOPT, STOPS, STOPWT,
    ZJETS, ZtautauJETS, ZtautauHF, ZtautauLF, ZllJETS, ZllLF, ZllHF,
    WJETS, WtauvJETS, WtauvHF, WtauvLF, WlvJETS, WlvLF, WlvHF,
    H, VH, WH, ZH, ttH, ggH, VBFH,
    DIBOSON, WW, WZ, ZZ, FAKE, QCD, MULTIJET,
    SIG, SMHH, HH, XtoHH, StoHH
};

// ENTRY
class ProcessInfo
{
public:
    ProcessInfo(const string& nm, const string& nmtex, eProcessType tp, 
                eProcess proc, const string& nmproc, int col) noexcept
        : name(nm), name_tex(nmtex), type(tp)
        , process(proc), process_name(nmproc), color(col) {}
public:
    string name; // same as histogram perfix
    string name_tex;
    eProcessType type;
    eProcess process;
    string process_name;
    int color; // EColor
    int rbg = 0xFFFFFF; // TODO: master of color platte
    TH1* histogram = nullptr; // depends on region and variable (will be set in Config)
    bool isMerged = false;
    RegionInfo* current_region;
    VariableInfo* current_variable;

    bool operator< (const ProcessInfo& p) const 
    { 
        return histogram->Integral() < p.histogram->Integral();
    }

    bool operator> (const ProcessInfo& p) const 
    { 
        return histogram->Integral() > p.histogram->Integral();
    }
};


// COLLECTION
class Processes
{
public:
    Processes() noexcept;
    ~Processes();
    Processes(Processes& ps) = delete;
    Processes& operator=(Processes& ps) = delete;

public:
    void add(const string& nm, const string& nmtex, eProcessType tp, 
             eProcess proc, const string& nmproc, int col) const;
    inline vector<ProcessInfo*>* content() const { return m_procs.get(); }

private:
    unique_ptr<vector<ProcessInfo*>> m_procs;
};

#endif // PROCESSES_H
