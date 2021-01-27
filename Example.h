#include "Config.h"
#include "Utils.h"
#include "DrawStackTool.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>

using std::cout; 
using std::endl;

void test_hadhad()
{
    BasicInfo* b = new BasicInfo("#sqrt{s} = 13 TeV", "L=139fb^{-1}");

    Processes* ps = new Processes();
    ps->add("data", "data", eProcessType::DATA, eProcess::DATA, "data", kBlack);
    ps->add("Zttbb", "Z#tau#tau + bb", eProcessType::BKG, eProcess::ZtautauHF, "Z#tau#tau+hf", kBlue);
    ps->add("Zttbc", "Z#tau#tau + bc", eProcessType::BKG, eProcess::ZtautauHF, "Z#tau#tau+hf", kBlue);
    ps->add("Zttbl", "Z#tau#tau + bl", eProcessType::BKG, eProcess::ZtautauLF, "Z#tau#tau+hf", kBlue-2);
    ps->add("Zttcc", "Z#tau#tau + cc", eProcessType::BKG, eProcess::ZtautauHF, "Z#tau#tau+hf", kBlue);
    ps->add("Zttcl", "Z#tau#tau + cl", eProcessType::BKG, eProcess::ZtautauLF, "Z#tau#tau+hf", kBlue-2);
    ps->add("Zttl",  "Z#tau#tau + l",  eProcessType::BKG, eProcess::ZtautauLF, "Z#tau#tau+hf", kBlue-2);
    ps->add("Zbb", "Zll + bb", eProcessType::BKG, eProcess::ZllJETS, "Zll+Jets", kBlue-4);
    ps->add("Zbc", "Zll + bc", eProcessType::BKG, eProcess::ZllJETS, "Zll+Jets", kBlue-4);
    ps->add("Zbl", "Zll + bl", eProcessType::BKG, eProcess::ZllJETS, "Zll+Jets", kBlue-4);
    ps->add("Zcc", "Zll + cc", eProcessType::BKG, eProcess::ZllJETS, "Zll+Jets", kBlue-4);
    ps->add("Zcl", "Zll + cl", eProcessType::BKG, eProcess::ZllJETS, "Zll+Jets", kBlue-4);
    ps->add("Zl" , "Zll + l",  eProcessType::BKG, eProcess::ZllJETS, "Zll+Jets", kBlue-4);
    ps->add("ttbar", "t#bar{t}", eProcessType::BKG, eProcess::TTBAR, "t#bar{t}", kOrange);
    ps->add("ttbarTF", "t#bar{t} TF", eProcessType::BKG, eProcess::TTBARFAKE, "t#bar{t}", kOrange-2);
    ps->add("ttbarFT", "t#bar{t} FT", eProcessType::BKG, eProcess::TTBARFAKE, "t#bar{t}", kOrange-2);
    ps->add("ttbarFF", "t#bar{t} FF", eProcessType::BKG, eProcess::TTBARFAKE, "t#bar{t}", kOrange-2);
    ps->add("stops",  "single top", eProcessType::BKG, eProcess::STOP, "single top", kOrange+2);
    ps->add("stopt",  "single top", eProcessType::BKG, eProcess::STOP, "single top", kOrange+2);
    ps->add("stopWt", "single top", eProcessType::BKG, eProcess::STOP, "single top", kOrange+2);
    ps->add("WW", "Diboson", eProcessType::BKG, eProcess::DIBOSON, "Diboson", kYellow+1);
    ps->add("WZ", "Diboson", eProcessType::BKG, eProcess::DIBOSON, "Diboson", kYellow+1);
    ps->add("ZZ", "Diboson", eProcessType::BKG, eProcess::DIBOSON, "Diboson", kYellow+1);
    ps->add("Wtt", "W+jets", eProcessType::BKG, eProcess::WJETS, "W+jets", kGreen);
    ps->add("W",   "W+jets", eProcessType::BKG, eProcess::WJETS, "W+jets", kGreen);
    ps->add("Fake", "Multi-jet", eProcessType::BKG, eProcess::FAKE, "Multi-jet", kPink);
    ps->add("ttH", "SM Higgs", eProcessType::BKG, eProcess::H, "SM Higgs", kMagenta);
    ps->add("WHbb", "SM Higgs", eProcessType::BKG, eProcess::H, "SM Higgs", kMagenta);
    ps->add("ZHbb", "SM Higgs", eProcessType::BKG, eProcess::H, "SM Higgs", kMagenta);
    ps->add("WHtautau", "SM Higgs", eProcessType::BKG, eProcess::H, "SM Higgs", kMagenta);
    ps->add("ZHtautau", "SM Higgs", eProcessType::BKG, eProcess::H, "SM Higgs", kMagenta);
    ps->add("ggFHtautau", "SM Higgs", eProcessType::BKG, eProcess::H, "SM Higgs", kMagenta);
    ps->add("VBFHtautau", "SM Higgs", eProcessType::BKG, eProcess::H, "SM Higgs", kMagenta);
    ps->add("hhttbb", "SM HH", eProcessType::SIG, eProcess::HH, "SM HH", kRed);

    Regions* rs = new Regions();
    rs->add("2tag2pjet_0ptv_LL_OS", "SR", eRegionType::SR);

    Variables* vs = new Variables();
    vs->add("mBB", "m_{BB} [GeV]", 10);

    Config* c = new Config(b, ps, rs, vs);

    c->load("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-semiboost.root", "Preselection");
    c->updateHistogramPtr(rs->content()->front(), vs->content()->front());

    DrawStackInfo* info = new DrawStackInfo();
    info->ratio_high = 1.82;
    info->ratio_low = 0.18;
    info->signal_scale = 1000;

    DrawStackTool* ds = new DrawStackTool(info);
    ds->check(c);
    ds->manipulate(c);
    ds->paint(c);
    ds->makeYield(c, "/tmp/zhangbw.txt");
    ds->output_path = "/tmp/zhangbw/";
    ds->run(c);

    delete b;
    delete ps;
    delete rs;
    delete vs;
    delete c;
    delete ds;
    delete info;

}