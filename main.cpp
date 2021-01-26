#include "Config.h"
#include "Utils.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello!" << endl;

    BasicInfo* b = new BasicInfo("13 TeV", "L=139fb^{-1}");

    Processes* ps = new Processes();
    ps->add("data", "data", eProcessType::DATA, eProcess::DATA, kBlack);
    ps->add("ttbar", "t#bar{t}", eProcessType::BKG, eProcess::TTBARTRUE, kOrange);
    ps->add("hhttbb", "SM HH", eProcessType::SIG, eProcess::HH, kRed);

    Regions* rs = new Regions();
    rs->add("2tag2pjet_0ptv_LL_OS", "SR", eRegionType::SR);

    Variables* vs = new Variables();
    vs->add("mBB", "m_{BB} [GeV]", 10);

    Config* c = new Config(b, ps, rs, vs);

    TFile* f = TFile::Open("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-semiboost.root");
    f->ls();

    TDirectory* d = (TDirectory*)f->Get("Preselection");

    TH1* h = (TH1*)d->Get(Utils::histString(ps->content().front(), rs->content().front(), vs->content().front()).c_str());
    cout << h->Integral();

    f->Close();

    return 0;
}
