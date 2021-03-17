#include "Config.h"
#include "Utils.h"
#include "DrawStackTool.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>

using std::cout; 
using std::endl;
using std::clog;

void test_zifeng_hadhad()
{
        BasicInfo* b = new BasicInfo("#sqrt{s} = 13 TeV", "L=139fb^{-1}");

        Regions* rs = new Regions();
        rs->add("2tag2pjet_0ptv_LL_OS",     "2 b-tag, 2 loose #tau, OS",        eRegionType::SR);
        rs->add("1tag2pjet_0ptv_LL_OS",     "1 b-tag, 2 loose #tau, OS",        eRegionType::CR);
        rs->add("2tag2pjet_0ptv_LL_SS",     "2 b-tag, 2 loose #tau, SS",        eRegionType::CR);
        rs->add("1tag2pjet_0ptv_LL_SS",     "1 b-tag, 2 loose #tau, SS",        eRegionType::CR);

        Variables* vs = new Variables();
        vs->add("mBB",                  "m_{BB} [GeV]",                                      10);
        vs->add("mMMC",                 "m_{#tau#tau} (MMC) [GeV]",                          12);
        vs->add("mHH",                  "m_{HH} [GeV]",                                      12);
        vs->add("mHHScaled",            "m_{HH} (Scaled) [GeV]",                             12);
        vs->add("dRBB",                 "#Delta R(B,B)",                                     4 );
        vs->add("pTBB",                 "p_{T}^{b,b}",                                       10);
        vs->add("dRTauTau",             "#Delta R(#tau_{had},#tau_{had})",                   4 );
        vs->add("pTTauTau",             "p_{T}^{#tau,#tau}",                                 10);
        vs->add("dPhiBBTauTau",         "#Delta #phi{bb,#tau#tau}",                          4 );
        vs->add("dRBBTauTau",           "#Delta R{bb,#tau#tau}",                             5 );
        vs->add("pTBalance",            "p_{T}^{b,b} / p_{T}^{#tau,#tau}",                   5 );
        vs->add("MET",                  "E_{T}^{miss} [GeV]",                                10);
        vs->add("nJets",                "Number of Jets",                                    1 );
        vs->add("Yield",                "Event Yield",                                       1 );
        vs->add("metSig",               "MET significance",                                  2 );
        vs->add("metSigPU",             "MET significance (pile-up)",                        2 );
        vs->add("Jet0Pt",               "Leading b-jet p_{T} (before corr.) [GeV]",          10);
        vs->add("Jet1Pt",               "Sub-leading b-jet p_{T} (before corr.) [GeV]",      10);
        vs->add("pTB0",                 "Leading b-jet p_{T} [GeV]",                         10);
        vs->add("pTB1",                 "Sub-leading b-jet p_{T} [GeV]",                     10);
        vs->add("LeadJetPt",            "Leading jet p_{T} [GeV]",                           10);
        vs->add("SubleadJetPt",         "Sub-leading jet p_{T} [GeV]",                       10);
        vs->add("LeadJetEta",           "Leading jet #eta",                                  2 );
        vs->add("SubleadJetEta",        "Sub-leading jet #eta",                              2 );
        vs->add("Jet0Eta",              "Leading b-jet #eta (before corr.)",                 2 );
        vs->add("Jet1Eta",              "Sub-leading b-jet #eta (before corr.)",             2 );
        vs->add("Tau0Pt",               "Leading #tau_{had} p_{T} [GeV]",                    10);
        vs->add("Tau1Pt",               "Sub-leading #tau_{had} p_{T} (before corr.) [GeV]", 10);
        vs->add("Tau0Eta",              "Leading #tau_{had} #eta",                           2 );
        vs->add("Tau1Eta",              "Sub-leading #tau_{had} #eta",                       2 );
        vs->add("Tau0Ntrk",             "Leading #tau_{had} N_{tracks}",                     1 );
        vs->add("Tau1Ntrk",             "Sub-leading #tau_{had} N_{tracks}",                 1 );
        vs->add("dPhiTauTau",           "#Delta #phi (#tau_{had},#tau_{had})",               4 );

        DrawStackInfo* info = new DrawStackInfo();
        info->blind = false;
        info->ratio_high = 1.82;
        info->ratio_low = 0.18;
        info->signal_scale = 100;

        for (VariableInfo* v : *(vs->content()))
        {
                Processes* ps = new Processes();
                ps->add("data",         "data",                         eProcessType::DATA,     eProcess::DATA,         "data",                         kBlack);
                ps->add("Zttbb",        "Z#tau#tau + bb",               eProcessType::BKG,      eProcess::ZllHF,        "Z+hf",                         kBlue);
                ps->add("Zttbc",        "Z#tau#tau + bc",               eProcessType::BKG,      eProcess::ZllHF,        "Z+hf",                         kBlue);
                ps->add("Zttbl",        "Z#tau#tau + bl",               eProcessType::BKG,      eProcess::ZllLF,        "Z+lf",                         kBlue-2);
                ps->add("Zttcc",        "Z#tau#tau + cc",               eProcessType::BKG,      eProcess::ZllHF,        "Z+hf",                         kBlue);
                ps->add("Zttcl",        "Z#tau#tau + cl",               eProcessType::BKG,      eProcess::ZllLF,        "Z+lf",                         kBlue-2);
                ps->add("Zttl",         "Z#tau#tau + l",                eProcessType::BKG,      eProcess::ZllLF,        "Z+lf",                         kBlue-2);
                ps->add("Zbb",          "Zll + bb",                     eProcessType::BKG,      eProcess::ZllHF,        "Z+hf",                         kBlue);
                ps->add("Zbc",          "Zll + bc",                     eProcessType::BKG,      eProcess::ZllHF,        "Z+hf",                         kBlue);
                ps->add("Zbl",          "Zll + bl",                     eProcessType::BKG,      eProcess::ZllLF,        "Z+lf",                         kBlue-2);
                ps->add("Zcc",          "Zll + cc",                     eProcessType::BKG,      eProcess::ZllHF,        "Z+hf",                         kBlue);
                ps->add("Zcl",          "Zll + cl",                     eProcessType::BKG,      eProcess::ZllLF,        "Z+lf",                         kBlue-2);
                ps->add("Zl",           "Zll + l",                      eProcessType::BKG,      eProcess::ZllLF,        "Z+lf",                         kBlue-2);
                ps->add("ttbar",        "t#bar{t}",                     eProcessType::BKG,      eProcess::TTBAR,        "t#bar{t}",                     kWhite);
                ps->add("ttbarSFTF",    "t#bar{t} TF",                  eProcessType::BKG,      eProcess::TTBARFAKE,    "fake-#tau t#bar{t}",           kOrange-2);
                ps->add("ttbarSFFT",    "t#bar{t} FT",                  eProcessType::BKG,      eProcess::TTBARFAKE,    "fake-#tau t#bar{t}",           kOrange-2);
                ps->add("ttbarSFFF",    "t#bar{t} FF",                  eProcessType::BKG,      eProcess::TTBARFAKE,    "fake-#tau t#bar{t}",           kOrange-2);
                ps->add("stops",        "single top",                   eProcessType::BKG,      eProcess::STOP,         "single top",                   kOrange+2);
                ps->add("stopt",        "single top",                   eProcessType::BKG,      eProcess::STOP,         "single top",                   kOrange+2);
                ps->add("stopWt",       "single top",                   eProcessType::BKG,      eProcess::STOP,         "single top",                   kOrange+2);
                ps->add("WW",           "WW",                           eProcessType::BKG,      eProcess::DIBOSON,      "Diboson",                      kYellow+1);
                ps->add("WZ",           "WZ",                           eProcessType::BKG,      eProcess::DIBOSON,      "Diboson",                      kYellow+1);
                ps->add("ZZ",           "ZZ",                           eProcessType::BKG,      eProcess::DIBOSON,      "Diboson",                      kYellow+1);
                ps->add("Wtt",          "W+jets",                       eProcessType::BKG,      eProcess::WJETS,        "W+jets",                       kGreen);
                ps->add("W",            "W+jets",                       eProcessType::BKG,      eProcess::WJETS,        "W+jets",                       kGreen);
                ps->add("Fake",         "Multi-jet",                    eProcessType::BKG,      eProcess::FAKE,         "Multi-jet",                    kCyan);
                ps->add("ttH",          "SM Higgs",                     eProcessType::BKG,      eProcess::H,            "SM Higgs",                     kMagenta);
                ps->add("WHbb",         "SM Higgs",                     eProcessType::BKG,      eProcess::H,            "SM Higgs",                     kMagenta);
                ps->add("ZHbb",         "SM Higgs",                     eProcessType::BKG,      eProcess::H,            "SM Higgs",                     kMagenta);
                ps->add("WHtautau",     "SM Higgs",                     eProcessType::BKG,      eProcess::H,            "SM Higgs",                     kMagenta);
                ps->add("ZHtautau",     "SM Higgs",                     eProcessType::BKG,      eProcess::H,            "SM Higgs",                     kMagenta);
                ps->add("ggFHtautau",   "SM Higgs",                     eProcessType::BKG,      eProcess::H,            "SM Higgs",                     kMagenta);
                ps->add("VBFHtautau",   "SM Higgs",                     eProcessType::BKG,      eProcess::H,            "SM Higgs",                     kMagenta);
                //  ps->add("hhttbbKL1p0",  "HH (#kappa_{#lambda}=1.0)",    eProcessType::SIG,      eProcess::HH,           "HH (#kappa_{#lambda}=1.0)",    kRed);
                //  ps->add("hhttbbKL5p0",  "HH (#kappa_{#lambda}=5.0)",    eProcessType::SIG,      eProcess::HH,           "HH (#kappa_{#lambda}=5.0)",    kRed);
                //ps->add("hhttbb", "HH -> ttbb", eProcessType::SIG, eProcess::HH, "HH -> ttbb", kRed);
                ps->add("hhttbb", "HH -> #tau#taubb", eProcessType::SIG, eProcess::HH, "HH -> #tau#taubb", kRed);


                Config* c = new Config(b, ps, rs, vs);
                // c->load("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-klambda-v1.root", "Preselection");
                c->load("/home/zifeng/HistCxAOD/hist-OUTPUT_all.root", "Preselection");
                c->updateHistogramPtr(rs->content()->front(), v);
                DrawStackTool* ds = new DrawStackTool(info);
                ds->output_path = "/home/zifeng/HistCxAOD/plots";
                if (ds->check(c))
                {
                        ds->manipulate(c);
                        ds->paint(c);
                        ds->makeYield(c);
                        ds->run(c);
                }
                else 
                {
                        clog << "Can not draw " << c->current_region->name << " " << c->current_variable->name << '\n';
                }

                delete ps;
                delete ds;
                delete c;
        }

        delete b;
        delete rs;
        delete vs;
        delete info;

}
