void AddTask_PhotonQA(
  TString   photonCutNumberV0Reader       = "060000084001001500000000",
  TString   TaskEventCutnumber            = "00000003",
  TString   TaskPhotonCutnumber           = "090000092663743800000000",
  Bool_t    isMC                          = kFALSE,
  Int_t     IsHeavyIon                    = 0,
  Bool_t    kHistograms                   = kTRUE,
  Double_t  kTree                         = 1.0,  // 0. / 0 / kFALSE for no, 1. / 1 / kTRUE for yes,  x > 1.0 will use only 1/x of the event statistics for the tree
  Double_t  kTreeHighPt                   = 100., // effective only for kTree >1. up to this pt write a fraction 1/kTree to the tree, above write all photons to the tree
  TString   V0ReaderCutNumberAODBranch    = "0000000060084001001500000",
  Bool_t    runBasicQAWithStandardOutput  = kTRUE,
  Bool_t    doEtaShiftV0Reader            = kFALSE,
  Bool_t    enableV0findingEffi           = kFALSE,              // enables V0finding efficiency histograms
  TString   fileNameExternalInputs        = "",
  Bool_t    enableElecDeDxPostCalibration = kFALSE,
  TString   generatorName                 = "",     // needed for accepted-header selection in added-signal MC
  Int_t     acceptedAddedParticles        = 0       // LHC20g10/LHC24a1: 0 all, 1 pi0, 6 eta, 13 pi0+eta
  ){
 
  AliCutHandlerPCM cuts;
  TString fileNamedEdxPostCalib       = cuts.GetSpecialFileNameFromString (fileNameExternalInputs, "FEPC:");

  // ================== GetAnalysisManager ===============================
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    return ;
  }

  // ================== GetInputEventHandler =============================
  AliVEventHandler *inputHandler=mgr->GetInputEventHandler();

    //=========  Set Cutnumber for V0Reader ================================
  TString cutnumberPhoton     = photonCutNumberV0Reader.Data();
  TString cutnumberEvent      = "00000003";
  if(IsHeavyIon==1)
    cutnumberEvent = "10000003";
  else if(IsHeavyIon==2)
    cutnumberEvent = "80000003";

  //========= Check V0 Reader in  ANALYSIS manager  =====
  TString V0ReaderName        = Form("V0ReaderV1_%s_%s",cutnumberEvent.Data(),cutnumberPhoton.Data());
  AliV0ReaderV1 *fV0ReaderV1  =  NULL;
  if( !(AliV0ReaderV1*)mgr->GetTask(V0ReaderName.Data()) ){
    cout << "V0Reader: " << V0ReaderName.Data() << " not found!!"<< endl;
    return;
  } else {
    cout << "V0Reader: " << V0ReaderName.Data() << " found!!"<< endl;
  }

  AliConvEventCuts *analysisEventCuts = new AliConvEventCuts();
  analysisEventCuts->SetV0ReaderName(V0ReaderName);
  if (generatorName.CompareTo("") != 0)
    analysisEventCuts->SetPeriodEnum(generatorName);
  analysisEventCuts->InitializeCutsFromCutString(TaskEventCutnumber.Data());
  analysisEventCuts->SetFillCutHistograms("",kFALSE);

  if (isMC && analysisEventCuts->GetSignalRejection() == 2){
    TList *HeaderList = new TList();
    HeaderList->SetOwner(kTRUE);
    if (generatorName.BeginsWith("LHC20g10") || generatorName.BeginsWith("LHC24a1")){
      auto addHeader = [&HeaderList](Int_t selection){
        if (selection == 1) HeaderList->Add(new TObjString("Injector (pi0)"));
        else if (selection == 2) HeaderList->Add(new TObjString("Injector (pi0a)"));
        else if (selection == 3) HeaderList->Add(new TObjString("Injector (pi0b)"));
        else if (selection == 4) HeaderList->Add(new TObjString("Injector (pi0c)"));
        else if (selection == 5) HeaderList->Add(new TObjString("Injector (pi0d)"));
        else if (selection == 6) HeaderList->Add(new TObjString("Injector (eta)"));
        else if (selection == 7) HeaderList->Add(new TObjString("Injector (etaa)"));
        else if (selection == 8) HeaderList->Add(new TObjString("Hijing"));
        else if (selection == 9) HeaderList->Add(new TObjString("Pileup"));
      };
      auto addHeaders = [&addHeader](Int_t first, Int_t last){
        for (Int_t selection = first; selection <= last; ++selection) addHeader(selection);
      };

      if (acceptedAddedParticles == 0) addHeaders(1,9);
      else if (acceptedAddedParticles >= 1 && acceptedAddedParticles <= 9) addHeader(acceptedAddedParticles);
      else if (acceptedAddedParticles == 10) addHeaders(1,5);
      else if (acceptedAddedParticles == 11) addHeaders(2,5);
      else if (acceptedAddedParticles == 12) addHeaders(6,7);
      else if (acceptedAddedParticles == 13){
        addHeader(1);
        addHeader(6);
      } else {
        cout << "ERROR invalid acceptedAddedParticles=" << acceptedAddedParticles
             << " for " << generatorName.Data() << endl;
        return;
      }
    } else {
      cout << "ERROR signal-rejection mode 2 requires a supported generatorName "
           << "(LHC20g10 or LHC24a1)" << endl;
      return;
    }
    analysisEventCuts->SetAcceptedHeader(HeaderList);
  }

  AliConversionPhotonCuts *analysisCuts = new AliConversionPhotonCuts();
  analysisCuts->SetV0ReaderName(V0ReaderName);
  if (enableElecDeDxPostCalibration){
    if (isMC == 0){
      if(fileNamedEdxPostCalib.CompareTo("") != 0){
        analysisCuts->SetElecDeDxPostCalibrationCustomFile(fileNamedEdxPostCalib);
        cout << "Setting custom dEdx recalibration file: " << fileNamedEdxPostCalib.Data() << endl;
     }
      analysisCuts->SetDoElecDeDxPostCalibration(enableElecDeDxPostCalibration);
      cout << "Enabled TPC dEdx recalibration." << endl;
    } else{
      cout << "ERROR enableElecDeDxPostCalibration set to True even if MC file. Automatically reset to 0"<< endl;
      enableElecDeDxPostCalibration=kFALSE;
      analysisCuts->SetDoElecDeDxPostCalibration(kFALSE);
    }
  }
  analysisCuts->InitializeCutsFromCutString(TaskPhotonCutnumber.Data());
  analysisCuts->SetFillCutHistograms("",kFALSE);

  AliAnalysisTaskConversionQA *fQA = new AliAnalysisTaskConversionQA(Form("%s_%s_QA",TaskEventCutnumber.Data(),TaskPhotonCutnumber.Data()));
  fQA->SetEventCuts(analysisEventCuts,IsHeavyIon);
  fQA->SetConversionCuts(analysisCuts,IsHeavyIon);
  fQA->FillType(kTree,kTreeHighPt,kHistograms);
  fQA->SetIsMC(isMC);
  fQA->SetV0ReaderName(V0ReaderName);
  mgr->AddTask(fQA);

  AliAnalysisDataContainer *cinput  = mgr->GetCommonInputContainer();
  if (runBasicQAWithStandardOutput){
    AliAnalysisDataContainer *coutput =
      mgr->CreateContainer(Form("GammaConv_V1QA_%s_%s", TaskEventCutnumber.Data(), TaskPhotonCutnumber.Data()), TList::Class(),
        AliAnalysisManager::kOutputContainer, Form("%s:GammaConvV1_QA_%s_%s",AliAnalysisManager::GetCommonFileName(), TaskEventCutnumber.Data(), TaskPhotonCutnumber.Data()));
    mgr->ConnectOutput(fQA,  1, coutput);
  } else {
    AliAnalysisDataContainer *coutput =
      mgr->CreateContainer(Form("GammaConv_V1QA_%s_%s", TaskEventCutnumber.Data(), TaskPhotonCutnumber.Data()), TList::Class(),
        AliAnalysisManager::kOutputContainer, Form("GammaConvV1_QA_%s_%s.root", TaskEventCutnumber.Data(), TaskPhotonCutnumber.Data()));
    mgr->ConnectOutput(fQA,  1, coutput);
  }
  mgr->ConnectOutput(fQA,2,mgr->CreateContainer(Form("PhotonQA_%s_%s", TaskEventCutnumber.Data(), TaskPhotonCutnumber.Data()), TTree::Class(), AliAnalysisManager::kOutputContainer, AliAnalysisManager::GetCommonFileName()) );
  mgr->ConnectInput(fQA,0,cinput);


  //connect containers
  return;
}
