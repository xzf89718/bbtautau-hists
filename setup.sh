whoami

echo ${PWD}

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
lsetup "root 6.20.06-x86_64-centos7-gcc8-opt" cmake git --quiet

cd ../build-bbtautau-hists
