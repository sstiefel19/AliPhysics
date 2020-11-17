#if !defined(__CINT__) || defined(__CLING__)
#include "AliMCEventHandler.h"
#include "AliESDInputHandler.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisAlien.h"
#include "AliAnalysisManager.h"

// #include "AliTaskCDBconnect.h"
// #include "AliPhysicsSelectionTask.h"
// #include "AliAnalysisTaskPIDResponse.h"
// #include "AliMultSelectionTask.h"
// #include "AliCentralitySelectionTask.h"
// #include "AliEmcalCorrectionTask.h"

#include "TString.h"


// R__ADD_INCLUDE_PATH($ALICE_PHYSICS)
R__ADD_INCLUDE_PATH($ALICE_PHYSICS)
// #include <PWG/EMCAL/macros/AddTaskAodSkim.C>
// #include <PWG/EMCAL/macros/AddTaskEsdSkim.C>
#include <PWGGA/GammaConv/macros/AddTask_V0Reader.C>
#include <PWGGA/GammaConv/macros/AddTask_ClusterQA.C>
#include <PWGGA/GammaConv/macros/AddTask_PhotonQA.C>
#include <PWGGA/GammaConv/macros/AddTask_GammaConvV1_pp.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaConvV1_pPb.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaConvV1_PbPb.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaCalo_pp.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaCalo_pPb.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaCalo_PbPb.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaConvCalo_pp.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaConvCalo_pPb.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaConvCalo_PbPb.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaCaloMerged_pp.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaCaloMerged_pPb.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaCaloMerged_PbPb.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_ConvMode_pp.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_CaloMode_pp.C>
// #include <PWGGA/GammaConv/macros/AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_MixedMode_pp.C>

#endif
#include "/Users/stephanstiefelmaier/repos/pcg/AnalysisSoftware/LocalTrainTests/localRunningChain.h"
#include <iostream>

using std::cout;
using std::endl;


//______________________________________________________________________________
void runLocalAnalysisROOT6_ilya(
    Int_t           intMCrunning                = 0,
    Int_t           collsys                     = 0, //0 pp, 1 pPb, 2 PbPb
    TString         runPeriod                   = "LHC15n",
    TString         runPeriodData               = "LHC15n",
    TString         dataType                    = "AOD",
    TString         runMode                     = "PQ2HC",//P:PCM, 2:PCM+Tree, Q:PhotonQA, H:hybrid PCMEMC, C: EMC
    Int_t           recoPassData                = 4,
    TString         tenderPassData              = "pass4",
    Bool_t          useCorrTask                 = kFALSE,
    TString         aodConversionCutnumber      = "10000003_06000008400000001000000000",
    Bool_t          isRun2                      = kFALSE,
    UInt_t          numLocalFiles               = 50,
    Int_t           chunk                       = -1,
    Int_t           trainConfig                 = 700)
{

    Bool_t tuneOnData = kFALSE;  // set this to false to get non fluctuating PID values in MC

//~ GammaConv_00000003_ 06000008400100001000000000 _gamma

    // constants
    // TString conversionPhotonCutnumber = "0d200009f9730000dge0404000";
    // Int_t trainConfig = 870;
    TString conversionPhotonCutnumber = "00000008400100001500000000";

    //~ Int_t trainConfig = 700;

    // since we will compile a class, tell root where to look for headers
    #if !defined (__CINT__) || defined (__CLING__)
        gInterpreter->ProcessLine(".include $ROOTSYS/include");
        gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
    #else
        gROOT->ProcessLine(".include $ROOTSYS/include");
        gROOT->ProcessLine(".include $ALICE_ROOT/include");
    #endif

    // Create analysis manager
    AliAnalysisManager* mgr                     = new AliAnalysisManager("LocalAnalysisTaskRunning");


    // change this objects to strings
    TString usedData(dataType);
    cout << dataType.Data() << " analysis chosen" << endl;
    // Check type of input and create handler for it
    TString localFiles("-1");
    localFiles = "testSample.txt";


    if(dataType.Contains("AOD")){
        AliAODInputHandler* aodH                = new AliAODInputHandler();
        mgr->SetInputEventHandler(aodH);
    } else if(dataType.Contains("ESD")){
        AliESDInputHandler* esdH                = new AliESDInputHandler();
        mgr->SetInputEventHandler(esdH);
    } else {
        cout << "Data type not recognized! You have to specify ESD, AOD, or sESD!\n";
    }

    cout << "Using " << localFiles.Data() << " as input file list.\n";

    // Create MC handler, if MC is demanded
    if (intMCrunning && (!dataType.Contains("AOD")))
    {
        AliMCEventHandler* mcH                  = new AliMCEventHandler();
        mcH->SetPreReadMode(AliMCEventHandler::kLmPreRead);
        mcH->SetReadTR(kTRUE);
        mgr->SetMCtruthEventHandler(mcH);
    }

    // -----------------------------------------
    //                CDB CONNECT
    // -----------------------------------------
    #if !defined (__CINT__) || defined (__CLING__)
        AliTaskCDBconnect *taskCDB=reinterpret_cast<AliTaskCDBconnect*>(
        //~ gInterpreter->ExecuteMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C()"));
         gInterpreter->ExecuteMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C(\"local://$ALIROOT_OCDB_ROOT/OCDB\")"));

        taskCDB->SetFallBackToRaw(kTRUE);
    #else
        gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/PilotTrain/AddTaskCDBconnect.C");
        AliTaskCDBconnect *taskCDB = AddTaskCDBconnect();
        taskCDB->SetFallBackToRaw(kTRUE);
    #endif

    // -----------------------------------------
    //            PHYSICS SELECTION
    // -----------------------------------------
    #if !defined (__CINT__) || defined (__CLING__)
        AliPhysicsSelectionTask *physSelTask=reinterpret_cast<AliPhysicsSelectionTask*>(
        gInterpreter->ExecuteMacro(Form("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C(%i)",intMCrunning ? 1 : 0)));
    #else
        gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
        AliPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(intMCrunning);
    #endif

    // -----------------------------------------
    //               PID RESPONSE
    // -----------------------------------------
    #if !defined (__CINT__) || defined (__CLING__)
        AliAnalysisTaskPIDResponse *pidRespTask=reinterpret_cast<AliAnalysisTaskPIDResponse*>(
        gInterpreter->ExecuteMacro(Form("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C(%i, %i, %i, \"%s\")",intMCrunning,kFALSE,tuneOnData,tenderPassData.Data())));
    #else
        gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
        AddTaskPIDResponse(intMCrunning,kFALSE,kTRUE,tenderPassData);
    #endif

    // -----------------------------------------
    //               MULT SELECTION
    // -----------------------------------------
    #if !defined (__CINT__) || defined (__CLING__)
        if(isRun2){
        cout << "SFS here\n";
            AliMultSelectionTask *multSelTask=reinterpret_cast<AliMultSelectionTask*>(
            gInterpreter->ExecuteMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C()"));
        } else {
                cout << "SFS here1\n";

            AliCentralitySelectionTask *multSelTask=reinterpret_cast<AliCentralitySelectionTask*>(
            gInterpreter->ExecuteMacro("$ALICE_PHYSICS/OADB/macros/AddTaskCentrality.C(kFALSE,kTRUE)"));
        }
    #else
        if(isRun2){
                cout << "SFS here2\n";

            gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
            AddTaskMultSelection();
        } else {
                cout << "SFS here3\n";

            gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskCentrality.C");
            AddTaskCentrality(kFALSE,kTRUE);
        }
    #endif

    // -----------------------------------------
    //   EMCAL TENDER or CORRECTION FRAMEWORK
    // -----------------------------------------
    TString taskNameSpecial                     = "";
    const UInt_t  kPhysSel                      = AliVEvent::kAny;
    /*(
    if(useCorrTask){
        AliEmcalCorrectionTask * correctionTask = AliEmcalCorrectionTask::AddTaskEmcalCorrectionTask("");
        //~ correctionTask->SelectCollisionCandidates(kPhysSel);
        if(intMCrunning==0){
           correctionTask->SelectCollisionCandidates(kPhysSel);
            // correctionTask->SetUserConfigurationFilename("/home/nschmidt/alice/ali-master/AliPhysics/PWGGA/GammaConv/config/PWGGA_CF_config.yaml");
            correctionTask->SetUserConfigurationFilename("$ALICE_PHYSICS/PWG/EMCAL/config/AliEmcalCorrectionConfiguration.yaml");
            correctionTask->Initialize();
            }
        else{
            // correctionTask->SetUserConfigurationFilename("/home/nschmidt/alice/ali-master/AliPhysics/PWGGA/GammaConv/config/PWGGA_CF_config_MC.yaml");
            correctionTask->SetUserConfigurationFilename("/Users/stephanstiefelmaier/work/data/userConfigurationppNewDefaultMC.yaml");
            correctionTask->Initialize(kTRUE);
            }
        //~ correctionTask->Initialize();
        //~ taskNameSpecial                 = "S500A100";
        //~ AliEmcalCorrectionTask * correctionTaskSpezial = AliEmcalCorrectionTask::AddTaskEmcalCorrectionTask(taskNameSpecial);
        //~ correctionTaskSpezial->SelectCollisionCandidates(AliVEvent::kAny);
        if(intMCrunning==0){
            taskNameSpecial                 = "S500A100";
            AliEmcalCorrectionTask * correctionTaskSpezial = AliEmcalCorrectionTask::AddTaskEmcalCorrectionTask(taskNameSpecial);
            correctionTaskSpezial->SelectCollisionCandidates(AliVEvent::kAny);

            // correctionTaskSpezial->SetUserConfigurationFilename("/home/nschmidt/alice/ali-master/AliPhysics/PWGGA/GammaConv/config/PWGGA_CF_config.yaml");
            correctionTaskSpezial->SetUserConfigurationFilename("$ALICE_PHYSICS/PWG/EMCAL/config/AliEmcalCorrectionConfiguration.yaml");
            }
        //~ else
            //~ // correctionTaskSpezial->SetUserConfigurationFilename("/home/nschmidt/alice/ali-master/AliPhysics/PWGGA/GammaConv/config/PWGGA_CF_config_MC.yaml");
            //~ correctionTaskSpezial->SetUserConfigurationFilename("~/alice_37/AliPhysics/PWGGA/GammaConv/config/PWGGA_CF_config_MC.yaml");
        //~ correctionTaskSpezial->Initialize();
    }else{
        #if !defined (__CINT__) || defined (__CLING__)
        cout << "SFS if !defined\n";
            if(intMCrunning>0){
                        cout << "SFS int\n";

                //~ PWG/EMCAL/macros/AddTaskEmcalCorrectionTask.C

                //~ AliAnalysisTask *tenderTask=reinterpret_cast<AliAnalysisTask*>(
                //~ gInterpreter->ExecuteMacro(Form("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEmcalCorrectionTask.C()",tenderPassData.Data())));

                AliAnalysisTask *tenderTask=reinterpret_cast<AliAnalysisTask*>(
                gInterpreter->ExecuteMacro(Form("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEMCALTender.C( kTRUE, kTRUE, kTRUE, kTRUE, kFALSE, kFALSE, kTRUE, kFALSE, kFALSE, kTRUE, AliEMCALRecoUtils::kNoCorrection, kTRUE, 0.5, 0.1, AliEMCALRecParam::kClusterizerv2, kFALSE, kFALSE, -500e9, 500e9, 1e6, \"%s\", kFALSE, \"raw://\", 0, 0, 0, 1, \"\", \"\", \"\", kFALSE )",tenderPassData.Data())));
                }
            else{
                                    cout << "SFS else int\n";

                AliAnalysisTask *tenderTask=reinterpret_cast<AliAnalysisTask*>(
                    gInterpreter->ExecuteMacro(Form("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEMCALTender.C( kTRUE, kTRUE, kTRUE, kTRUE, kFALSE, kFALSE, kTRUE, kTRUE, kTRUE, kTRUE, AliEMCALRecoUtils::kNoCorrection, kTRUE, 0.5, 0.1, AliEMCALRecParam::kClusterizerv2, kFALSE, kFALSE, -500e9, 500e9, 1e6, \"%s\", kFALSE, \"raw://\", 0, 0, 0, 1, \"\", \"\", \"\", kTRUE )",tenderPassData.Data()))); // last argument loads temperature calib now new calib for Run1 and Run2
                    }
        #else
            cout << "SFS else not defined\n";
            gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEMCALTender.C");
            if(intMCrunning>0){
            cout << "SFS int\n";
                AliAnalysisTask *emcalTender            = AddTaskEMCALTender(  kTRUE, kTRUE, kTRUE, kTRUE, kFALSE, kFALSE, kTRUE, kFALSE, kFALSE, kTRUE, AliEMCALRecoUtils::kNoCorrection, kTRUE, 0.5, 0.1, AliEMCALRecParam::kClusterizerv2, kFALSE, kFALSE, -500e9, 500e9, 1e6,tenderPassData,kFALSE   );
                }
            else{
                        cout << "SFS else int\n";

                AliAnalysisTask *emcalTender            = AddTaskEMCALTender(  kTRUE, kTRUE, kTRUE, kTRUE, kFALSE, kFALSE, kTRUE, kTRUE, kTRUE, kTRUE, AliEMCALRecoUtils::kNoCorrection, kTRUE, 0.5, 0.1, AliEMCALRecParam::kClusterizerv2, kFALSE, kFALSE, -500e9, 500e9, 1e6,tenderPassData,kFALSE   );
                }
        #endif
    }

    */
    // -----------------------------------------
    //               PHOS TENDER
    // -----------------------------------------

    /*
    TString customPHOSBadMap = "$ALICE_PHYSICS/PWGGA/GammaConv/macros/data/PHOSBadChannelMapRunDepRun2_13TeV.root";
    Int_t intCustomPHOSBadMap = 1;
    #if !defined (__CINT__) || defined (__CLING__)
        AliAnalysisTask *phosTendTask=reinterpret_cast<AliAnalysisTask*>(
        gInterpreter->ExecuteMacro(Form("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_PHOSTender_PCMconfig.C( \"PHOSTenderTask\", \"PHOSTender\", \"%s\", %i, kFALSE, %i, \"%s\", \"Run1\",kTRUE )",runPeriod.Data(),recoPassData,intCustomPHOSBadMap,customPHOSBadMap.Data())));
    #else
        gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_PHOSTender_PCMconfig.C");
        AliAnalysisTask *phosTenderTask  = AddTask_PHOSTender_PCMconfig("PHOSTenderTask", "PHOSTender", runPeriod.Data(), recoPassData, kFALSE,0, "",kFALSE);
    #endif
*/

    // -----------------------------------------
    //               V0 READER
    // -----------------------------------------
    #if !defined (__CINT__) || defined (__CLING__)
        AddTask_V0Reader(runPeriod.Data(),kFALSE,kFALSE,kTRUE,collsys,aodConversionCutnumber.Data(), conversionPhotonCutnumber.Data());
    #else
        gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_V0Reader.C");
        AliAnalysisTask *taskV0Reader                = AddTask_V0Reader(runPeriod.Data(),kFALSE,kFALSE,kTRUE,collsys,aodConversionCutnumber.Data(), "00000008400100001500000000");
    #endif


    // -----------------------------------------
    //                SKIMMING
    // -----------------------------------------
    if(runMode.Contains("S")){
        #if !defined (__CINT__) || defined (__CLING__)
            // if(dataType.Contains("AOD")){
            //     AliAodSkimTask *AODskimtask = reinterpret_cast<AliAodSkimTask *>(
            //         gInterpreter->ExecuteMacro(Form("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskAodSkim.C ( -1, 0.9, NULL, AliVEvent::kINT7, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE,\"skimROOT6\" )")));
            //     AODskimtask->SetGammaBrName(Form("GammaConv_%s_gamma",aodConversionCutnumber.Data()));
            //     AODskimtask->SetCopyConv(kTRUE);
            //     AODskimtask->SetDoVertWoRefs(kFALSE);
            //     AODskimtask->SetCleanTracks(kTRUE);
            //     AODskimtask->SetRemoveTracks(kTRUE);
            // }
            // if(dataType.Contains("ESD"))
            //     AddTaskEsdSkim(kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, "Tracks", "AliSkimmedESD.root");
        #else
        #endif
    }

    // -----------------------------------------
    //                QA Wagons
    // -----------------------------------------
    // if(runMode.Contains("QA")){
    //     #if !defined (__CINT__) || defined (__CLING__)
    //         AddTask_PhotonQA("00000008400100001500000000","00010113","090000092663743800000000",intMCrunning,collsys);
    //         if(!dataType.Contains("AOD"))
    //             AddTask_ClusterQA("00000008400100001500000000","00010113", "1111100010022700000","3885500010022700000", "0163300000000000", 1, 1, intMCrunning, collsys, kTRUE, 1.0, "", kFALSE, kFALSE, runPeriod.Data());
    //     #else
    //     #endif
    // }


    // -----------------------------------------
    //               GammaConv PCM
    // -----------------------------------------
    if(runMode.Contains("P")){
        #if !defined (__CINT__) || defined (__CLING__)
            if(collsys==0){
                AddTask_GammaConvV1_pp(trainConfig, intMCrunning, conversionPhotonCutnumber.Data(), runPeriod.Data(), 2, 2, kTRUE, kFALSE, kFALSE, kFALSE, "3", 0, "", 0, "", kFALSE, runPeriodData.Data(), 0, kFALSE, kFALSE, kFALSE, 1, 0, 0);
            }
            if(collsys==1){
                // AddTask_GammaConvV1_PbPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),runPeriod.Data(),  2, 2, kFALSE, kFALSE, kFALSE, kFALSE,3, 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, kFALSE, "",kFALSE, "",0,kFALSE,0, kFALSE,kFALSE,kFALSE,"1042");
            }
            if(collsys==2){
                // AddTask_GammaConvV1_pPb(0, intMCrunning, "00000008400100001500000000",runPeriod.Data(),  2, 2, kFALSE, kFALSE, kFALSE, kFALSE,3, 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE,"",0,kFALSE,kFALSE,"300");
            }
        #else
            if(collsys==0){
                gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaConvV1_pp.C");
                AliAnalysisTask *taskPCMpp1 = AddTask_GammaConvV1_pp(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  2, 2, kFALSE, kFALSE, kFALSE, kFALSE,3, 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",0,kFALSE,kFALSE,kFALSE,1,0,0,"1020");
                AliAnalysisTask *taskPCMpp2 = AddTask_GammaConvV1_pp(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  2, 2, kFALSE, kFALSE, kFALSE, kFALSE,3, 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",0,kFALSE,kFALSE,kFALSE,1,0,0,"400");
            }
            if(collsys==1){
                gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaConvV1_PbPb.C");
                AliAnalysisTask *taskPCMPbPb = AddTask_GammaConvV1_PbPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),runPeriod.Data(),  2, 2, kFALSE, kFALSE, kFALSE, kFALSE,3, 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, kFALSE, "",kFALSE, "",0,kFALSE,0, kFALSE,kFALSE,kFALSE,"1042");
            }
            if(collsys==2){
                gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaConvV1_pPb.C");
                AliAnalysisTask *taskPCMpPb = AddTask_GammaConvV1_pPb(0, intMCrunning, "00000008400100001500000000",runPeriod.Data(),  2, 2, kFALSE, kFALSE, kFALSE, kFALSE,3, 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE,"",0,kFALSE,kFALSE,"300");
            }
        #endif
    }


    // -----------------------------------------
    //          GammaCalo EMC/DMC/PHOS
    // -----------------------------------------
    // if(runMode.Contains("C")){
    //     #if !defined (__CINT__) || defined (__CLING__)
    //         if(collsys==0){
    //             AddTask_GammaCalo_pp(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, kFALSE, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",kTRUE, !taskNameSpecial.CompareTo("") ? "106" : Form("106_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==1){
    //             AddTask_GammaCalo_PbPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),runPeriod.Data(),  1, 2, 5, kFALSE, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, 0, "",kFALSE, "",kTRUE,kFALSE,!taskNameSpecial.CompareTo("") ? "218" : Form("218_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==2){
    //             AddTask_GammaCalo_pPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 1, 0, kFALSE, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",kTRUE, !taskNameSpecial.CompareTo("") ? "200" : Form("200_CF%s",taskNameSpecial.Data()));
    //         }
    //     #else
    //         if(collsys==0){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaCalo_pp.C");
    //             AliAnalysisTask *taskPCMpp = AddTask_GammaCalo_pp(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, kFALSE, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",kTRUE,!taskNameSpecial.CompareTo("") ? "106" : Form("106_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==1){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaCalo_PbPb.C");
    //             AliAnalysisTask *taskPCMPbPb = AddTask_GammaCalo_PbPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),runPeriod.Data(),  1, 2, 5, kFALSE, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, 0, "",kFALSE, "",kTRUE,kFALSE,!taskNameSpecial.CompareTo("") ? "218" : Form("218_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==2){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaCalo_pPb.C");
    //             AliAnalysisTask *taskPCMpPb = AddTask_GammaCalo_pPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, kFALSE, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",kTRUE,!taskNameSpecial.CompareTo("") ? "200" : Form("200_CF%s",taskNameSpecial.Data()));
    //         }
    //     #endif
    // }


    // -----------------------------------------
    //      GammaConvCalo PCM-(EMC/DMC/PHOS)
    // -----------------------------------------
    // if(runMode.Contains("H")){
    //     #if !defined (__CINT__) || defined (__CLING__)
    //         if(collsys==0){
    //             AddTask_GammaConvCalo_pp(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, "",kFALSE, "",kTRUE,kFALSE,kFALSE,1.,0.,0.,kTRUE, !taskNameSpecial.CompareTo("") ? "106" : Form("106_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==1){
    //             AddTask_GammaConvCalo_PbPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, "",kFALSE, "",kFALSE,kTRUE, kFALSE, kTRUE, 0, kTRUE, !taskNameSpecial.CompareTo("") ? "300" : Form("300_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==2){
    //             AddTask_GammaConvCalo_pPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",kTRUE,kFALSE, !taskNameSpecial.CompareTo("") ? "508" : Form("508_CF%s",taskNameSpecial.Data()));
    //             AddTask_GammaConvCalo_pPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",kTRUE,kFALSE, !taskNameSpecial.CompareTo("") ? "700" : Form("700_CF%s",taskNameSpecial.Data()));
    //         }
    //     #else
    //         if(collsys==0){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaConvCalo_pp.C");
    //             AliAnalysisTask *taskPCMEMCpp = AddTask_GammaConvCalo_pp(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, "",kFALSE, "",kTRUE,kFALSE,kFALSE,1.,0.,0.,kTRUE, !taskNameSpecial.CompareTo("") ? "106" : Form("106_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==1){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaConvCalo_PbPb.C");
    //             AliAnalysisTask *taskPCMEMCPbPb = AddTask_GammaConvCalo_PbPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, "",kFALSE, "",kFALSE,kTRUE, kFALSE, kTRUE, 0, kTRUE,!taskNameSpecial.CompareTo("") ? "300" : Form("300_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==2){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaConvCalo_pPb.C");
    //             AliAnalysisTask *taskPCMEMCpPb1 = AddTask_GammaConvCalo_pPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",kTRUE,kFALSE,!taskNameSpecial.CompareTo("") ? "508" : Form("508_CF%s",taskNameSpecial.Data()));
    //             AliAnalysisTask *taskPCMEMCpPb2 = AddTask_GammaConvCalo_pPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, kFALSE,3., 0,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, "",kFALSE, "",kTRUE,kFALSE,!taskNameSpecial.CompareTo("") ? "700" : Form("700_CF%s",taskNameSpecial.Data()));
    //         }
    //     #endif
    // }


    // -----------------------------------------
    //              CaloMerged mEMC
    // -----------------------------------------
    // if(runMode.Contains("M")){
    //     #if !defined (__CINT__) || defined (__CLING__)
    //         if(collsys==0){
    //             AddTask_GammaCaloMerged_pp(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, 3.,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, 1,kTRUE, kFALSE,1.0,kFALSE,kFALSE, !taskNameSpecial.CompareTo("") ? "137" : Form("137_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==1){
    //             AddTask_GammaCaloMerged_PbPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, 3.,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, 0, 0,1,kTRUE, kFALSE,1.0,kFALSE,kFALSE, !taskNameSpecial.CompareTo("") ? "201" : Form("201_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==2){
    //             AddTask_GammaCaloMerged_pPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, 3.,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, 1,kTRUE, kFALSE,1.0,kFALSE,kFALSE, !taskNameSpecial.CompareTo("") ? "200" : Form("200_CF%s",taskNameSpecial.Data()));
    //         }
    //     #else
    //         if(collsys==0){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaCaloMerged_pp.C");
    //             AliAnalysisTask *taskPCMpp = AddTask_GammaCaloMerged_pp(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, 3.,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, 1,kTRUE, kFALSE,1.0,kFALSE,kFALSE,!taskNameSpecial.CompareTo("") ? "137" : Form("137_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==1){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaCaloMerged_PbPb.C");
    //             AliAnalysisTask *taskPCMPbPb = AddTask_GammaCaloMerged_PbPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, 3.,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", kFALSE, 0, 0,1,kTRUE, kFALSE,1.0,kFALSE,kFALSE,!taskNameSpecial.CompareTo("") ? "201" : Form("201_CF%s",taskNameSpecial.Data()));
    //         }
    //         if(collsys==2){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaCaloMerged_pPb.C");
    //             AliAnalysisTask *taskPCMpPb = AddTask_GammaCaloMerged_pPb(0, intMCrunning, "00000008400100001500000000", runPeriod.Data(),  1, 2, 5, 0, kFALSE, kFALSE, 3.,"FMUW:fileNameMultWeights;FEPC:fileNamedEdxPostCalib", 0, 1,kTRUE, kFALSE,1.0,kFALSE,kFALSE,!taskNameSpecial.CompareTo("") ? "200" : Form("200_CF%s",taskNameSpecial.Data()));
    //         }
    //     #endif
    // }

    // -----------------------------------------
    //              Heavy Meson task
    // -----------------------------------------
    // if(runMode.Contains("O")){
    //     #if !defined (__CINT__) || defined (__CLING__)
    //         if(collsys==0){
    //             AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_ConvMode_pp(0,intMCrunning,"00000008400100001500000000",1,1,"",kFALSE,"",-1,runPeriod.Data(),0,"110");
    //             AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_CaloMode_pp(0,intMCrunning,"00000008400100001500000000",1,1,"",kFALSE,"",-1,runPeriod.Data(),0,"110");
    //             AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_MixedMode_pp(0,intMCrunning,"00000008400100001500000000",1,1,"",kFALSE,"",-1,runPeriod.Data(),0,"110");
    //         }
    //         if(collsys==1){
    //         }
    //         if(collsys==2){
    //         }
    //     #else
    //         if(collsys==0){
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_ConvMode_pp.C");
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_CaloMode_pp.C");
    //             gROOT->LoadMacro("$ALICE_PHYSICS/PWGGA/GammaConv/macros/AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_MixedMode_pp.C");
    //             AliAnalysisTask *taskPCMpp =                 AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_ConvMode_pp(0,intMCrunning,"00000008400100001500000000",1,1,"",kFALSE,"",-1,runPeriod.Data(),0,"110");
    //             AliAnalysisTask *taskPCMpp =                 AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_CaloMode_pp(0,intMCrunning,"00000008400100001500000000",1,1,"",kFALSE,"",-1,runPeriod.Data(),0,"110");
    //             AliAnalysisTask *taskPCMpp =                 AddTask_GammaConvNeutralMesonPiPlPiMiNeutralMeson_MixedMode_pp(0,intMCrunning,"00000008400100001500000000",1,1,"",kFALSE,"",-1,runPeriod.Data(),0,"110");
    //         if(collsys==1){
    //             // TODO
    //         }
    //         if(collsys==2){
    //             // TODO
    //         }
    //     #endif
    // }


    mgr->SetUseProgressBar(1, 10);
    if (!mgr->InitAnalysis()) return;
    mgr->PrintStatus();

    // LOCAL CALCULATION
    TChain* chain = 0;
    if (usedData == "AOD") {
        chain = CreateAODChain(localFiles.Data(), numLocalFiles,0,kFALSE,"AliAODGammaConversion.root");
    } else {  // ESD
        chain = CreateESDChain(localFiles.Data(), numLocalFiles);
    }

    cout << endl << endl;
    cout << "****************************************" << endl;
    cout << "*                                      *" << endl;
    cout << "*            start analysis            *" << endl;
    cout << "*                                      *" << endl;
    cout << "****************************************" << endl;
    cout << endl << endl;

    // start analysis
    cout << "Starting LOCAL Analysis...";
    mgr->SetDebugLevel(5);
    mgr->StartAnalysis("local", chain);

    cout << endl << endl;
    cout << "****************************************" << endl;
    cout << "*                                      *" << endl;
    cout << "*             end analysis             *" << endl;
    cout << "*                                      *" << endl;
    cout << "****************************************" << endl;
    cout << endl << endl;
}

