#include "HistTool.h"

#include <exception>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <iomanip>

using namespace std;

# define FIVE_COLUMN_TABLE(A, B, C, D, E) \
       std::left << setw(30) << A \
    << std::left << setw(15) << B \
    << std::left << setw(15) << C \
    << std::left << setw(15) << D \
    << std::left << setw(15) << E << endl

bool HistTool::check(const Config* c) const 
{
    vector<ProcessInfo*>* ps = c->processes->content();
    clog << "removing nullptrs\n";
    ps->erase(remove_if(ps->begin(), ps->end(), [](const ProcessInfo* p) {
        return !p->histogram; }), ps->end());

    if (ps->size() < 1)
    {
        cerr << "FAIL: empty input\n";
        return false;
    }

    return true;
}

void HistTool::manipulate(Config* c) 
{
    c->setManipulated(true);
    vector<ProcessInfo*>* ps_in_c = c->processes->content();
    clog << "merging\n";
    map<eProcess, vector<ProcessInfo*>> procs;
    for_each(ps_in_c->begin(), ps_in_c->end(), [&procs](ProcessInfo* p) {
        procs[p->process].push_back(p); });

    for_each(procs.begin(), procs.end(), [&ps_in_c, &c](pair<const eProcess, vector<ProcessInfo*>>& pair) {
        // TODO: basically copy, copy assignment?
        ProcessInfo* front = pair.second.front();
        ProcessInfo* merged = new ProcessInfo(
            front->process_name, front->process_name, front->type, 
            front->process, front->process_name, front->color);
        
        merged->histogram = (TH1*)front->histogram->Clone();

        for_each(pair.second.begin() + 1, pair.second.end(), [&merged](const ProcessInfo* p) { 
            merged->histogram->Add(p->histogram); });

        merged->isMerged = true;
        /// @note: make sure the process are not mixed
        merged->norm_factor = front->norm_factor;
        merged->current_region = front->current_region;
        merged->current_variable = front->current_variable;
        
        c->current_region = merged->current_region;
        c->current_variable = merged->current_variable;

        ps_in_c->emplace_back(merged);
    });

    ps_in_c->erase(remove_if(ps_in_c->begin(), ps_in_c->end(), [](const ProcessInfo* p) {
        return !p->isMerged; }), ps_in_c->end());

    sort(ps_in_c->begin(), ps_in_c->end(), [](const ProcessInfo* p1, const ProcessInfo* p2) {
        return p1->type < p2->type; });

    // if do draw stack, the bkg and sig must be sorted!
}

void HistTool::makeYield(const Config* c) const
{   
    ostringstream oss;
    oss << output_path << "/" 
        << c->current_region->name << "_" 
        << c->current_variable->name << ".txt";

    ofstream fout(oss.str());
    vector<ProcessInfo*>* ps = c->processes->content();

    // total backgrouds
    bool hasBkg = false;
    int entriesBkg = 0;
    double sumBkg  = 0.0;
    double errBkg  = 0.0;

    fout << FIVE_COLUMN_TABLE("Process", "Entries", "Yield", "Error", "Rel.Err.");
    for (ProcessInfo* p : *ps)
    {
        double error;
        int from = 0;
        int to = p->histogram->GetNbinsX() + 1;
        int nentries = p->histogram->GetEntries();
        double integral = p->histogram->IntegralAndError(from, to, error, "");
        double eOverI = *(long*)(&integral) ? error / integral : 0.;
        fout << FIVE_COLUMN_TABLE(p->name, nentries, integral * p->norm_factor, error * p->norm_factor, eOverI);

        if (p->type == eProcessType::BKG)
        {
            hasBkg = true;
            entriesBkg += nentries;
            sumBkg += integral * p->norm_factor; 
            errBkg += error * error * p->norm_factor * p->norm_factor;
        }
    }
    if (hasBkg) {
        errBkg = sqrt(errBkg);
        fout << FIVE_COLUMN_TABLE("Total Bkg", entriesBkg, sumBkg, errBkg, errBkg / sumBkg); 
    }

    clog << "Yields saved in " << oss.str() << '\n';
}
