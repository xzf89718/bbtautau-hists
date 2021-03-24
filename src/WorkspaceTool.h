#ifndef WORKSPACETOOL_H
#define WORKSPACETOOL_H

#include <iostream>
#include <chrono>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <thread>
#include <mutex>
#include <algorithm>
#include <atomic>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "CommonInclude_WS.h"
#include "Utils_WS.h"
#include "Utils.h"

using std::string;
using std::map;
using std::set;
using std::pair;
using std::tuple;
using std::cout;
using std::clog;
using std::cerr;
using std::endl;
using std::for_each;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::ifstream;
using std::istringstream;

using std::chrono::steady_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

namespace fs = std::filesystem;

// Macros
#define MAP_UPDATE_PARA(map, v) \
    auto v1 = v->getErrorHi(); \
    auto v2 = v->getErrorLo(); \
    /*if (v1 < 0) { auto tmp = v1; v1 = v2; v2 = tmp; }*/ \
    map[v->GetName()] = std::make_tuple(v->getVal(), v1, v2);

#define PRINT_PARA_MAP(map) \
    Tools::println("%:", #map);\
    for (const auto& pp : map) \
    { \
        Tools::println("Name: %  =  %  [ %  % ]", \
        pp.first, std::get<0>(pp.second), std::get<1>(pp.second), std::get<2>(pp.second)); \
    }

#define ITX(f) _IterateAndDo(cParaSet, f);

struct Parameter
{
    Parameter() = default;
    string name;
    double val;
    double err_hi;
    double err_lo;
};

struct WorkspaceInfo
{
    WorkspaceInfo() = default;
    string path;
    string workspace_name;
    string config_name = "ModelConfig";
    string data_name = "obsData";
    bool use_asimov = true;
    double mu_asimov = 1.0;
};

class WorkspaceTool
{
public:
    WorkspaceTool(const WorkspaceInfo* info)
        : m_cInfo(info)
    {
        RooMsgService::instance().setGlobalKillBelow(ERROR);
        Load();
    };

    ~WorkspaceTool()
    {
        m_cFile->Close();
    }

    WorkspaceTool(WorkspaceTool& old) = delete;

    WorkspaceTool& operator=(WorkspaceTool& old) = delete;

public:
    void Check() {
        Tools::println("%", m_cWs);
        Tools::println("%", m_cSBModel->GetName());

        Tools::println("POIs:");
        PrintParametersOfInterest();
        Tools::println("NPs:");
        PrintNuisanceParameters();
    }

    void CheckParaMaps()
    {
        PRINT_PARA_MAP(m_mapNPsInit);
        PRINT_PARA_MAP(m_mapNPsFitted);
        PRINT_PARA_MAP(m_mapPOIsFitted);
    }

    string NameOfPOI()
    {
        return m_sPOIName;
    }

    enum class ePOI : uint8_t { VALUE, ERRORUP, ERRORDOWN };

private:
    void Load() {
        if (!fs::exists(fs::path(m_cInfo->path)))
        {
            throw std::runtime_error("Input does not exist");
        }
        m_cFile = TFile::Open(m_cInfo->path.c_str(), "READ");
        m_cWs = static_cast<RooWorkspace*>(m_cFile->Get(m_cInfo->workspace_name.c_str()));

        if (!m_cWs)
        {
            throw std::runtime_error("Workspace does not exist");
        }

        m_cSBModel = static_cast<ModelConfig*>(m_cWs->obj(m_cInfo->config_name.c_str()));
        m_cNPs = m_cSBModel->GetNuisanceParameters();
        m_cPOIs = m_cSBModel->GetParametersOfInterest();
        m_sPOIName = static_cast<RooRealVar*>(m_cPOIs->first())->GetName();
        m_cData = m_cWs->data(m_cInfo->data_name.c_str());

        if (m_cInfo->use_asimov) {
            Tools::println("Using Asimov data! with mu = %", m_cInfo->mu_asimov);
            static_cast<RooRealVar*>(m_cPOIs->first())->setVal(m_cInfo->mu_asimov);
            RooArgSet* allParams = m_cSBModel->GetPdf()->getParameters(*m_cData);
            RooArgSet globObs("globObs");
            RooAbsData* asimovData = AsymptoticCalculator::MakeAsimovData(*m_cSBModel, *allParams, globObs);
            m_cData = asimovData;
        }

        GetSetOfNPs(m_cNPs);
    }

protected:
    const WorkspaceInfo* m_cInfo;
    TFile* m_cFile;
    RooWorkspace* m_cWs;
    ModelConfig* m_cSBModel;
    const RooArgSet* m_cNPs;
    const RooArgSet* m_cPOIs;
    string m_sPOIName;
    RooAbsData* m_cData;
    map<string, tuple<double, double, double>> m_mapNPsInit; // Name, Val, Hi, Lo
    map<string, tuple<double, double, double>> m_mapNPsFitted; // Name, Val, Hi, Lo
    map<string, tuple<double, double, double>> m_mapPOIsFitted; // Name, Val, Hi, Lo
    set<string> m_setStrNPs;

// ============================================================================
// Useful Functions
// ============================================================================
private:
    template<typename Func>
    void _IterateAndDo(const RooArgSet*& cParaSet, Func pFunc)
    {
        TIterator* itParas = cParaSet->createIterator();
        RooRealVar* cPara = nullptr;
        while((cPara = (RooRealVar*)itParas->Next()))
            pFunc(cPara);
    }

    template<typename Func, typename ... Targs>
    void _IterateAndDo(const RooArgSet*& cParaSet, Func pFunc, Targs ... args)
    {
        TIterator* itParas = cParaSet->createIterator();
        RooRealVar* cPara = nullptr;
        while((cPara = (RooRealVar*)itParas->Next()))
            pFunc(cPara, args ...);
    }

    void _PrintParameters(const RooArgSet* cParaSet)
    {
        ITX([](const RooRealVar* v){
            Tools::println("Name: %  =  %  [ %  % ]",
                    v->GetName(), v->getVal(), v->getErrorHi(), v->getErrorLo());
        });
    }

    void UpdateMapNPsInit(const RooArgSet* cParaSet)
    {
        ITX([this](const RooRealVar* v){ MAP_UPDATE_PARA(m_mapNPsInit, v); });
    }

    void UpdateMapNPsFinal(const RooArgSet* cParaSet)
    {
        ITX([this](const RooRealVar* v){ MAP_UPDATE_PARA(m_mapNPsFitted, v); });
    }

    void UpdateMapPOIsFitted(const RooArgSet* cParaSet)
    {
        ITX([this](const RooRealVar* v){ MAP_UPDATE_PARA(m_mapPOIsFitted, v); });
    }

    void PrintNuisanceParameters()
    {
        _PrintParameters(m_cNPs);
    }

    void PrintParametersOfInterest()
    {
        _PrintParameters(m_cPOIs);
    }

    void GetSetOfNPs(const RooArgSet* cParaSet)
    {
        ITX([this](const RooRealVar* v){ m_setStrNPs.insert(string(v->GetName())); });
    }

// ============================================================================
// Fitting
// ============================================================================
private:
    // em. layers of snapshot, not sure
    void Fit(int8_t nLogLevel=-1) {
        RooRealVar* cPOI = static_cast<RooRealVar*>(m_cPOIs->first());
        auto sMinimizerType = ROOT::Math::MinimizerOptions::DefaultMinimizerType();
        Tools::println("POI [%] initial value is [%]", cPOI->GetName(), cPOI->getVal());
        Tools::println("Set [%] as minimizer", sMinimizerType);
        // Remove constant paras
        RooArgSet cConstrainParas;
        cConstrainParas.add(*m_cNPs);
        RooStats::RemoveConstantParameters(&cConstrainParas);
        auto timeStart = steady_clock::now();
        // >>> hack this <<<
        RooFitResult* cRes = m_cSBModel->GetPdf()->fitTo(
                    *m_cData, InitialHesse(false), Minos(false), Minimizer("Minuit", "Migrad"),
                    Strategy(1), PrintLevel(nLogLevel), Constrain(cConstrainParas), Save(true),
                    Offset(RooStats::IsNLLOffset()));
        auto timeEnd = steady_clock::now();

        bFitted = true;
        if (nLogLevel > 0)
            cRes->Print();
        Tools::println("Fit status [%], spent [%ms]",
                cRes->status(), duration_cast<milliseconds>(timeEnd-timeStart).count());
        Tools::println("POI [%] final value is [%  %  %]",
                cPOI->GetName(), cPOI->getVal(), cPOI->getErrorHi(), cPOI->getErrorLo());
    }

public:
    void FitAll(int8_t nLogLevel=-1) {
        // prefit paramters (not useful)
        UpdateMapNPsInit(m_cNPs);
        // Fitting
        Fit(nLogLevel);
        // Store results in maps and take snapshot
        UpdateMapNPsFinal(m_cNPs);
        UpdateMapPOIsFitted(m_cPOIs);
    }

    void FitWithFixedPara(const string& sPara,
                          const map<string, tuple<double, double, double>>& mapNPsFromFitAll,
                          double nMode, int8_t nLogLevel=-1)
    {
        const auto& tupleVal = mapNPsFromFitAll.at(sPara);
        Tools::println("Initial value: % | error_up % | error_down %",
                std::get<0>(tupleVal), std::get<1>(tupleVal), std::get<2>(tupleVal));
        double fFixedVal = std::get<0>(tupleVal);
        fFixedVal += nMode > 0 ? TMath::Abs(nMode) * std::get<1>(tupleVal) : TMath::Abs(nMode) * std::get<2>(tupleVal);
        RooRealVar* cNP = (RooRealVar*)m_cNPs->find(sPara.c_str());
        cNP->setVal(fFixedVal);
        cNP->setConstant(true);
        Fit(nLogLevel);
        UpdateMapNPsFinal(m_cNPs);
        UpdateMapPOIsFitted(m_cPOIs);
    }

public:
    map<string, tuple<double, double, double>> GetFittedNPs()
    {
        return m_mapNPsFitted;
    }

    map<string, tuple<double, double, double>> GetFittedPOIs()
    {
        return m_mapPOIsFitted;
    }

    set<string> GetNPs()
    {
        return m_setStrNPs;
    }

    map<WorkspaceTool::ePOI, double> GetCache(const string& nm)
    {
        tuple<double, double, double> tupleVals;
        if (m_mapNPsFitted.find(nm) != m_mapNPsFitted.end())
            tupleVals = m_mapNPsFitted.at(nm);
        else if (m_mapPOIsFitted.find(nm) != m_mapPOIsFitted.end())
            tupleVals = m_mapPOIsFitted.at(nm);

        map<WorkspaceTool::ePOI, double> res = {
            {WorkspaceTool::ePOI::VALUE,   std::get<0>(tupleVals)},
            {WorkspaceTool::ePOI::ERRORUP, std::get<1>(tupleVals)},
            {WorkspaceTool::ePOI::ERRORDOWN, std::get<2>(tupleVals)}
        };

        return std::move(res);
    }

public:
    bool bFitted = false;

};

#endif