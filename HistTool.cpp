#include "HistTool.h"

#include <exception>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <iomanip>

using namespace std;

HistTool::HistTool() noexcept
{
}

bool HistTool::check(const Config* c) const 
{
    vector<ProcessInfo*>* ps = c->processes->content();
    clog << "removing nullptrs\n";
    ps->erase(remove_if(ps->begin(), ps->end(), [](const ProcessInfo* p) {
        return !p->histogram; }), ps->end());

    int n_data_check = count_if(ps->begin(), ps->end(), [](const ProcessInfo* p) {
        return p->type == eProcessType::DATA; });

    if (n_data_check > 1) {
        cerr << "Hey you have more than one data!\n";
        // return false;
    }
    
    return true;
}

void HistTool::merge(const Config* c) const 
{
    vector<ProcessInfo*>* ps_in_c = c->processes->content();
    clog << "merging\n";
    map<eProcess, vector<ProcessInfo*>> procs;
    for_each(ps_in_c->begin(), ps_in_c->end(), [&procs](ProcessInfo* p) {
        procs[p->process].push_back(p); });

    for_each(procs.begin(), procs.end(), [&ps_in_c](pair<const eProcess, vector<ProcessInfo*>>& pair) {
        ProcessInfo* front = pair.second.front();
        ProcessInfo* merged = new ProcessInfo(front->processName, front->processName, front->type, 
            front->process, front->processName, front->color);
        merged->histogram = (TH1*)front->histogram->Clone();
        for_each(pair.second.begin() + 1, pair.second.end(), [&merged](const ProcessInfo* p) { 
            merged->histogram->Add(p->histogram); });
        merged->isMerged = true;
        ps_in_c->emplace_back(merged);
    });

    ps_in_c->erase(remove_if(ps_in_c->begin(), ps_in_c->end(), [](const ProcessInfo* p) {
        return !p->isMerged; }), ps_in_c->end());

    sort(ps_in_c->begin(), ps_in_c->end(), [](const ProcessInfo* p1, const ProcessInfo* p2) {
        return p1->type < p2->type; });

    // if do draw stack, the bkg and sig must be sorted!
}

void HistTool::makeYield(const Config* c, const string& fn) const
{
    // ofstream fout(fn);
    vector<ProcessInfo*>* ps = c->processes->content();

    cout << std::left << setw(25) << "Process"
         << std::left << setw(15) << "Entries"
         << std::left << setw(15) << "Yield"
         << std::left << setw(15) << "Error" 
         << std::left << setw(15) << "Rel.Err." << endl;
    
    for (ProcessInfo* p : *ps)
    {
        double error;
        int from = 0;
        int to = p->histogram->GetNbinsX() + 1;
        int nentries = p->histogram->GetEntries();
        double integral = p->histogram->IntegralAndError(from, to, error, "");
        double eOverI = *(long*)(&integral) ? error / integral : 0.;
        cout << std::left << setw(25) << p->name 
             << std::left << setw(15) << nentries
             << std::left << setw(15) << integral
             << std::left << setw(15) << error 
             << std::left << setw(15) << eOverI << endl;
    }
}