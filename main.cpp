#include "Example.h"
#include "Example_klambda.h"
#include "Example_yield.h"
#include "TROOT.h"

#include <iostream>

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    gROOT->SetBatch(1);
    
    cout << "Hello!" << endl;

    // test_hadhad();

    test_hadhad_klambda();

    // test_hadhad_yield();

    return 0;
}
