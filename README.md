# bbtautau-hists

[![Build Status](https://travis-ci.com/peppapiggyme/bbtautau-hists.svg?branch=master)](https://travis-ci.com/peppapiggyme/bbtautau-hists)

a plotting tool for bbtautau-ers

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

## run an example
```bash
# in `build/`, do
./bbtautau-hists
```

## run ranking
```bash
# in `bbtautau-hists/`, do
hep_sub jobs/submit_ranking.sh -g atlas -os CentOS7 -o path/to/stdout -e path/to/stderr -argu workspace.root path/to/output -mem 4000 -wt mid
```

## more info
see issues
