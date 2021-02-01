#include "Example.h"
#include "Example_klambda.h"
#include "TROOT.h"

#include <iostream>

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    gROOT->SetBatch(1);
    
    cout << "Hello!" << endl;

    // test_hadhad();

    test_hadhad_klambda();

    return 0;
}
