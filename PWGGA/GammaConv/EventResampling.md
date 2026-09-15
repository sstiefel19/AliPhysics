# Event resampling in GammaConvV1

`AliAnalysisTaskGammaConvV1` supports an optional delete-one-group jackknife mode.
It is intended to provide statistically matched inputs for covariance estimates
between the inclusive-photon yield, meson yields, `Rgamma`, the direct-photon
yield, and different transverse-momentum bins.

The mode is disabled by default. When enabled, every event is assigned
deterministically to one of `K` folds from its run and event identifiers. A task
instance excludes one fold before any photon, meson, or mixed-event processing.
Running one independent task instance for every excluded fold therefore gives
`K` delete-one-group replicas. Each task has its own background handlers, so its
event-mixing pools contain only events retained in that replica.

## Configuration

The final arguments of `AddTask_GammaConvV1_PbPb` are:

```cpp
TString   additionalTrainConfig                = "0",
Int_t     eventResamplingNSubsamples           = 0,
Int_t     eventResamplingExcludedSubsample     = -1,
ULong64_t eventResamplingSeed                  = 0
```

For example, the following produces replica 3 of a 20-fold jackknife:

```cpp
AddTask_GammaConvV1_PbPb(
  trainConfig, isMC, photonCutNumberV0Reader, periodNameV0Reader,
  enableQAMesonTask, enableQAPhotonTask, enableLightOutput,
  enableTHnSparse, enableTriggerMimicking, enableTriggerOverlapRej,
  settingMaxFacPtHard, debugLevel, fileNameExternalInputs,
  acceptedAddedParticles, intPtWeightsCalculationMethod, generatorName,
  enableMultiplicityWeighting, periodNameAnchor, enableMatBudWeightsPi0,
  enableElecDeDxPostCalibration, enableFlattening, enableChargedPrimary,
  enablePlotVsCentrality, processAODcheckForV0s, theUseGetMesonWeightNew,
  additionalTrainConfig, 20, 3, 0);
```

Create all replicas with the same `K` and seed and with excluded-fold indices
`0` through `K-1`. Their output names include, for example,
`_JK03of20_S0`, preventing task, container, and file-name collisions when the
replicas are run in the same analysis manager.

Each output contains two top-level QA histograms:

- `EventResampling`: numbers of events seen, retained, and excluded;
- `EventResamplingSubsample`: occupancy of every fold before exclusion.

The fold occupancy must be checked before using the replicas. Empty or strongly
imbalanced folds indicate that the event identifiers or the chosen `K` are not
suitable for that production.

## Covariance calculation

Every replica must be carried through the same correction, cocktail, `Rgamma`,
direct-photon, and fit workflow as the nominal result. For a vector of final
observables `theta^(k)` from the replica that excludes fold `k`, calculate

```text
theta_bar = (1/K) sum_k theta^(k)
Cov(theta_i, theta_j) = ((K-1)/K)
                        sum_k (theta_i^(k)-theta_bar_i)
                              (theta_j^(k)-theta_bar_j)
```

The vector can contain both `gammaInc` and `Rgamma` bins, so the same covariance
matrix provides same-bin `Cov(gammaInc_i,Rgamma_i)`, cross-bin
`Cov(gammaInc_i,Rgamma_j)`, and within-observable cross-bin terms. Derived
quantities must be calculated separately for every replica; they must not be
formed from independently resampled inputs.

## Scope

This implementation resamples the GammaConvV1 event processing, including its
mixed-event background. It does not automatically rerun downstream correction
or cocktail macros. Those steps must consume the corresponding replica outputs.
Systematic variations are separate from this statistical jackknife covariance.
