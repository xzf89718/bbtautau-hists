# bbtautau-hists

[![Build Status](https://travis-ci.com/peppapiggyme/bbtautau-hists.svg?branch=master)](https://travis-ci.com/peppapiggyme/bbtautau-hists)

(not just) a plotting tool for bbtautau-ers

## dependency
c++17, ROOT, CMake

## setup
```bash
git clone git@github.com:peppapiggyme/bbtautau-hists.git
cd ../
mkdir build/
cd build/
cmake ../bbtautau-hists/
make
```

## Examples
- Stack plots: Example.cpp, HadHadSR_v15.cpp
- Yield tables: Example_yield.cpp, HadHadSR_v15_yield.cpp
- klambda comparison: Example_klambda.cpp
- Nonres signal comparison (Py vs Hw): Example_NonRes.cpp
- Ztautau MadGraph uncertainty: Example_ZtautauMG.cpp
- Single top (Wt) uncertainty: Example_WtDS.cpp, Example_WtGen.cpp, Example_WtOTF.cpp
- LepHad ttbar uncertainties: Example_ttbarSyst.cpp
- Lephad/Hadhad ttbar reweighting comparison: Example_ttbarRew.cpp
- Auto-binning for MVA score distribution: HadHadSR_v15_binning.cpp
- Example use Workspace: Example_WS.cpp
- Produce pulls&constrains of NPs: Example_Pulls.cpp
- Produce ranking of NPs (pre-fit impact on mu): Example_Ranking.cpp

> If not specify, then for hadhad

## run an example
```bash
# in `build/`, do
./run-main
```

## run a test
```bash
# in `build/`, do
./run-test
```

## run pulls
```bash
# in `build/`, do
./run-pulls <path/to/workspace> <path/to/output/tag>
```

## run ranking
```bash
# in `build/`, do
./run-ranking <path/to/workspace> <path/to/output/tag>
```

## more info
see issues
