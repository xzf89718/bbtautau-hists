#include "ExamplesInclude.h"
#include "TROOT.h"

#include <iostream>

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    gROOT->SetBatch(1);
    
    cout << "Hello!" << endl;

    // test_hadhad("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-klambda-v2.root");

    // test_hadhad_klambda("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-klambda-v2.root");

    // test_hadhad_yield("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-klambda-v2.root");

    // test_hadhad_ZtautauMG("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-Ztautau-v1.root");

    // test_hadhad_WtDS("/scratchfs/atlas/zhangbw/CxAODReaderForSystStudy/run/hist-top-v2.root");

    // test_hadhad_WtGen("/scratchfs/atlas/zhangbw/CxAODReaderForSystStudy/run/hist-top-v2.root");

    // test_hadhad_WtOTF("/scratchfs/atlas/zhangbw/CxAODReaderForSystStudy/run/hist-top-v2.root");

    // test_hadhad_NonRes("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-NonRes-v0.root");

    // test_lephad_ttbarSys("/scratchfs/atlas/zhangbw/CxAODReaderForSystStudy/run/hist-TTBar-v3.root");

    // hadhadsr_v15("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-HadHad-v1.root");

    // hadhadsr_v15_yield("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-HadHadWSI-v1.all.root");

    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/zhangbw/WSMakerOutput/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_SM_MVA_0/workspaces/combined/0.root");
    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/zhangbw/WSMakerOutput/Bowen_HadHadWSI_v1.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_SM_BDT_0/workspaces/combined/0.root");

    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/zhangbw/WSMakerOutput/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_2HDM_MVA_300/workspaces/combined/300.root");
    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/zhangbw/WSMakerOutput/Bowen_HadHadWSI_v1.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_2HDM_BDT_300/workspaces/combined/300.root");

    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/zhangbw/WSMakerOutput/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_2HDM_MVA_500/workspaces/combined/500.root");
    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/zhangbw/WSMakerOutput/Bowen_HadHadWSI_v1.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_2HDM_BDT_500/workspaces/combined/500.root");

    // test_ranking("/publicfs/atlas/atlasnew/higgs/hh2X/zhangbw/WSMakerOutput/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_SM_MVA_0/workspaces/combined/0.root", 
    //              "/tmp/zhangbw/Ranking_SM_oneAsim.txt");
    // test_ranking_plot("/tmp/zhangbw/Ranking_SM_oneAsim.txt", "/tmp/zhangbw/Ranking_SM_oneAsim.png");

    // test_ranking("/scratchfs/atlas/zhangbw/ResolvedStatAna/WSMaker_Group/output/Chris_HadHadWSI_v3.1_HH_13TeV_1_Systs_hadhad_SM_MVA_0/workspaces/combined/0.root", 
    //              "/workfs2/atlas/zhangbw/bbtautau-hists/output/Ranking_SM_oneAsim.txt");
    test_ranking_plot("/workfs2/atlas/zhangbw/bbtautau-hists/output/Ranking_SM_oneAsim.txt", "/workfs2/atlas/zhangbw/bbtautau-hists/output/Ranking_SM_oneAsim.png");

    // test_pulls("/scratchfs/atlas/zhangbw/ResolvedStatAna/WSMaker_Group/output/Chris_HadHadWSI_v3.1_HH_13TeV_1_Systs_hadhad_SM_MVA_0/workspaces/combined/0.root", 
    //            "/workfs2/atlas/zhangbw/bbtautau-hists/output/Pulls_SM.txt");
    // test_pulls_plot("/workfs2/atlas/zhangbw/bbtautau-hists/output/Pulls_SM.txt", "/workfs2/atlas/zhangbw/bbtautau-hists/output/Pulls_SM.png");

    cout << "Ciao!" << endl;

    return 0;
}
