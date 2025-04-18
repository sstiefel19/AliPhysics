AliAnalysisTask *AddTask_ConversionAODProduction( Int_t dataset                 = 0,
                                                  Bool_t isMC                   = kFALSE,
                                                  TString periodNameV0Reader    = "",
                                                  Bool_t addv0sInESDFilter      = kTRUE,
                                                  Bool_t onlyOnTheFly           = kFalse
                                                ){

    // Before doing anything, we load the needed library
    gSystem->Load("libPWGGAGammaConv");
    // dataset 0: pp
    // dataset 1: PbPb
    // dataset 2: pPb

    //get the current analysis manager
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr) {
      Error("AddTask_V0ReaderV1", "No analysis manager found.");
      return 0;
    }

    //identification of low B field runs
    Bool_t lowBfield = kFALSE;

    //get CDB entry
    AliCDBEntry* entry = AliCDBManager::Instance()->Get("GRP/GRP/Data");
    if(!entry){
      Info("AddTask_ConversionAODProduction", "cannot get AliCDBEntry for GRP/GRP/Data");
    }else{
      const AliGRPObject* grpData = dynamic_cast<AliGRPObject*>(entry->GetObject());
      if(!grpData){
        Info("AddTask_ConversionAODProduction", "cannot get AliCDBEntry for GRP/GRP/Data");
      }else{
        Float_t fL3current = grpData->GetL3Current((AliGRPObject::Stats)0);
        if(TMath::Abs(fL3current) < 15000) lowBfield = kTRUE;
      }
    }

//========= Add PID Reponse to ANALYSIS manager ====
    if(!(AliPIDResponse*)mgr->GetTask("PIDResponseTask")){
      gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
      AddTaskPIDResponse(isMC);
    }

    TString analysiscut;
    TString analysiscutEvent;
    TString analysiscutB;

    if(dataset == 1){ //PbPb
      analysiscutEvent = "10000003";
      if(lowBfield){
          analysiscut  = "06000088d00000001100000000";
          analysiscutB = "16000088d00000001100000000";
      } else {
          analysiscut  = "06000008d00000001100000000";
          analysiscutB = "16000008d00000001100000000";
      }
    } else if (dataset == 2){ // pPb
      analysiscutEvent = "80000003";
      if(lowBfield){
          analysiscut  = "06000088d00000001100000000";
          analysiscutB = "16000088d00000001100000000";
      } else {
          analysiscut  = "06000008d00000001100000000";
          analysiscutB = "16000008d00000001100000000";
      }
    } else{ // pp
      analysiscutEvent = "00000003";
      if(lowBfield){
          analysiscut  = "06000088d00100001100000000";
          analysiscutB = "16000088d00100001100000000";
      } else {
          analysiscut  = "06000008d00100001100000000";
          analysiscutB = "16000008d00100001100000000";
      }
    }

    //========= Add V0 Reader to  ANALYSIS manager =====

    AliV0ReaderV1 *fV0Reader=new AliV0ReaderV1("ConvGammaAODProduction");
    if (periodNameV0Reader.CompareTo("") != 0) fV0Reader->SetPeriodName(periodNameV0Reader);
    fV0Reader->SetCreateAODs(kTRUE);
    fV0Reader->SetUseOwnXYZCalculation(kTRUE);
    fV0Reader->SetUseAODConversionPhoton(kTRUE);
    if (addv0sInESDFilter){fV0Reader->SetAddv0sInESDFilter(kTRUE);}
    // fV0Reader->CheckAODConsistency();

    if(!onlyOnTheFly){
      AliV0ReaderV1 *fV0ReaderB=new AliV0ReaderV1("ConvGammaAODProductionB");
      if (periodNameV0Reader.CompareTo("") != 0) fV0ReaderB->SetPeriodName(periodNameV0Reader);
      fV0ReaderB->SetCreateAODs(kTRUE);
      fV0ReaderB->SetUseOwnXYZCalculation(kTRUE);
      fV0ReaderB->SetUseAODConversionPhoton(kTRUE);
      if (addv0sInESDFilter){fV0ReaderB->SetAddv0sInESDFilter(kTRUE);}
      // fV0ReaderB->CheckAODConsistency();
    }

    AliConvEventCuts *fEventCutsA=NULL;
    AliConvEventCuts *fEventCutsB=NULL;
    if(analysiscutEvent!=""){
      fEventCutsA = new AliConvEventCuts(analysiscutEvent.Data(), analysiscutEvent.Data());
      fEventCutsA->SetPreSelectionCutFlag(kTRUE);
      fEventCutsA->SetV0ReaderName("ConvGammaAODProduction");
      if(fEventCutsA->InitializeCutsFromCutString(analysiscutEvent.Data())){
          fV0Reader->SetEventCuts(fEventCutsA);
      }
      if(!onlyOnTheFly){
        fEventCutsB = new AliConvEventCuts(analysiscutEvent.Data(), analysiscutEvent.Data());
        fEventCutsB->SetPreSelectionCutFlag(kTRUE);
        fEventCutsB->SetV0ReaderName("ConvGammaAODProductionB");
        if(fEventCutsB->InitializeCutsFromCutString(analysiscutEvent.Data())){
            fV0ReaderB->SetEventCuts(fEventCutsB);
        }
      }
    }

    // Set AnalysisCut Number
    AliConversionPhotonCuts *fCuts = new AliConversionPhotonCuts(analysiscut.Data(), analysiscut.Data());
    if(fCuts->InitializeCutsFromCutString(analysiscut.Data())){
      fCuts->SetIsHeavyIon(dataset);
      fV0Reader->SetConversionCuts(fCuts);
    }

    fV0Reader->Init();
    mgr->AddTask(fV0Reader);

     if(!onlyOnTheFly){
      AliConversionPhotonCuts *fCutsB = new AliConversionPhotonCuts(analysiscutB.Data(), analysiscutB.Data());
      if(fCutsB->InitializeCutsFromCutString(analysiscutB.Data())){
        fCutsB->SetIsHeavyIon(dataset);
        fV0ReaderB->SetConversionCuts(fCutsB);
      }
      fV0ReaderB->Init();
      mgr->AddTask(fV0ReaderB);

    AliLog::SetGlobalLogLevel(AliLog::kInfo);

    //================================================
    //              data containers
    //================================================
    //            find input container
    //below the trunk version
    AliAnalysisDataContainer *cinput  = mgr->GetCommonInputContainer();
    AliAnalysisDataContainer *coutputPCMv0sA = mgr->CreateContainer("PCM offlineV0Finder container", TBits::Class(), AliAnalysisManager::kExchangeContainer);

    // connect output
    mgr->ConnectOutput(fV0Reader,1,coutputPCMv0sA);

    // connect input V0Reader
    mgr->ConnectInput(fV0Reader,0,cinput);

    // Do the same but for the offline V0s
    if(!onlyOnTheFly){
      AliAnalysisDataContainer *coutputPCMv0sB = mgr->CreateContainer("PCM onflyV0Finder container", TBits::Class(), AliAnalysisManager::kExchangeContainer);
      mgr->ConnectOutput(fV0ReaderB,1,coutputPCMv0sB);
      mgr->ConnectInput(fV0ReaderB,0,cinput);
    }

    return fV0Reader;
}
