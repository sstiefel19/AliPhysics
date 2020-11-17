#!/bin/bash
#local running with this code requires testfiles downloaded and a testSampleESD.txt or testSampleAOD.txt text file with the input files stored in, for example pPb_5TeV/LHC16q/testSampleESD.txt

#energy="pPb_5TeV"
energy="pp_13TeV"

# intMCrunning=0 #0: data, 1: MC, 2: JJ MC
# runPeriod="LHC17h"
# runPeriodData="LHC17h"
# useCorrTask="kTRUE"
# passNumber="pass1"
# runNumber="273103"

#~ intMCrunning=0 #0: data, 1: MC, 2: JJ MC
#~ data_sim="data"
#~ fileNo="001"
#~ pass="pass1"
#~ runPeriod="LHC18f"
#~ runPeriodData="LHC18f"
#~ useCorrTask="kTRUE"
#~ passNumber="pass1"
#~ runNumber="287658"
#~ trainconfig="700"

# my stuff
#~ intMCrunning=1 #0: data, 1: MC, 2: JJ MC
#~ runPeriod="LHC18g4"
#~ runPeriodData="LHC18g"
#~ useCorrTask="kFALSE"
#~ passNumber="pass1"
#~ runNumber="285396"


#~ intMCrunning=2 #0: data, 1: MC, 2: JJ MC
#~ data_sim="sim"
#~ fileNo="002"
#~ pass=""
#~ runPeriod="LHC20b2b"
#~ runPeriodData="LHC17"
#~ useCorrTask="kTRUE" # what is this? check!
#~ ##~ useCorrTask="kFALSE" # what is this? check!
#~ passNumber="pass1" # what this do for MC?
#~ runNumber="271886"
#~ trainconfig="700"

intMCrunning=2 #0: data, 1: MC, 2: JJ MC
data_sim="sim"
fileNo="001"
pass=""
runPeriod="LHC17f8h"
runPeriodData="LHC16dp"
useCorrTask="kTRUE" # what is this? check!
passNumber="pass1" # what this do for MC?
runNumber="252235"
trainconfig="700"


collsys=0 #0: pp, 1: PbPb, 2: pPb
dataType=AOD #ESD or AOD
runMode="P" #switch for which tasks to run: QA (photon and cluster QA), P (PCM), C (Calo [EMC, DMC, PHOS]), H (hybrid PCM-Calo), M (merged EMC), S (skimming ESD or AOD)
recoPassData=1
tenderPassData="pass1"
aodConversionCutnumber="00000003_06000008400100001000000000";
numLocalFiles=1
isRun2="kTRUE"

#~ testDirectory=/Users/stephanstiefelmaier/work/projects/project_refactorProcessPhotons/

# my stuff
mkdir -p ./$runPeriod/${runMode}_$dataType/$runNumber/$fileNo
cd ./$runPeriod/${runMode}_$dataType/$runNumber/$fileNo

#~ echo $AODs/data/PbPb_5TeV/$runPeriod/pass3/$dataType/$runNumber/$fileNo/AliAOD.root > testSample.txt
echo $AODs/$data_sim/$energy/$runPeriod/$runNumber/$pass/$dataType/$fileNo/AliAOD.root > testSample.txt
cat testSample.txt

aliroot -x -l -b -q '/Users/stephanstiefelmaier/repos/alice/AliPhysics/PWGGA/GammaConv/macros/runLocalAnalysisROOT6_ilya.C('$intMCrunning','$collsys', "'$runPeriod'", "'$runPeriodData'", "'$dataType'", "'$runMode'", '$recoPassData', "'$tenderPassData'", '$useCorrTask', "'$aodConversionCutnumber'", '$isRun2', '$numLocalFiles', '-1', '$trainconfig')'


# end my stuff

#~ if [ $dataType == AOD ]; then
    #~ for i in {0001..0005}
    #~ do
    #~ cd $testDirectory
    #~ echo "$testDirectory/$energy/$runPeriod/$passNumber/$dataType/$runNumber/$i/root_archive.zip" > $energy/$runPeriod/testSampleAOD.txt
    #~ mkdir -p $energy/$runPeriod/$runMode$dataType/$i
    #~ cd $energy/$runPeriod/$runMode$dataType/$i
    #~ aliroot -x -l -b -q $testDirectory'/runLocalAnalysisROOT6.C('$intMCrunning','$collsys', "'$runPeriod'", "'$runPeriodData'", "'$dataType'", "'$runMode'", '$recoPassData', "'$tenderPassData'", '$useCorrTask', "'$aodConversionCutnumber'", '$isRun2', '$numLocalFiles')'
    #~ done
#~ else
    #~ i_count=0
    #~ for foldername in $testDirectory/$energy/$runPeriod/$passNumber/$dataType/**/*; do
        #~ cd $testDirectory
        #~ i_count=$(($i_count+1))
        #~ if [ $i_count -le 10 ]; then
            #~ runPart=${foldername##*/}
            #~ mkdir -p $energy/$runPeriod/$runMode$dataType/$runPart
            #~ echo "${foldername}/root_archive.zip" > $energy/$runPeriod/testSampleESD.txt
            #~ cd $energy/$runPeriod/$runMode$dataType/$runPart
            #~ aliroot -x -l -b -q '/home/ilya/uni/analysis/AnalysisSoftware/LocalTrainTests/runLocalAnalysisROOT6.C('$intMCrunning','$collsys', "'$runPeriod'", "'$runPeriodData'", "'$dataType'", "'$runMode'", '$recoPassData', "'$tenderPassData'", '$useCorrTask', "'$aodConversionCutnumber'", '$isRun2', '$numLocalFiles')'
        #~ fi
    #~ done
#~ fi
