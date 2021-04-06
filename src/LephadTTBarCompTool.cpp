#include "LephadTTBarCompTool.h"

#include "CommonInclude.h"
#include "Utils.h"

using namespace std;

void LephadTTBarCompTool::paint(const Config* c) const
{
  if (!m_isCombined)
  {
    throw runtime_error("Must combine reweight systematics first!");
  }
  CompTool::paint(c);

    vector<ProcessInfo*>* ps = c->processes->content();
    auto& p = ps->front();
    auto& systs = ps->front()->systematic_histograms;
    auto& nom = systs.at("ttbar reweight"); 
    auto& nomSample = ps->front()->histogram;
    nom->SetLineStyle(1);
    nom->SetLineColor(kGreen+2);
    nom->SetMarkerColor(kGreen+2);
    nomSample->SetLineColor(kBlack);
    nomSample->SetMarkerColor(kBlack);

    for (auto& pp : p->systematic_histograms)
    {
      if (pp.first == "ttbar reweight")
      {
        continue;
      }
      pp.second->SetLineWidth(2);
      pp.second->SetLineStyle(2);
      pp.second->SetMarkerSize(0);

      if (pp.first.find("COMBINED_REWEIGHT") != string::npos)
      {
        pp.second->SetLineColor(kYellow + 1);
      }

      else if (pp.first.find("COMBINED_SCALE_RADI") != string::npos)
      {
        pp.second->SetLineColor(kMagenta + 1);
      }

      else 
      {
        cerr << "You shouldn't be here: " << pp.first << '\n';
      }

    }
}

void LephadTTBarCompTool::combineReweightSysts(const Config* c)
{
  vector<ProcessInfo*>* ps = c->processes->content();
  auto& systs = ps->front()->systematic_histograms;
  auto& nom = systs.at("TTBarNominal__1up");
  auto& nomSample = ps->front()->histogram;

  TH1F* h_up = (TH1F*)nom->Clone("COMBINED_REWEIGHT__1up");
  TH1F* h_down = (TH1F*)nom->Clone("COMBINED_REWEIGHT__1down");

  TH1F* th_up = (TH1F*)nom->Clone("COMBINED_SCALE_RADI__1up");
  TH1F* th_down = (TH1F*)nom->Clone("COMBINED_SCALE_RADI__1down");

  for (int i = 1; i <= nom->GetNbinsX(); ++i) {
    float uncSquared{0.f};
    h_up->SetBinError(i, 0.f);
    h_down->SetBinError(i, 0.f);
    for (auto& pp : systs) {
      if (i == 1 && pp.first.find("TTBarReweight") == std::string::npos) {
        clog << pp.first << " is not ttbar reweight uncertainty (skip it)\n";
        continue;
      }
      float diff = pp.second->GetBinContent(i) - nom->GetBinContent(i);
      uncSquared += diff * diff;
    }
    float unc = std::sqrt(uncSquared);
    h_up->SetBinContent(i, nom->GetBinContent(i) + unc);
    h_down->SetBinContent(i, nom->GetBinContent(i) - unc);
  }

  for (int i = 1; i <= nomSample->GetNbinsX(); ++i) {
    float uncSquared{0.f};
    th_up->SetBinError(i, 0.f);
    th_down->SetBinError(i, 0.f);
    for (auto& pp : systs) {
      if (i == 1 && pp.first.find("TTBarReweight") != std::string::npos) {
        clog << pp.first << " is ttbar reweight uncertainty (skip it)\n";
        continue;
      }
      float diff = pp.second->GetBinContent(i) - nomSample->GetBinContent(i);
      uncSquared += diff * diff;
    }
    float unc = std::sqrt(uncSquared);
    th_up->SetBinContent(i, nomSample->GetBinContent(i) + unc);
    th_down->SetBinContent(i, nomSample->GetBinContent(i) - unc);
  }

  systs.clear();
  systs["ttbar reweight"] = (TH1*)nom->Clone("TTBarNominal");

  Utils::histAssignSyst(h_up, ps->front(), "COMBINED_REWEIGHT__1up");
  Utils::histAssignSyst(h_down, ps->front(), "COMBINED_REWEIGHT__1down");
  Utils::histAssignSyst(th_up, ps->front(), "COMBINED_SCALE_RADI__1up");
  Utils::histAssignSyst(th_down, ps->front(), "COMBINED_SCALE_RADI__1down");

  m_isCombined = true;
}
