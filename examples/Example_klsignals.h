#include "Config.h"
#include "Utils.h"
#include "CompTool.h"
#include "HistNameTool.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>
#include <string>

using std::cout; 
using std::endl;
using std::clog;
using HistNameTool::transformFloat;
void hadhad_klsignals_examples(const std::string& filename="default")
{
    BasicInfo* b = new BasicInfo("#sqrt{s} = 13 TeV", "L = 139 fb^{-1}");

    Regions* rs = new Regions();
    rs->add("2tag2pjet_0ptv_LL_OS",     "2 b-tag, 2 loose #tau, OS",        eRegionType::SR);

    Variables* vs = new Variables();
    vs->add("mBB",                  "m_{BB} [GeV]",                                      10);
    vs->add("mMMC",                 "m_{#tau#tau} (MMC) [GeV]",                          12);
    vs->add("mHH",                  "m_{HH} [GeV]",                                       1);
    vs->add("mHHScaled",            "m_{HH} (Scaled) [GeV]",                             12);
    vs->add("dRBB",                 "#Delta R(B,B)",                                     4 );
    vs->add("pTBB",                 "p_{T}^{b,b}",                                       10);
    vs->add("dRTauTau",             "#Delta R(#tau_{had},#tau_{had})",                   4 );
    vs->add("pTTauTau",             "p_{T}^{#tau,#tau}",                                 10);
    vs->add("dPhiBBTauTau",         "#Delta #phi{bb,#tau#tau}",                          4 );
    vs->add("dRBBTauTau",           "#Delta R{bb,#tau#tau}",                             5 );
    vs->add("pTBalance",            "p_{T}^{b,b} / p_{T}^{#tau,#tau}",                   5 );
    vs->add("MET",                  "E_{T}^{miss} [GeV]",                                5 );
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
    vs->add("Tau1Pt",               "Sub-leading #tau_{had} p_{T} [GeV]",                10);
    vs->add("Tau0Eta",              "Leading #tau_{had} #eta",                           2 );
    vs->add("Tau1Eta",              "Sub-leading #tau_{had} #eta",                       2 );
    vs->add("Tau0Ntrk",             "Leading #tau_{had} N_{tracks}",                     1 );
    vs->add("Tau1Ntrk",             "Sub-leading #tau_{had} N_{tracks}",                 1 );
    vs->add("dPhiTauTau",           "#Delta #phi (#tau_{had},#tau_{had})",               4 );

    CompInfo* info = new CompInfo();
    info->ratio_high = 1.38;
    info->ratio_low = 0.62;
    for(VariableInfo* v: *(vs->content()))
    {   
        {
            std::string base_name = "hhttbbKL";
            std::string from_reco = "fromReco";
            {
                Processes* ps = new Processes();
                ps->assign_norm_factors = false;
                int i_color = 0;
                for (auto &i : {0.0, 2.0, 5.0})
                {
                    ps->add(base_name + transformFloat(i) +from_reco, 
                            "HH Klambda = " + transformFloat(i) + " combine at reco level", 
                            eProcessType::SIG, eProcess::HHKLXFROMRECO,
                            "HH Klambda = " + transformFloat(i) + " combine at reco level", 
                            kRed + i_color
                           );
                    i_color = i_color + 1;
                }
                Config* c = new Config(b, ps, rs, vs);
                c->load(filename, "Preselection");
                info->parameter = "0_2_5";
                c->updateHistogramPtr(rs->content()->front(), v);
                CompTool* ct = new CompTool(info);
                ct->output_path = "/home/zifeng/HistCxAOD/KLReweight/test_signals";
                if (ct->check(c))
                {
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
        }

        {
            std::string base_name = "hhttbbKL";
            std::string from_reco = "fromReco";
            {
                Processes* ps = new Processes();
                ps->assign_norm_factors = false;
                int i_color = 0;
                for (auto &i : {-5.0, 1.0, 10.0})
                {
                    ps->add(base_name + transformFloat(i) +from_reco, 
                            "HH Klambda = " + transformFloat(i) + " combine at reco level", 
                            eProcessType::SIG, eProcess::HHKLXFROMRECO,
                            "HH Klambda = " + transformFloat(i) + " combine at reco level", 
                            kRed + i_color
                           );
                    i_color = i_color + 1;
                }
                Config* c = new Config(b, ps, rs, vs);
                c->load(filename, "Preselection");
                info->parameter = "n5_1_10";
                c->updateHistogramPtr(rs->content()->front(), v);
                CompTool* ct = new CompTool(info);
                ct->output_path = "/home/zifeng/HistCxAOD/KLReweight/test_signals";
                if (ct->check(c))
                {
                    ct->paint(c);
                    ct->run_noratio(c);
                }
                else 
                {
                    clog << "Can not draw " << c->current_region->name << " " << c->current_variable->name << '\n';
                }

                delete ps;
                delete ct;
                delete c;
            }
        }
    }


    
    delete b;
    delete rs;
    delete vs;
    delete info;

}
