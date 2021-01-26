#include "Config.h"
#include "Utils.h"
#include "HistTool.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>

using std::cout; 
using std::endl;

void test_hadhad()
{
    BasicInfo* b = new BasicInfo("13 TeV", "L=139fb^{-1}");

    Processes* ps = new Processes();
    ps->add("data", "data", eProcessType::DATA, eProcess::DATA, "data", kBlack);
    ps->add("test", "data", eProcessType::DATA, eProcess::DATA, "data", kBlack);
    ps->add("ttbar", "t#bar{t}", eProcessType::BKG, eProcess::TTBAR, "t#bar{t}", kOrange);
    ps->add("ttbar", "t#bar{t}", eProcessType::BKG, eProcess::TTBAR, "t#bar{t}", kOrange);
    ps->add("Fake", "Multi-jet", eProcessType::BKG, eProcess::FAKE, "Multi-jet", kPink);
    ps->add("hhttbb", "SM HH", eProcessType::SIG, eProcess::HH, "SM HH", kRed);

    Regions* rs = new Regions();
    rs->add("2tag2pjet_0ptv_LL_OS", "SR", eRegionType::SR);

    Variables* vs = new Variables();
    vs->add("mBB", "m_{BB} [GeV]", 10);

    Config* c = new Config(b, ps, rs, vs);

    c->load("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-semiboost.root", "Preselection");
    c->updateHistogramPtr(rs->content()->front(), vs->content()->front());

    HistTool* ht = new HistTool();
    ht->check(c);
    ht->merge(c);
    ht->makeYield(c, "/tmp/zhangb.txt");


    delete b;
    delete ps;
    delete rs;
    delete vs;
    delete c;
    delete ht;

}