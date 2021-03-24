/**
 * @brief: make this tool as a child of WorkspaceTool
 */

#ifndef RANKINGTOOL_H
#define RANKINGTOOL_H

#include "StatisticTool.h"
#include "WorkspaceTool.h"

static mutex gMutex;

class RankingEngine : public StatisticTool
{
public:
    RankingEngine(const WorkspaceInfo* info, const string& name="RankingEngine") 
        :  StatisticTool(name), m_cInfo(info) 
    {
        Tools::println("Initializing [%]", this->sToolName);
        RooMsgService::instance().setGlobalKillBelow(ERROR);
    }
    virtual ~RankingEngine() override
    {
        for (auto& p : m_fits)
        {
            delete p.second;
        }
        m_fits.clear();
    }
    RankingEngine(RankingEngine& old) = delete;
    RankingEngine& operator=(RankingEngine& old) = delete;

public:
    virtual void RunRanking()
    {
        vector<thread> vThr;

        auto run = [this](const string& sNP, const double nMode) 
        {
            lock_guard<mutex> lg(gMutex);
            string sPostFix = nMode > 0 ? "_Hi" : "_Lo";
            m_fits[sNP + sPostFix] = new WorkspaceTool(m_cInfo);
            m_fits[sNP + sPostFix]->FitWithFixedPara(sNP, m_fits["base"]->GetFittedNPs(), nMode, -1);
            // m_fits[sNP + sPostFix]->Check();
            m_mapAltPOIs[sNP + sPostFix] = m_fits[sNP + sPostFix]->GetCache(m_fits[sNP + sPostFix]->NameOfPOI());
        };

        m_fits["base"] = new WorkspaceTool(m_cInfo);
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

    map<WorkspaceTool::ePOI, double> GetFittedPOI()
    {
        return m_nPOI;
    }

    map<string, map<WorkspaceTool::ePOI, double>> GetFittedPOIWithFixedNPs()
    {
        return m_mapAltPOIs;
    }

protected:
    const WorkspaceInfo* m_cInfo;
    map<string, WorkspaceTool*> m_fits;
    map<WorkspaceTool::ePOI, double> m_nPOI;
    map<string, map<WorkspaceTool::ePOI, double>> m_mapAltPOIs;
};

struct RankingData
{
    RankingData(const string& np, double hi, double lo)
        : fixedNP(np), deltaMuByHi(hi), deltaMuByLo(lo) {}
    string fixedNP;
    double deltaMuByHi;
    double deltaMuByLo;
};

class RankingPlotter : public StatisticTool
{
public:
    RankingPlotter(const string& name="RankingPlotter") : StatisticTool(name)
    {

    }
    
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
                        >> m_nPOI[WorkspaceTool::ePOI::VALUE] 
                        >> m_nPOI[WorkspaceTool::ePOI::ERRORUP] 
                        >> m_nPOI[WorkspaceTool::ePOI::ERRORDOWN]; // symmetric of up due to Hesse
                }
                else
                {
                    iss >> tmp 
                        >> m_mapAltPOIs[tmp][WorkspaceTool::ePOI::VALUE] 
                        >> m_mapAltPOIs[tmp][WorkspaceTool::ePOI::ERRORUP] 
                        >> m_mapAltPOIs[tmp][WorkspaceTool::ePOI::ERRORDOWN]; // symmetric of up due to Hesse
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
                    m_vData.emplace_back(sNameOfNP, p.second[WorkspaceTool::ePOI::VALUE]-m_nPOI[WorkspaceTool::ePOI::VALUE], -999);
                }
                else 
                {
                    m_vData.emplace_back(sNameOfNP, -999, p.second[WorkspaceTool::ePOI::VALUE]-m_nPOI[WorkspaceTool::ePOI::VALUE]);
                }
            }
            else 
            {
                if (sDirection == "Hi")
                {
                    m_vData[m_mapLookUp[sNameOfNP]].deltaMuByHi = p.second[WorkspaceTool::ePOI::VALUE]-m_nPOI[WorkspaceTool::ePOI::VALUE];
                }
                else
                {
                    m_vData[m_mapLookUp[sNameOfNP]].deltaMuByLo = p.second[WorkspaceTool::ePOI::VALUE]-m_nPOI[WorkspaceTool::ePOI::VALUE];
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
    map<WorkspaceTool::ePOI, double> m_nPOI;
    map<string, map<WorkspaceTool::ePOI, double>> m_mapAltPOIs;
    vector<RankingData> m_vData;
    map<string, std::size_t> m_mapLookUp;
};

#endif // RANKINGTOOL_H
