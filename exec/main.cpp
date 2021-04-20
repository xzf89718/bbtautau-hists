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

    test_hadhad_WtDS("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-top-v3.root");
    // test_hadhad_WtGen("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-top-v3.root");
    // test_hadhad_WtOTF("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-top-v3.root");

    // test_hadhad_NonRes("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-NonRes-v3.root");

    // test_lephad_ttbarSys("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-TTBar-v3.root");

    // test_lephad_ttbarRew("/scratchfs/atlas/bowenzhang/CxAODReaderForSystStudy/run/hist-TTBar-v4.root");
    // test_hadhad_ttbarRew("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-ttbarReweight-HadHad-v1.root");

    // hadhadsr_v15("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-HadHad-v1.root");
    // hadhadsr_v15_yield("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-HadHadWSI-v1.all.root");
    // hadhadsr_v15_binning("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-HadHadWSI-v1.all.root");

    // test_ttbarKine("/scratchfs/atlas/bowenzhang/CxAODReaderSemiBoosted/run/hist-ttbar-kinematics.root");

    cout << "Ciao!" << endl;

    return 0;
}
