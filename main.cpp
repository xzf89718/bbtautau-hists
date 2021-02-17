#include "Example.h"
#include "Example_klambda.h"
#include "Example_yield.h"
#include "Example_ZtautauMG.h"
#include "Example_NonRes.h"
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

    // test_hadhad_ZtautauMG("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-Ztautau.root");

    test_hadhad_NonRes("/scratchfs/atlas/zhangbw/CxAODReaderSemiBoosted/run/hist-NonRes-v0.root");

    return 0;
}
