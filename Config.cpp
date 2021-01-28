#include "Config.h"

#include <iterator>
#include <iostream>
#include <exception>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

BasicInfo::BasicInfo(const string& ecm, const string& lumi) noexcept
    : ecm(ecm), luminosity(lumi)
{

}

Config::Config(const BasicInfo* b, const Processes* ps, const Regions* rs, const Variables* vs) noexcept
    : basic(b), processes(ps), regions(rs), variables(vs)
{
    m_fin = nullptr;
    m_dir = "";
    m_loaded = false;
}

Config::~Config() noexcept
{
    m_fin->Close();
}

void Config::load(const string& fn, const string& dir)
{
    if (!fs::exists(fs::path(fn)))
    {
        throw std::runtime_error("Input does not exist");
    }
    m_fin.reset(TFile::Open(fn.c_str(), "read"));
    if (!m_fin)
    {
        throw std::runtime_error("Input does not exist");
    }
    m_dir = dir;
    if (!m_fin->GetListOfKeys()->Contains(dir.c_str()) && dir != "")
    {
        m_fin->ls();
        throw std::runtime_error(dir + " does not exist in the input file");
    }
    m_loaded = true;
}


void Config::updateHistogramPtr(RegionInfo* r, VariableInfo* v)
{
    if (!m_loaded)
    {
        throw std::runtime_error("You must load first");
    }

    current_region = r;
    current_variable = v;

    TDirectory* d = nullptr;
    if (m_dir != "")
        d = (TDirectory*)m_fin->Get(m_dir.c_str());
    else
        d = (TDirectory*)m_fin->GetMotherDir();

    for (ProcessInfo* p : *(processes->content()))
    {
        const std::string& fullname = Utils::histString(p, r, v);
        if (d->GetListOfKeys()->Contains(fullname.c_str()))
        {
            TH1* h = (TH1*)d->Get(fullname.c_str());
            Utils::histAssign(h, p, r, v);
        }
        else
        {
            clog << fullname << " is not in " << m_dir << " (skip it)\n";
        }
        // else that p.histogram will remain as nullptr
        // later when make plot this should checked
    }
    
}

