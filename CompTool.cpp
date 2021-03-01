#include "CompTool.h"
#include "CommonInclude.h"

#include <sstream>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

CompTool::CompTool(const CompInfo* info)
    : HistTool(), m_info(info)
{

}

CompTool::~CompTool()
{

}

bool CompTool::check(const Config* c) const
{
    if (!HistTool::check(c))
        return false;

    vector<ProcessInfo*>* ps = c->processes->content();

    return ps->size() > 1;
}

void CompTool::paint(const Config* c) const
{
    vector<ProcessInfo*>* ps = c->processes->content();
    for_each(ps->begin(), ps->end(), [&c](ProcessInfo* p) {
        if (c->current_variable->binning) {
            TH1* rebinned = p->histogram->Rebin(c->current_variable->n_bins, p->histogram->GetName(), c->current_variable->binning);
            p->histogram = (TH1*)rebinned->Clone();
        } else {
            p->histogram->Rebin(c->current_variable->n_rebin);
        }
        p->histogram->SetLineWidth(2);
        p->histogram->SetLineStyle(1);
        p->histogram->SetMarkerColor(p->color);
        p->histogram->SetMarkerSize(0);
        p->histogram->SetLineColor(p->color);
    });
}

void CompTool::run(const Config* c) const
{
    if (!fs::exists(fs::path(output_path)))
    {
        throw std::runtime_error("Output path does not exist");
    }
    
    vector<ProcessInfo*>* ps = c->processes->content();

    gROOT->SetStyle("ATLAS");
    gStyle->SetErrorX(0.5);

    TCanvas* c1 = new TCanvas("c", "", 900, 1200);
    c1->SetRightMargin(1.6 * c1->GetRightMargin());

    TPad* upper_pad = new TPad("upper_pad", "", 0, 0.5, 1, 1);
    upper_pad->SetRightMargin(1.6 * upper_pad->GetRightMargin());
    upper_pad->SetBottomMargin(0.03);
    upper_pad->SetTickx(false);
    upper_pad->SetTicky(false);
    upper_pad->SetLogx(m_info->logx);
    upper_pad->SetLogy(m_info->logy);
    upper_pad->Draw();

    TPad* lower_pad = new TPad("lower_pad", "", 0, 0, 1, 0.5);
    lower_pad->SetRightMargin(1.6 * lower_pad->GetRightMargin());
    lower_pad->SetTopMargin(0);
    lower_pad->SetBottomMargin(0.14);
    lower_pad->SetGridy();
    lower_pad->SetLogx(m_info->logx);
    lower_pad->Draw();

    upper_pad->cd();

    if (m_info->shape_only) {
        ps->front()->histogram->Scale(1.0 / ps->front()->histogram->Integral());
    } else {
        ps->front()->histogram->Scale(ps->front()->norm_factor);
    }
    TH1* base = ps->front()->histogram;
    base->Draw("HIST E1");
    base->GetXaxis()->SetLabelSize(0);
    base->GetXaxis()->SetTitleSize(0);
    base->GetXaxis()->SetTitleOffset(1.3);
    base->GetYaxis()->SetTitle("Events");
    base->GetYaxis()->SetLabelSize(0.04);
    base->GetYaxis()->SetTitleSize(0.045);
    base->SetMaximum(base->GetMaximum() * 1.4);
    base->GetYaxis()->ChangeLabel(1, -1, 0);

    for_each(ps->begin()+1, ps->end(), [this](const ProcessInfo* p) {
        if (m_info->shape_only) {
            p->histogram->Scale(1.0 / p->histogram->Integral());
        } else {
            p->histogram->Scale(p->norm_factor);
        }
        p->histogram->Draw("HIST E1 SAME"); });

    double y = 0.92 - 0.05 * (ps->size() + 1);
    TLegend* legend = new TLegend(0.60, y, 0.90, 0.92);
    legend->SetTextFont(42);
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);
    legend->SetTextSize(0.035);
    legend->SetTextAlign(12);

    for_each(ps->begin(), ps->end(), [&legend](const ProcessInfo* p) {
        legend->AddEntry(p->histogram, p->name_tex.c_str(), "lep"); });

    legend->Draw("SAME");

    TLatex *text = new TLatex();
    text->SetNDC();
    if (m_info->atlas) {
        text->SetTextFont(72);
        text->SetTextSize(0.050);
        text->DrawLatex(0.20, 0.86, "ATLAS");
        text->SetTextFont(42);
        text->DrawLatex(0.20 + 0.12, 0.86, m_info->atlas_label);
        text->SetTextSize(0.045);
    }
    ostringstream oss{c->basic->ecm};
    text->DrawLatex(0.20, 0.80, oss.str().c_str());
    text->SetTextSize(0.040);
    text->DrawLatex(0.20, 0.74, ps->front()->current_region->name_tex.c_str());

    lower_pad->cd();
    double resize = 1;

    TH1* err = (TH1*)base->Clone();
    TH1* base_scale = (TH1*)base->Clone();
    for (int i = 0; i < base_scale->GetNbinsX() + 2; ++i)
    {
        base_scale->SetBinError(i, 0.0);
    }
    err->Divide(base_scale);
    err->SetFillStyle(3254);
    err->SetFillColor(kGray + 3);
    err->SetMarkerSize(0);
    err->SetName("Unc.");
    err->GetXaxis()->SetTitle(ps->front()->current_variable->name_tex.c_str());
    err->GetXaxis()->SetTitleOffset(1.2 * resize);
    err->GetXaxis()->SetTitleSize(0.045 * resize);
    err->GetXaxis()->SetLabelSize(0.04 * resize);
    err->GetYaxis()->SetTitle(m_info->ratio_tex.c_str());
    err->GetYaxis()->SetTitleOffset(1.2 * resize);
    err->GetYaxis()->SetTitleSize(0.045 * resize);
    err->GetYaxis()->SetLabelSize(0.04 * resize);
    err->GetYaxis()->SetNdivisions(505);
    err->SetMinimum(m_info->ratio_low);
    err->SetMaximum(m_info->ratio_high);
    err->Draw("E2");
    for_each(ps->begin()+1, ps->end(), [&base](const ProcessInfo* p) {
        TH1* rat = (TH1*)p->histogram->Clone();
        rat->Divide(base);
        rat->Draw("E0 SAME"); });

    ostringstream oss_out;
    oss_out << output_path << "/" 
            << c->current_region->name << "_"
            << c->current_variable->name << "_"
            << m_info->parameter << ".png";
    c1->Update();
    c1->SaveAs(oss_out.str().c_str());

    delete upper_pad;
    delete lower_pad;
    delete legend;
    delete text;
    delete c1;
}
