#include "Example.h"
#include "Example_zifeng.h"
#include "Example_KLReweight.h"

#include "TROOT.h"

#include <iostream>

using namespace std;

int main(/*int argc, char *argv[]*/)
{
    gROOT->SetBatch(1);
    
    cout << "Hello!" << endl;
    Plots_KLReweigt();
    //test_hadhad();
   // test_zifeng_hadhad();   
    return 0;
}
