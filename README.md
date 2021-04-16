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
- Stack plots: Example.h, HadHadSR_v15.h
- Yield tables: Example_yield.h, HadHadSR_v15_yield.h
- klambda comparison: Example_klambda.h
- Nonres signal comparison (Py vs Hw): Example_NonRes.h
- Ztautau MadGraph uncertainty: Example_ZtautauMG.h
- Single top (Wt) uncertainty: Example_WtDS.h, Example_WtGen.h, Example_WtOTF.h
- LepHad ttbar uncertainties: Example_ttbarSyst.h
- Lephad/Hadhad ttbar reweighting comparison: Example_ttbarRew.h
- Example use Workspace: Example_ws.h
- Produce pulls&constrains of NPs: Example_Pulls.h
- Produce ranking of NPs (pre-fit impact on mu): Example_Ranking.h

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
