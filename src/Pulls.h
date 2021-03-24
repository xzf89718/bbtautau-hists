#ifndef PULLS_H
#define PULLS_H

#include "StatisticTool.h"
#include "WorkspaceTool.h"

#include <fstream>

using std::ofstream;

class PullsEngine : public StatisticTool
{
public:
    PullsEngine(const WorkspaceInfo* info, const string& name="PullsEngine") 
        :  StatisticTool(name), m_cInfo(info) 
    {
        Tools::println("Initializing [%]", this->sToolName);
        RooMsgService::instance().setGlobalKillBelow(ERROR);
    }
    virtual ~PullsEngine() override
    {
        delete m_cTool;
    }
    PullsEngine(PullsEngine& old) = delete;
    PullsEngine& operator=(PullsEngine& old) = delete;

public:
    virtual void Execute() override 
    {
        m_cTool = new WorkspaceTool(m_cInfo);
        Tools::println(">> All: ");
        m_cTool->FitAll(-1);
        m_mapFittedNPs = m_cTool->GetFittedNPs();
    }

public:
    void WriteToTxt(const string& outname)
    {
        ofstream output(outname);
        for (auto& np : m_mapFittedNPs)
        {
            output << OUTPUT_TABLE_4(
                np.first,
                std::get<0>(np.second),
                std::get<1>(np.second),
                std::get<2>(np.second));
        }
    }

private:
    const WorkspaceInfo* m_cInfo;
    WorkspaceTool* m_cTool;
    map<string, tuple<double, double, double>> m_mapFittedNPs;
};

class PullsPlotter : public StatisticTool
{
public:
    PullsPlotter(const string& name="PullsPlotter") : StatisticTool(name)
    {

    }
    
    void LoadFromTxt(const std::string& sFileName)
    {
        m_fCounter = 0.5;
        constexpr double offset = 0.0; // maybe useful when making comparison plots
        ifstream input(sFileName, std::ios::in);
        if (input.is_open())
        {
            string sLine;
            while (getline(input, sLine))
            {
                istringstream iss(sLine);
                Parameter par;
                iss >> par.name >> par.val >> par.err_hi >> par.err_lo;
                if (par.name.find("gamma") == std::string::npos)
                {
                    vLabels.push_back(par.name);
                    vX.push_back(m_fCounter + offset);
                    vX_ErrHi.push_back(0.0);
                    vX_ErrLo.push_back(0.0);
                    vY.push_back(par.val);
                    vY_ErrHi.push_back(std::fabs(par.err_hi));
                    vY_ErrLo.push_back(std::fabs(par.err_lo));
                    m_fCounter += 1.0;
                }
            }
        }
    }

    void Draw(const string output)
    {
        gROOT->SetStyle("ATLAS");
        gStyle->SetErrorX(0.5);

        TCanvas* c1 = new TCanvas("c", "", 1400, 800);
        c1->SetBottomMargin(0.50);
        c1->SetLeftMargin(0.05);
        c1->SetRightMargin(0.025);

        TMultiGraph* mg = new TMultiGraph();
        
        double x[2] = {0.0, m_fCounter - 0.5};
        double y[2] = {0.0, 0.0};
        double ex[2] = {0.0, 0.0};
        double ey_two[2] = {2.0, 2.0};
        double ey_one[2] = {1.0, 1.0};
        TGraphErrors* g_two = new TGraphErrors(2, x, y, ex, ey_two);
        TGraphErrors* g_one = new TGraphErrors(2, x, y, ex, ey_one);
        TGraphAsymmErrors* g_nps = new TGraphAsymmErrors(vLabels.size(), 
            &vX[0], &vY[0], &vX_ErrHi[0], &vX_ErrLo[0], &vY_ErrHi[0], &vY_ErrLo[0]);

        g_two->SetLineWidth(0);
        g_two->SetFillColor(kYellow+2);
        g_one->SetLineWidth(0);
        g_one->SetFillColor(kGreen+2);
        g_nps->SetMarkerSize(1);
        g_nps->SetMarkerColor(kBlack);
        g_nps->SetMarkerStyle(20);

        mg->Add(g_two, "E3");
        mg->Add(g_one, "E3");
        mg->Add(g_nps, "PE1");

        mg->Draw("SAME");

        mg->GetYaxis()->SetTitle("Pull");
        mg->GetYaxis()->SetTitleOffset(0.5);
        mg->GetYaxis()->SetTitleSize(0.03);
        mg->GetYaxis()->SetLabelSize(0.03);
        mg->GetYaxis()->SetRangeUser(-4.2, 4.2);
        mg->GetXaxis()->SetRangeUser(0., m_fCounter - 0.5);
        mg->GetXaxis()->LabelsOption("v");
        mg->GetXaxis()->SetLabelOffset(0.01);
        mg->GetXaxis()->SetLabelSize(0.02);
        for (std::size_t i = 1; i <= vLabels.size(); ++i)
        {
            cout << i << ",  " << vLabels.at(i-1).c_str() << endl;
            auto ibin = mg->GetXaxis()->FindBin((double)i - 0.5);
            mg->GetXaxis()->SetBinLabel(ibin, vLabels.at(i-1).c_str());
        }

        TLegend* legend = new TLegend(0.75, 0.85, 0.90, 0.93);
        legend->SetTextFont(42);
        legend->SetFillStyle(0);
        legend->SetBorderSize(0);
        legend->SetTextSize(0.035);
        legend->SetTextAlign(12);
        legend->AddEntry(g_one, "1#sigma");
        legend->AddEntry(g_two, "2#sigma");
        legend->Draw("same");

        TLatex *text = new TLatex();
        text->SetNDC();
        text->SetTextFont(72);
        text->SetTextSize(0.050);
        text->DrawLatex(0.20, 0.96, "ATLAS");
        text->SetTextFont(42);
        text->DrawLatex(0.20 + 0.1, 0.96, "Internal");
        text->SetTextSize(0.045);
        text->SetTextSize(0.040);
        text->DrawLatex(0.75, 0.96, "Fit to Data");

        c1->SaveAs(output.c_str());

        delete g_nps;
        delete g_two;
        delete g_one;
        delete mg;
        delete legend;
        delete text;
        delete c1;
    }

public:
    float m_fCounter;
    std::vector<std::string> vLabels;
    std::vector<double> vX;
    std::vector<double> vX_ErrHi;
    std::vector<double> vX_ErrLo;
    std::vector<double> vY;
    std::vector<double> vY_ErrHi;
    std::vector<double> vY_ErrLo;
};

#endif