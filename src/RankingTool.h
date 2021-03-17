/**
 * @todo: make this tool as a child of WorkspaceTool
 */

#ifndef RANKINGTOOL_H
#define RANKINGTOOL_H

#include <iostream>
#include <chrono>
#include <map>
#include <set>
#include <tuple>
#include <thread>
#include <mutex>
#include <algorithm>
#include <atomic>
#include <fstream>
#include <sstream>

#include "CommonInclude_WS.h"
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

class FitResult
{
public:
    FitResult(const string& fn, const string& wsn)
        : m_sRootFile(fn), m_sWorkspace(wsn)
    {
        RooMsgService::instance().setGlobalKillBelow(ERROR);
        Load();
    }

    ~FitResult()
    {
        m_cFile->Close();
    }

    FitResult(FitResult& old) = delete;

    FitResult& operator=(FitResult& old) = delete;

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

private:
    void Load(bool bAsimov=true, double fPOI=1.0) {
        m_cFile = TFile::Open(m_sRootFile.c_str(), "READ");
        m_cWs = static_cast<RooWorkspace*>(m_cFile->Get(m_sWorkspace.c_str()));
        m_cSBModel = static_cast<ModelConfig*>(m_cWs->obj("ModelConfig"));
        m_cNPs = m_cSBModel->GetNuisanceParameters();
        m_cPOIs = m_cSBModel->GetParametersOfInterest();
        m_sPOIName = static_cast<RooRealVar*>(m_cPOIs->first())->GetName();
        m_cData = m_cWs->data("obsData");

        if (bAsimov) {
            Tools::println("Using Asimov data! with mu = %", fPOI);
            static_cast<RooRealVar*>(m_cPOIs->first())->setVal(fPOI);
            RooArgSet* allParams = m_cSBModel->GetPdf()->getParameters(*m_cData);
            RooArgSet globObs("globObs");
            RooAbsData* asimovData = AsymptoticCalculator::MakeAsimovData(*m_cSBModel, *allParams, globObs);
            m_cData = asimovData;
        }

        GetSetOfNPs(m_cNPs);
    }

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

protected:
    string m_sRootFile;
    string m_sWorkspace;
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

public:
    bool bFitted = false;

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

    map<FitResult::ePOI, double> GetCache(const string& nm)
    {
        tuple<double, double, double> tupleVals;
        if (m_mapNPsFitted.find(nm) != m_mapNPsFitted.end())
            tupleVals = m_mapNPsFitted.at(nm);
        else if (m_mapPOIsFitted.find(nm) != m_mapPOIsFitted.end())
            tupleVals = m_mapPOIsFitted.at(nm);

        map<FitResult::ePOI, double> res = {
            {FitResult::ePOI::VALUE,   std::get<0>(tupleVals)},
            {FitResult::ePOI::ERRORUP, std::get<1>(tupleVals)},
            {FitResult::ePOI::ERRORDOWN, std::get<2>(tupleVals)}
        };

        return std::move(res);
    }
};

class Engine
{
public:
    Engine()
    {
        RooMsgService::instance().setGlobalKillBelow(ERROR);
    }
    virtual ~Engine()
    {
        for (auto& p : m_fits)
        {
            delete p.second;
        }
        m_fits.clear();
    }
    Engine(Engine& old) = delete;
    Engine& operator=(Engine& old) = delete;

public:
    virtual void RunRanking()
    {
        vector<thread> vThr;

        auto run = [this](const std::string& sNP, const double nMode) 
        {
            lock_guard<mutex> lg(this->m);
            string sPostFix = nMode > 0 ? "_Hi" : "_Lo";
            m_fits[sNP + sPostFix] = new FitResult(m_sFileName, m_sWorkspaceName);
            m_fits[sNP + sPostFix]->FitWithFixedPara(sNP, m_fits["base"]->GetFittedNPs(), nMode, -1);
            // m_fits[sNP + sPostFix]->Check();
            m_mapAltPOIs[sNP + sPostFix] = m_fits[sNP + sPostFix]->GetCache(m_fits[sNP + sPostFix]->NameOfPOI());
        };

        m_fits["base"] = new FitResult(m_sFileName, m_sWorkspaceName);
        Tools::println(">> All: ");
        m_fits["base"]->FitAll(-1);
        m_nPOI = m_fits["base"]->GetCache(m_fits["base"]->NameOfPOI());
        m_fits["base"]->Check();
        Tools::println(">> Fixed NP: ");
        for (const auto& sNP: m_fits["base"]->GetNPs())
        {
            for (double nMode : {1.0f, -1.0f})
            {
                vThr.emplace_back(run, sNP, nMode);
            }
        }
        for_each(vThr.begin(), vThr.end(), [](thread& t){ t.join(); });
    }

    map<FitResult::ePOI, double> GetFittedPOI()
    {
        return m_nPOI;
    }

    map<string, map<FitResult::ePOI, double>> GetFittedPOIWithFixedNPs()
    {
        return m_mapAltPOIs;
    }

protected:
    map<string, FitResult*> m_fits;
    // std::string m_sFileName = "/scratchfs/atlas/zhangbw/ResolvedStatAna/WSMaker_Group/output/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_2HDM_MVA_300/workspaces/combined/300.root";
    std::string m_sFileName = "/scratchfs/atlas/zhangbw/ResolvedStatAna/WSMaker_Group/output/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_SM_MVA_0/workspaces/combined/0.root";
    std::string m_sWorkspaceName = "combined";
    map<FitResult::ePOI, double> m_nPOI;
    map<string, map<FitResult::ePOI, double>> m_mapAltPOIs;

private:
    mutex m;
};

struct RankingData
{
    RankingData(const string& np, double hi, double lo)
        : fixedNP(np), deltaMuByHi(hi), deltaMuByLo(lo) {}
    string fixedNP;
    double deltaMuByHi;
    double deltaMuByLo;
};

class Plotter 
{
public:
    Plotter() = default;
    
    void LoadFromTxt(const std::string& sFileName)
    {
        ifstream input(sFileName, std::ios::in);
        if (input.is_open())
        {
            string sLine;
            unsigned nRow = 0;
            while (getline(input, sLine))
            {
                istringstream iss(sLine);
                string tmp;
                if (nRow == 0) 
                {
                    Tools::println("Starting!");
                }
                else if (nRow == 1)
                {
                    iss >> tmp 
                        >> m_nPOI[FitResult::ePOI::VALUE] 
                        >> m_nPOI[FitResult::ePOI::ERRORUP] 
                        >> m_nPOI[FitResult::ePOI::ERRORDOWN]; // symmetric of up due to Hesse
                }
                else
                {
                    iss >> tmp 
                        >> m_mapAltPOIs[tmp][FitResult::ePOI::VALUE] 
                        >> m_mapAltPOIs[tmp][FitResult::ePOI::ERRORUP] 
                        >> m_mapAltPOIs[tmp][FitResult::ePOI::ERRORDOWN]; // symmetric of up due to Hesse
                }
                nRow++;
            }
        }

        for (auto& p : m_mapAltPOIs)
        {
            string sNameOfNP = p.first.substr(0, p.first.length()-3);
            string sDirection = p.first.substr(p.first.length()-2);
            if (m_mapLookUp.find(sNameOfNP) == m_mapLookUp.end()) 
            {
                m_mapLookUp[sNameOfNP] = m_vData.size();
                if (sDirection == "Hi")
                {
                    m_vData.emplace_back(sNameOfNP, p.second[FitResult::ePOI::VALUE]-m_nPOI[FitResult::ePOI::VALUE], -999);
                }
                else 
                {
                    m_vData.emplace_back(sNameOfNP, -999, p.second[FitResult::ePOI::VALUE]-m_nPOI[FitResult::ePOI::VALUE]);
                }
            }
            else 
            {
                if (sDirection == "Hi")
                {
                    m_vData[m_mapLookUp[sNameOfNP]].deltaMuByHi = p.second[FitResult::ePOI::VALUE]-m_nPOI[FitResult::ePOI::VALUE];
                }
                else
                {
                    m_vData[m_mapLookUp[sNameOfNP]].deltaMuByLo = p.second[FitResult::ePOI::VALUE]-m_nPOI[FitResult::ePOI::VALUE];
                }
            }
        }

        std::sort(m_vData.begin(), m_vData.end(), [](RankingData& a, RankingData& b) {
            return std::fabs(a.deltaMuByHi - a.deltaMuByLo) > std::fabs(b.deltaMuByHi - b.deltaMuByLo);
        });

        for (auto& info : m_vData)
        {
            Tools::println("% % %", info.fixedNP, info.deltaMuByHi, info.deltaMuByLo);
        }
    }

    void Draw(const string output, uint8_t nNPs=15)
    {
        gROOT->SetStyle("ATLAS");
        gStyle->SetErrorX(0.5);

        TCanvas* c1 = new TCanvas("c", "", 900, 1500);
        c1->SetLeftMargin(0.20);
        c1->SetBottomMargin(0.60);
        TH1* h_one = new TH1D("one", "", nNPs, 0, nNPs);
        TH1* h_hi = new TH1D("hi", "", nNPs, 0, nNPs);
        TH1* h_lo = new TH1D("lo", "", nNPs, 0, nNPs);
        for (std::size_t i = 1; i <= nNPs; i++) {
            h_hi->GetXaxis()->SetBinLabel(i, m_vData[i-1].fixedNP.c_str());
            h_hi->SetBinContent(i, m_vData[i-1].deltaMuByHi);
            h_lo->SetBinContent(i, m_vData[i-1].deltaMuByLo);
            h_one->SetBinContent(i, 0);
        }

        h_hi->SetBarWidth(0.90);
        h_lo->SetBarWidth(0.90);

        h_hi->SetLineColor(kRed);
        h_hi->SetLineWidth(1);
        h_hi->SetFillColor(kRed);
        h_hi->SetFillStyle(3245);
        h_hi->GetXaxis()->LabelsOption("v");
        h_hi->GetYaxis()->SetRangeUser(-0.005, 0.005);
        // h_hi->GetYaxis()->SetRangeUser(-0.5, 0.5);
        h_hi->GetYaxis()->SetTitleOffset(2);
        h_hi->GetYaxis()->SetTitle("#Delta#mu");
        h_lo->SetLineColor(kBlue);
        h_lo->SetLineWidth(1);
        h_lo->SetFillColor(kBlue);
        h_lo->SetFillStyle(3254);

        h_one->SetLineColor(kRed+1);
        h_one->SetLineWidth(3);

        h_hi->Draw("hist b");
        h_lo->Draw("hist b same");
        h_one->Draw("hist same");

        TLegend* legend = new TLegend(0.50, 0.85, 0.80, 0.93);
        legend->SetTextFont(42);
        legend->SetFillStyle(0);
        legend->SetBorderSize(0);
        legend->SetTextSize(0.035);
        legend->SetTextAlign(12);
        legend->AddEntry(h_hi, "+1#sigma post-fit impact on #mu");
        legend->AddEntry(h_lo, "-1#sigma post-fit impact on #mu");
        legend->Draw("same");

        TLatex *text = new TLatex();
        text->SetNDC();
        text->SetTextFont(72);
        text->SetTextSize(0.050);
        text->DrawLatex(0.21, 0.96, "ATLAS");
        text->SetTextFont(42);
        text->DrawLatex(0.21 + 0.18, 0.96, "Internal");
        text->SetTextSize(0.045);
        text->SetTextSize(0.040);
        text->DrawLatex(0.60, 0.96, "Fit to Asimov (#mu=0.1)");

        c1->SaveAs(output.c_str());

        delete h_hi;
        delete h_lo;
        delete c1;
    }

private:
    map<FitResult::ePOI, double> m_nPOI;
    map<string, map<FitResult::ePOI, double>> m_mapAltPOIs;
    vector<RankingData> m_vData;
    map<string, std::size_t> m_mapLookUp;
};

#endif // RANKINGTOOL_H
