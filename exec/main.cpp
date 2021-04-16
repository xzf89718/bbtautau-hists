#include "ExamplesInclude.h"
#include "TROOT.h"

#include <iostream>

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    gROOT->SetBatch(1);
    
    cout << "Hello!" << endl;

    // test_hadhad("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-klambda-v2.root");

    // test_hadhad_klambda("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-klambda-v2.root");

    // test_hadhad_yield("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-klambda-v2.root");

    // test_hadhad_ZtautauMG("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-Ztautau-v1.root");

    // test_hadhad_WtDS("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-top-v2.root");

    // test_hadhad_WtGen("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-top-v2.root");

    // test_hadhad_WtOTF("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-top-v2.root");

    // test_hadhad_NonRes("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-NonRes-v3.root");

    // test_lephad_ttbarSys("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-TTBar-v3.root");

    // test_lephad_ttbarRew("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-TTBar-v4.root");
    // test_hadhad_ttbarRew("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-ttbarReweight-HadHad-v1.root");

    // hadhadsr_v15("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-HadHad-v1.root");
    // hadhadsr_v15_yield("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-HadHadWSI-v1.all.root");
    // hadhadsr_v15_binning("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-HadHadWSI-v1.all.root");

    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/bowenzhang/WSMakerOutput/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_SM_MVA_0/workspaces/combined/0.root");
    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/bowenzhang/WSMakerOutput/Bowen_HadHadWSI_v1.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_SM_BDT_0/workspaces/combined/0.root");

    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/bowenzhang/WSMakerOutput/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_2HDM_MVA_300/workspaces/combined/300.root");
    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/bowenzhang/WSMakerOutput/Bowen_HadHadWSI_v1.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_2HDM_BDT_300/workspaces/combined/300.root");

    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/bowenzhang/WSMakerOutput/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_2HDM_MVA_500/workspaces/combined/500.root");
    // test_ws_info("/publicfs/atlas/atlasnew/higgs/hh2X/bowenzhang/WSMakerOutput/Bowen_HadHadWSI_v1.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_2HDM_BDT_500/workspaces/combined/500.root");

    // test_ranking("/publicfs/atlas/atlasnew/higgs/hh2X/bowenzhang/WSMakerOutput/Bowen_HadHadWSI_v2.ZgenTtbarNorm_HH_13TeV_ZgenTtbarNorm_Systs_hadhad_SM_MVA_0/workspaces/combined/0.root", 
    //              "/tmp/bowenzhang/Ranking_SM_oneAsim.txt");
    // test_ranking_plot("/tmp/bowenzhang/Ranking_SM_oneAsim.txt", "/tmp/bowenzhang/Ranking_SM_oneAsim.png");

    // test_ranking("/scratchfs/atlas/bowenzhang/ResolvedStatAna/WSMaker_Group/output/Chris_HadHadWSI_v3.1_HH_13TeV_1_Systs_hadhad_SM_MVA_0/workspaces/combined/0.root", 
    //              "/scratchfs/atlas/bowenzhang/bbtautau-hists/output/Ranking_SM_oneAsim.txt");
    // test_ranking_plot("/scratchfs/atlas/bowenzhang/bbtautau-hists/output/Ranking_SM_oneAsim.txt", "/scratchfs/atlas/bowenzhang/bbtautau-hists/output/Ranking_SM_oneAsim.png");

    // test_pulls("/scratchfs/atlas/bowenzhang/ResolvedStatAna/WSMaker_Group/output/Chris_HadHadWSI_v3.1_HH_13TeV_1_Systs_hadhad_SM_MVA_0/workspaces/combined/0.root", 
    //            "/scratchfs/atlas/bowenzhang/bbtautau-hists/output/Pulls_SM.txt");
    // test_pulls_plot("/scratchfs/atlas/bowenzhang/bbtautau-hists/output/Pulls_SM.txt", "/scratchfs/atlas/bowenzhang/bbtautau-hists/output/Pulls_SM.png");

    cout << "Ciao!" << endl;

    return 0;
}
