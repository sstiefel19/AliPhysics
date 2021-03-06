/*********************************************************************
 *                                                                   *
 * Configfemtoanalysis.C - configuration macro for the femtoscopic   *
 * analysis, meant as a QA process for two-particle effects          *
 *                                                                   *
 * Author: Adam Kisiel (Adam.Kisiel@cern.ch)                         *
 *                                                                   *
 *********************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT_)
#include "AliFemtoManager.h"
#include "AliFemtoEventReaderESDChain.h"
#include "AliFemtoEventReaderESDChainKine.h"
#include "AliFemtoEventReaderAODChain.h"
#include "AliFemtoSimpleAnalysis.h"
#include "AliFemtoBasicEventCut.h"
#include "AliFemtoESDTrackCut.h"
#include "AliFemtoCorrFctn.h"
#include "AliFemtoCutMonitorParticleYPt.h"
#include "AliFemtoCutMonitorParticleVertPos.h"
#include "AliFemtoCutMonitorParticleMomRes.h"
#include "AliFemtoCutMonitorParticlePID.h"
#include "AliFemtoCutMonitorEventMult.h"
#include "AliFemtoCutMonitorEventVertex.h"
#include "AliFemtoShareQualityTPCEntranceSepPairCut.h"
#include "AliFemtoPairCutAntiGamma.h"
#include "AliFemtoPairCutRadialDistance.h"
#include "AliFemtoQinvCorrFctn.h"
#include "AliFemtoCorrFctnNonIdDR.h"
#include "AliFemtoShareQualityCorrFctn.h"
#include "AliFemtoTPCInnerCorrFctn.h"
#include "AliFemtoVertexMultAnalysis.h"
#include "AliFemtoCorrFctn3DSpherical.h"
#include "AliFemtoChi2CorrFctn.h"
#include "AliFemtoCorrFctnTPCNcls.h"
#include "AliFemtoBPLCMS3DCorrFctn.h"
#include "AliFemtoCorrFctn3DLCMSSym.h"
#include "AliFemtoModelBPLCMSCorrFctn.h"
#include "AliFemtoModelCorrFctn3DSpherical.h"
#include "AliFemtoModelGausLCMSFreezeOutGenerator.h"
#include "AliFemtoModelGausRinvFreezeOutGenerator.h"
#include "AliFemtoModelManager.h"
#include "AliFemtoModelWeightGeneratorBasic.h"
#include "AliFemtoModelWeightGeneratorLednicky.h"
#include "AliFemtoCorrFctnDirectYlm.h"
#include "AliFemtoModelCorrFctnDirectYlm.h"
#include "AliFemtoModelCorrFctnSource.h"
#include "AliFemtoCutMonitorParticlePtPDG.h"
#include "AliFemtoKTPairCut.h"
#include "AliFemtoAvgSepCorrFctn.h"
#endif

//________________________________________________________________________
AliFemtoManager* ConfigFemtoAnalysis() {

  double PionMass = 0.13956995;
  double KaonMass = 0.493677;
  double ProtonMass = 0.938272013;

  // double psi = TMath::Pi()/2.;
  // double psid = TMath::Pi()/6.;

  // int runepvzero[7] = {1, 1, 1, 1, 1, 1, 1};
  // double epvzerobins[7] = {-psi, -psi+psid, -psi+2*psid, -psi+3*psid, -psi+4*psid, -psi+5*psid, -psi+6*psid};

  double psi = TMath::Pi()/2.;
  double psid = TMath::Pi()/3.;

  int runepvzero[4] = {0, 0, 0, 1};
  double epvzerobins[4] = {-psi, -psi+psid, -psi+2*psid, -psi+3*psid};

  int runmults[10] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  int multbins[11] = {0.001, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900};

  int runch[3] = {1, 1, 1};
  const char *chrgs[3] = { "PP", "APAP", "PAP" };

  int runktdep = 1;
  double ktrng[3] = {0.01, 1.0, 5.0};

  int numOfMultBins = 10;
  int numOfChTypes = 3;
  int numOfkTbins = 2;
  int numOfEPvzero = 4;

  int runqinv = 1;
  int runshlcms = 0;// 0:PRF(PAP), 1:LCMS(PP,APAP)

  int runtype = 2; // Types 0 - global, 1 - ITS only, 2 - TPC Inner
  int isrealdata = 1;

  //  int gammacut = 1;

  double shqmax = 1.0;
  int nbinssh = 100;

  AliFemtoEventReaderAODChain *Reader = new AliFemtoEventReaderAODChain();
  Reader->SetFilterBit(7);
  Reader->SetCentralityPreSelection(0.001, 310);
  Reader->SetEPVZERO(kTRUE);
  Reader->SetCentralityFlattening(kTRUE);
  Reader->SetDCAglobalTrack(kTRUE);

  AliFemtoManager* Manager = new AliFemtoManager();
  Manager->SetEventReader(Reader);

  AliFemtoVertexMultAnalysis    *anetaphitpc[10*3*2];
  AliFemtoBasicEventCut         *mecetaphitpc[10*3*2];
  AliFemtoCutMonitorEventMult   *cutPassEvMetaphitpc[50];
  AliFemtoCutMonitorEventMult   *cutFailEvMetaphitpc[50];
  // AliFemtoCutMonitorEventVertex *cutPassEvVetaphitpc[50];
  // AliFemtoCutMonitorEventVertex *cutFailEvVetaphitpc[50];
  AliFemtoESDTrackCut           *dtc1etaphitpc[50];
  AliFemtoESDTrackCut           *dtc2etaphitpc[50];
  AliFemtoCutMonitorParticleYPt *cutPass1YPtetaphitpc[50];
  AliFemtoCutMonitorParticleYPt *cutFail1YPtetaphitpc[50];
  AliFemtoCutMonitorParticlePID *cutPass1PIDetaphitpc[50];
  AliFemtoCutMonitorParticlePID *cutFail1PIDetaphitpc[50];
  AliFemtoCutMonitorParticleYPt *cutPass2YPtetaphitpc[50];
  AliFemtoCutMonitorParticleYPt *cutFail2YPtetaphitpc[50];
  AliFemtoCutMonitorParticlePID *cutPass2PIDetaphitpc[50];
  AliFemtoCutMonitorParticlePID *cutFail2PIDetaphitpc[50];
  //   AliFemtoPairCutAntiGamma      *sqpcetaphitpcdiff[10*3];
  //   AliFemtoShareQualityTPCEntranceSepPairCut      *sqpcetaphitpcsame[10*3];
  //AliFemtoPairCutAntiGamma      *sqpcetaphitpc[10*3];
  AliFemtoPairCutRadialDistance      *sqpcetaphitpc[50];
  //  AliFemtoChi2CorrFctn          *cchiqinvetaphitpc[20*2];
  AliFemtoKTPairCut             *ktpcuts[50*2];
  AliFemtoCorrFctnDirectYlm     *cylmtpc[50];
  AliFemtoCorrFctnDirectYlm     *cylmkttpc[50*2];
  AliFemtoCorrFctnDirectYlm     *cylmetaphitpc[10*3];
  AliFemtoQinvCorrFctn          *cqinvkttpc[50*2];
  AliFemtoQinvCorrFctn          *cqinvtpc[50];
  AliFemtoCorrFctnNonIdDR       *ckstartpc[50];
  AliFemtoCorrFctnNonIdDR       *ckstarkttpc[50*2];
  AliFemtoCorrFctnDEtaDPhi      *cdedpetaphi[50*2];
  AliFemtoAvgSepCorrFctn          *cAvgSeptpc[50];

  //   AliFemtoCorrFctn3DLCMSSym     *cq3dlcmskttpc[20*2];
  //   AliFemtoCorrFctnTPCNcls       *cqinvnclstpc[20];
  //   AliFemtoShareQualityCorrFctn  *cqinvsqtpc[20*10];
  //   AliFemtoChi2CorrFctn          *cqinvchi2tpc[20];
    AliFemtoTPCInnerCorrFctn      *cqinvinnertpc[50];

  // *** Third QA task - HBT analysis with all pair cuts off, TPC only ***
  // *** Begin pion-pion (positive) analysis ***
  int aniter = 0;

  for (int imult = 0; imult < numOfMultBins; imult++) {
    if (runmults[imult]) {

      for (int ichg = 0; ichg < numOfChTypes; ichg++) {
        if (runch[ichg]) {

          for (int iepvzero = 0; iepvzero < numOfEPvzero; iepvzero++) {
            if (runepvzero[iepvzero]) {

              aniter = imult * numOfChTypes + ichg * numOfEPvzero + iepvzero;
              // aniter = ichg * numOfMultBins + imult * numOfEPvzero + iepvzero;

              // cout << "aniter = " << aniter << endl;
              //        aniter = ichg * numOfMultBins + imult;

              // if (ichg == 2)
              // 	runshlcms = 0;
              // else
              // 	runshlcms = 1;


              //________________________

              anetaphitpc[aniter] = new AliFemtoVertexMultAnalysis(8, -8.0, 8.0, 4, multbins[imult], multbins[imult+1]);
              anetaphitpc[aniter]->SetNumEventsToMix(10);
              anetaphitpc[aniter]->SetMinSizePartCollection(1);
              anetaphitpc[aniter]->SetVerboseMode(kFALSE);

              mecetaphitpc[aniter] = new AliFemtoBasicEventCut();
              mecetaphitpc[aniter]->SetEventMult(0.001,100000);
              mecetaphitpc[aniter]->SetVertZPos(-8,8);

              if (iepvzero == 3)
                mecetaphitpc[aniter]->SetEPVZERO(epvzerobins[0],epvzerobins[3]);
              else
                mecetaphitpc[aniter]->SetEPVZERO(epvzerobins[iepvzero],epvzerobins[iepvzero+1]);


              cutPassEvMetaphitpc[aniter] = new AliFemtoCutMonitorEventMult(Form("cutPass%stpcM%iPsi%i", chrgs[ichg], imult, iepvzero));
              cutFailEvMetaphitpc[aniter] = new AliFemtoCutMonitorEventMult(Form("cutFail%stpcM%iPsi%i", chrgs[ichg], imult, iepvzero));
              mecetaphitpc[aniter]->AddCutMonitor(cutPassEvMetaphitpc[aniter], cutFailEvMetaphitpc[aniter]);

              // cutPassEvVetaphitpc[aniter] = new AliFemtoCutMonitorEventVertex(Form("cutPass%stpcM%i", chrgs[ichg], imult));
              // cutFailEvVetaphitpc[aniter] = new AliFemtoCutMonitorEventVertex(Form("cutFail%stpcM%i", chrgs[ichg], imult));
              // mecetaphitpc[aniter]->AddCutMonitor(cutPassEvVetaphitpc[aniter], cutFailEvVetaphitpc[aniter]);

              dtc1etaphitpc[aniter] = new AliFemtoESDTrackCut();
              dtc2etaphitpc[aniter] = new AliFemtoESDTrackCut();

              if (ichg == 0) {
                dtc1etaphitpc[aniter]->SetCharge(1.0);
                dtc1etaphitpc[aniter]->SetPt(0.7,4.0);
              }
              else if (ichg == 1) {
                dtc1etaphitpc[aniter]->SetCharge(-1.0);
                dtc1etaphitpc[aniter]->SetPt(0.7,4.0);
              }
              else if (ichg == 2) {
                dtc1etaphitpc[aniter]->SetCharge(-1.0);
                dtc2etaphitpc[aniter]->SetCharge(1.0);
                dtc1etaphitpc[aniter]->SetPt(0.7,4.0);
                dtc2etaphitpc[aniter]->SetPt(0.7,4.0);
              }

              dtc1etaphitpc[aniter]->SetEta(-0.8,0.8);
              dtc1etaphitpc[aniter]->SetMass(ProtonMass);
              dtc1etaphitpc[aniter]->SetMostProbableProton();
              dtc1etaphitpc[aniter]->SetNsigma(3.0);
              //dtc1etaphitpc[aniter]->SetNsigma(2.0);
              dtc1etaphitpc[aniter]->SetNsigmaTPCTOF(kTRUE);
              //dtc1etaphitpc[aniter]->SetNsigmaTPConly(kTRUE);

              if (ichg == 2) {
                dtc2etaphitpc[aniter]->SetEta(-0.8,0.8);
                dtc2etaphitpc[aniter]->SetMass(ProtonMass);
                dtc2etaphitpc[aniter]->SetMostProbableProton();
                dtc2etaphitpc[aniter]->SetNsigma(3.0);
                //dtc2etaphitpc[aniter]->SetNsigma(2.0);
                dtc2etaphitpc[aniter]->SetNsigmaTPCTOF(kTRUE);
                //dtc2etaphitpc[aniter]->SetNsigmaTPConly(kTRUE);

              }

              // Track quality cuts

              if (runtype == 0) {
                dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kTPCrefit|AliESDtrack::kITSrefit);
                //	    dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kTPCrefit);
                //    dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kITSrefit);
                dtc1etaphitpc[aniter]->SetminTPCncls(80);
                dtc1etaphitpc[aniter]->SetRemoveKinks(kTRUE);
                dtc1etaphitpc[aniter]->SetLabel(kFALSE);
                //    dtc1etaphitpc[aniter]->SetMaxITSChiNdof(6.0);
                dtc1etaphitpc[aniter]->SetMaxTPCChiNdof(4.0);
                dtc1etaphitpc[aniter]->SetMaxImpactXY(0.2);
                //            dtc1etaphitpc[aniter]->SetMaxImpactXYPtDep(0.0182, 0.0350, -1.01);
                dtc1etaphitpc[aniter]->SetMaxImpactZ(0.15);
                //      dtc1etaphitpc[aniter]->SetMaxSigmaToVertex(6.0);
              }
              else if (runtype == 1) {
                //      dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kTPCrefit|AliESDtrack::kITSrefit);
                //    dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kTPCrefit);
                //	    dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kITSrefit|AliESDtrack::kITSpureSA);
                //      dtc1etaphitpc[aniter]->SetminTPCncls(70);
                dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kITSrefit);
                dtc1etaphitpc[aniter]->SetRemoveKinks(kTRUE);
                dtc1etaphitpc[aniter]->SetLabel(kFALSE);
                //    dtc1etaphitpc[aniter]->SetMaxITSChiNdof(6.0);
                //      dtc1etaphitpc[aniter]->SetMaxTPCChiNdof(6.0);
                dtc1etaphitpc[aniter]->SetMaxImpactXY(0.2);
                dtc1etaphitpc[aniter]->SetMaxImpactZ(0.25);
                //      dtc1etaphitpc[aniter]->SetMaxSigmaToVertex(6.0);
              }
              else if (runtype == 2) {
                //dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kTPCrefit|AliESDtrack::kITSrefit);
                dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kTPCin);
                dtc1etaphitpc[aniter]->SetminTPCncls(80);
                dtc1etaphitpc[aniter]->SetRemoveKinks(kTRUE);
                dtc1etaphitpc[aniter]->SetLabel(kFALSE);
                dtc1etaphitpc[aniter]->SetMaxTPCChiNdof(4.0);

                // dtc1etaphitpc[aniter]->SetMaxImpactXYPtDep(0.0205, 0.035, -1.1);     //      DCA xy
                // dtc1etaphitpc[aniter]->SetMaxImpactXYPtDep(0.018, 0.035, -1.01);     //      DCA xy

                // // tpc-only
                // dtc1etaphitpc[aniter]->SetMaxImpactXY(2.4); // 2.4 0.1
                // dtc1etaphitpc[aniter]->SetMaxImpactZ(3.2); // 2.0 0.1

                // // global
                dtc1etaphitpc[aniter]->SetMaxImpactXYPtDep(0.035, 0.042, -0.9);     //      DCA xy
                dtc1etaphitpc[aniter]->SetMaxImpactZ(0.1); // 2.0 0.1

                if (ichg == 2) {
                  //dtc1etaphitpc[aniter]->SetStatus(AliESDtrack::kTPCrefit|AliESDtrack::kITSrefit);
                  dtc2etaphitpc[aniter]->SetStatus(AliESDtrack::kTPCin);
                  dtc2etaphitpc[aniter]->SetminTPCncls(80);
                  dtc2etaphitpc[aniter]->SetRemoveKinks(kTRUE);
                  dtc2etaphitpc[aniter]->SetLabel(kFALSE);
                  dtc2etaphitpc[aniter]->SetMaxTPCChiNdof(4.0);
                  // dtc2etaphitpc[aniter]->SetMaxImpactXY(2.4); // 2.4 0.1
                  // // dtc2etaphitpc[aniter]->SetMaxImpactXYPtDep(0.0205, 0.035, -1.1);     //      DCA xy
                  // //dtc2etaphitpc[aniter]->SetMaxImpactXYPtDep(0.018, 0.035, -1.01);     //      DCA xy
                  // dtc2etaphitpc[aniter]->SetMaxImpactZ(3.2); // 2.0 0.1

                  // // tpc-only
                  // dtc1etaphitpc[aniter]->SetMaxImpactXY(2.4); // 2.4 0.1
                  // dtc1etaphitpc[aniter]->SetMaxImpactZ(3.2); // 2.0 0.1

                  // // global
                  dtc1etaphitpc[aniter]->SetMaxImpactXYPtDep(0.035, 0.042, -0.9);     //      DCA xy
                  dtc1etaphitpc[aniter]->SetMaxImpactZ(0.1); // 2.0 0.1

                }

              }

              cutPass1YPtetaphitpc[aniter] = new AliFemtoCutMonitorParticleYPt(Form("cutPass1%stpcM%iPsi%i", chrgs[ichg], imult, iepvzero),ProtonMass);
              cutFail1YPtetaphitpc[aniter] = new AliFemtoCutMonitorParticleYPt(Form("cutFail1%stpcM%iPsi%i", chrgs[ichg], imult, iepvzero),ProtonMass);
              dtc1etaphitpc[aniter]->AddCutMonitor(cutPass1YPtetaphitpc[aniter], cutFail1YPtetaphitpc[aniter]);

              cutPass1PIDetaphitpc[aniter] = new AliFemtoCutMonitorParticlePID(Form("cutPass1%stpcM%iPsi%i", chrgs[ichg], imult, iepvzero),2);//0-pion,1-kaon,2-proton
              cutFail1PIDetaphitpc[aniter] = new AliFemtoCutMonitorParticlePID(Form("cutFail1%stpcM%iPsi%i", chrgs[ichg], imult , iepvzero),2);
              dtc1etaphitpc[aniter]->AddCutMonitor(cutPass1PIDetaphitpc[aniter], cutFail1PIDetaphitpc[aniter]);

              // if (ichg == 2){
              //     cutPass2PIDetaphitpc[aniter] = new AliFemtoCutMonitorParticlePID(Form("cutPass2%stpcM%i", chrgs[ichg], imult),2);//0-pion,1-kaon,2-proton
              //     cutFail2PIDetaphitpc[aniter] = new AliFemtoCutMonitorParticlePID(Form("cutFail2%stpcM%i", chrgs[ichg], imult),2);
              //     dtc2etaphitpc[aniter]->AddCutMonitor(cutPass2PIDetaphitpc[aniter], cutFail2PIDetaphitpc[aniter]);
              // }

              // sqpcetaphitpc[aniter] = new AliFemtoPairCutAntiGamma();
              sqpcetaphitpc[aniter] = new AliFemtoPairCutRadialDistance();

              if (runtype == 0) {
                sqpcetaphitpc[aniter]->SetShareQualityMax(1.0);
                sqpcetaphitpc[aniter]->SetShareFractionMax(0.05);
                sqpcetaphitpc[aniter]->SetRemoveSameLabel(kFALSE);
                // sqpcetaphitpc[aniter]->SetMaxEEMinv(0.0);
                // sqpcetaphitpc[aniter]->SetMaxThetaDiff(0.0);
                //	    sqpcetaphitpc[aniter]->SetTPCEntranceSepMinimum(1.5);
                //sqpcetaphitpc[aniter]->SetRadialDistanceMinimum(0.12, 0.03);
                //	    sqpcetaphitpc[aniter]->SetEtaDifferenceMinimum(0.02);
              }
              else if (runtype == 1) {
                sqpcetaphitpc[aniter]->SetShareQualityMax(1.0);
                sqpcetaphitpc[aniter]->SetShareFractionMax(1.05);
                sqpcetaphitpc[aniter]->SetRemoveSameLabel(kFALSE);
                // sqpcetaphitpc[aniter]->SetMaxEEMinv(0.002);
                // sqpcetaphitpc[aniter]->SetMaxThetaDiff(0.008);
                //	    sqpcetaphitpc[aniter]->SetTPCEntranceSepMinimum(5.0);
                //sqpcetaphitpc[aniter]->SetRadialDistanceMinimum(1.2, 0.03);
                //	    sqpcetaphitpc[aniter]->SetEtaDifferenceMinimum(0.02);
              }
              else if (runtype == 2) {
                //sqpcetaphitpc[aniter]->SetUseAOD(kTRUE);

                sqpcetaphitpc[aniter]->SetShareQualityMax(1.0);
                sqpcetaphitpc[aniter]->SetShareFractionMax(0.05);
                sqpcetaphitpc[aniter]->SetRemoveSameLabel(kFALSE);

                //	    if (gammacut == 0) {
                //sqpcetaphitpc[aniter]->SetMaxEEMinv(0.0);
                //sqpcetaphitpc[aniter]->SetMaxThetaDiff(0.0);
                //}
                //else if (gammacut == 1) {
                //sqpcetaphitpc[aniter]->SetMaxEEMinv(0.002);
                //sqpcetaphitpc[aniter]->SetMaxThetaDiff(0.008);
                //}

                // sqpcetaphitpc[aniter]->SetMagneticFieldSign(-1); // field1 -1, field3 +1
                // sqpcetaphitpc[aniter]->SetMinimumRadius(0.8); // biggest inefficiency for R=1.1 m (checked on small sample)

                sqpcetaphitpc[aniter]->SetMinimumRadius(1.2); //0.8
                sqpcetaphitpc[aniter]->SetPhiStarMin(kFALSE);
                sqpcetaphitpc[aniter]->SetPhiStarDifferenceMinimum(0.045); // 0.012 - pions, 0.017 - kaons, 0.018
                sqpcetaphitpc[aniter]->SetEtaDifferenceMinimum(0.01); // 0.017 - pions, 0.015 - kaons

              }

              anetaphitpc[aniter]->SetEventCut(mecetaphitpc[aniter]);

              if (ichg == 2) {
                anetaphitpc[aniter]->SetFirstParticleCut(dtc1etaphitpc[aniter]);
                anetaphitpc[aniter]->SetSecondParticleCut(dtc2etaphitpc[aniter]);
              }
              else {
                anetaphitpc[aniter]->SetFirstParticleCut(dtc1etaphitpc[aniter]);
                anetaphitpc[aniter]->SetSecondParticleCut(dtc1etaphitpc[aniter]);
              }

              anetaphitpc[aniter]->SetPairCut(sqpcetaphitpc[aniter]);


              if (ichg == 2) {
                ckstartpc[aniter] = new AliFemtoCorrFctnNonIdDR(Form("ckstar%stpcM%iPsi%i", chrgs[ichg], imult, iepvzero),nbinssh,0.0,shqmax);
                anetaphitpc[aniter]->AddCorrFctn(ckstartpc[aniter]);
              }
              else {

                cqinvtpc[aniter] = new AliFemtoQinvCorrFctn(Form("cqinv%stpcM%iPsi%i", chrgs[ichg], imult, iepvzero),2*nbinssh,0.0,2*shqmax);
                anetaphitpc[aniter]->AddCorrFctn(cqinvtpc[aniter]);

              }

              // cylmtpc[aniter] = new AliFemtoCorrFctnDirectYlm(Form("cylm%stpcM%i", chrgs[ichg], imult),2,nbinssh, 0.0,shqmax,runshlcms);
              // anetaphitpc[aniter]->AddCorrFctn(cylmtpc[aniter]);


              // cAvgSeptpc[aniter] = new AliFemtoAvgSepCorrFctn(Form("cAvgSep%stpcM%iPsi%i", chrgs[ichg], imult, iepvzero),4*nbinssh,0.0,200);
              // anetaphitpc[aniter]->AddCorrFctn(cAvgSeptpc[aniter]);

              cqinvinnertpc[aniter] = new AliFemtoTPCInnerCorrFctn(Form("cqinvinner%stpcM%d", chrgs[ichg], imult),nbinssh,0.0,shqmax);
              cqinvinnertpc[aniter]->SetRadius(1.2);
              anetaphitpc[aniter]->AddCorrFctn(cqinvinnertpc[aniter]);


              if (runktdep) {
                int ktm;
                for (int ikt=0; ikt<numOfkTbins; ikt++) {

                  ktm = aniter * numOfkTbins + ikt;
                  ktpcuts[ktm] = new AliFemtoKTPairCut(ktrng[ikt], ktrng[ikt+1]);


                  // cylmkttpc[ktm] = new AliFemtoCorrFctnDirectYlm(Form("cylm%stpcM%ikT%i", chrgs[ichg], imult, ikt),2,nbinssh,0.0,shqmax,runshlcms);
                  // cylmkttpc[ktm]->SetPairSelectionCut(ktpcuts[ktm]);
                  // anetaphitpc[aniter]->AddCorrFctn(cylmkttpc[ktm]);

                  if (ichg == 2) {
                    ckstarkttpc[ktm] = new AliFemtoCorrFctnNonIdDR(Form("ckstar%stpcM%iPsi%ikT%i", chrgs[ichg], imult, iepvzero, ikt),nbinssh,0.0,shqmax);
                    ckstarkttpc[ktm]->SetPairSelectionCut(ktpcuts[ktm]);
                    anetaphitpc[aniter]->AddCorrFctn(ckstarkttpc[ktm]);


                  }
                  else {
                    cqinvkttpc[ktm] = new AliFemtoQinvCorrFctn(Form("cqinv%stpcM%iPsi%ikT%i", chrgs[ichg], imult, iepvzero, ikt),2*nbinssh,0.0,2*shqmax);
                    cqinvkttpc[ktm]->SetPairSelectionCut(ktpcuts[ktm]);
                    anetaphitpc[aniter]->AddCorrFctn(cqinvkttpc[ktm]);
                  }

                  // 	      cqinvsqtpc[ktm] = new AliFemtoShareQualityCorrFctn(Form("cqinvsq%stpcM%ikT%i", chrgs[ichg], imult, ikt),nbinssh,0.0,shqmax);
                  // 	      cqinvsqtpc[ktm]->SetPairSelectionCut(ktpcuts[ktm]);
                  // 	      anetaphitpc[aniter]->AddCorrFctn(cqinvsqtpc[ktm]);

                  // cqinvinnertpc[ktm] = new AliFemtoTPCInnerCorrFctn(Form("cqinvinner%stpcM%ikT%i", chrgs[ichg], imult, ikt),nbinssh,0.0,shqmax);
                  // cqinvinnertpc[ktm]->SetPairSelectionCut(ktpcuts[ktm]);
                  // cqinvinnertpc[ktm]->SetRadius(1.2);
                  // anetaphitpc[aniter]->AddCorrFctn(cqinvinnertpc[ktm]);

                  // 	      if (run3d) {
                  // 		cq3dlcmskttpc[ktm] = new AliFemtoCorrFctn3DLCMSSym(Form("cq3d%stpcM%ikT%i", chrgs[ichg], imult, ikt),60,(imult>3)?((imult>6)?((imult>7)?0.6:0.4):0.25):0.15);
                  // 		cq3dlcmskttpc[ktm]->SetPairSelectionCut(ktpcuts[ktm]);
                  // 		anetaphitpc[aniter]->AddCorrFctn(cq3dlcmskttpc[ktm]);
                  // 	      }
                }
              }

              // cdedpetaphi[aniter] = new AliFemtoCorrFctnDEtaDPhi(Form("cdedp%stpcM%i", chrgs[ichg], imult),240, 240);
              // anetaphitpc[aniter]->AddCorrFctn(cdedpetaphi[aniter]);

              Manager->AddAnalysis(anetaphitpc[aniter]);
            }
          }
        }
      }
    }
  }


// *** End pion-pion (positive) analysis


  return Manager;
}
