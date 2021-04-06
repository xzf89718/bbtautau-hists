/**
 * This is for lephad channel
 */

#include "Config.h"
#include "Utils.h"
#include "SystCompTool.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>

using std::cout; 
using std::endl;
using std::clog;

void test_lephad_ttbarRew(const std::string& filename)
{
    BasicInfo* b = new BasicInfo("#sqrt{s} = 13 TeV", "L = 139 fb^{-1}");

    Regions* rs = new Regions();
    rs->add("2tag2pjet_0ptv_OS",     "2 b-tag, loose #tau, OS",        eRegionType::SR);

    Variables* vs_presel = new Variables();
    vs_presel->add("mBB",                  "m_{BB} [GeV]",                                      20);
    vs_presel->add("mMMC",                 "m_{#tau#tau} (MMC) [GeV]",                          20);
    vs_presel->add("mHH",                  "m_{HH} [GeV]",                                      50);
    vs_presel->add("dRBB",                 "#Delta R(B,B)",                                     5 );
    vs_presel->add("pTBB",                 "p_{T}^{b,b}",                                       20);
    vs_presel->add("dRTauTau",             "#Delta R(#tau_{had},#tau_{had})",                   5 );
    vs_presel->add("pTTauTau",             "p_{T}^{#tau,#tau}",                                 20);
    vs_presel->add("MET",                  "E_{T}^{miss} [GeV]",                                20);
    vs_presel->add("nJets",                "Number of Jets",                                    1 );
    vs_presel->add("Yield",                "Event Yield",                                       1 );
    vs_presel->add("pTB0",                 "Leading b-jet p_{T} [GeV]",                         20);
    vs_presel->add("pTB1",                 "Sub-leading b-jet p_{T} [GeV]",                     20);
    vs_presel->add("pTTau",                "#tau_{had} p_{T} [GeV]",                            20);
    vs_presel->add("pTLep",                "Lepton p_{T} [GeV]",                                20);
    vs_presel->add("pTTauLep",             "p_{T}^{#tau,lep}",                                  20);
    vs_presel->add("dRTauLep",             "#Delta R(#tau_{had},lep)",                          5 );

    Systematics* ss = new Systematics();
    // OK the name is misleading. It is ttbar with the nominal ttbar reweighting applied
    ss->add("TTBarNominal", "t#bar{t} ad-hoc reweight", eSystematicType::TwoSide);
    ss->add("TTBAR_REWEIGHT_NNLO", "t#bar{t} NNLO reweight", eSystematicType::TwoSide);

    CompInfo* info = new CompInfo();
    info->ratio_high = 1.38;
    info->ratio_low = 0.62;
    info->shape_only = false;

    for (VariableInfo* v : *(vs_presel->content()))
    {
        Processes* ps = new Processes();
        ps->add("ttbar",            "ttbar Nominal",        eProcessType::BKG,      eProcess::TTBAR,            "ttbar Nominal",        kBlack);

        Config* c = new Config(b, ps, rs, vs_presel, ss);
        c->load(filename, "Preselection");
        info->parameter = "Presel";
        c->updateHistogramPtr(rs->content()->front(), v);
        SystCompTool* ct = new SystCompTool(info);
        ct->output_path = "/tmp/bowenzhang/ttbarRew/";
        if (ct->check(c))
        {
            ct->manipulate(c);
            ct->makeYield(c, info->parameter);
            ct->paint(c);
            ct->run(c);
        }
        else 
        {
            clog << "Can not draw " << c->current_region->name << " " << c->current_variable->name << '\n';
        }

        delete ps;
        delete ct;
        delete c;
    }

    delete b;
    delete rs;
    delete vs_presel;
    delete info;

}