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

Resampling is requested through `additionalTrainConfig` of
`AddTask_GammaConvV1_PbPb`, like the other special settings, with the token

```text
JK<excluded>of<K>[s<seed>]
```

For example, `"0_JK3of20"` produces replica 3 of a 20-fold jackknife with seed
0, and `"0_JK3of20s7"` the same replica with seed 7. The first token remains the
usual additional train-config counter. Without a `JK` token nothing changes: the
task, container, and file names and the output content are identical to a build
without this feature.

Create all replicas with the same `K` and seed and with excluded-fold indices
`0` through `K-1`, i.e. `K` wagons of the same train configuration whose
`additionalTrainConfig` differ only in the `JK` token. All replicas are written
to the same output file as the nominal task, `GCo_<trainConfig>.root`; the task
and container names carry a suffix such as `_JK03of20_S0`, so the replicas do
not collide with each other or with the nominal task.

Each replica's output list contains two additional QA histograms:

- `EventResampling`: numbers of events seen, retained, and excluded;
- `EventResamplingSubsample`: occupancy of every fold before exclusion.

The fold occupancy must be checked before using the replicas. Fold populations
that deviate from `N/K` by much more than statistical fluctuations indicate that
the event identifiers or the chosen `K` are not suitable for that production,
and the replicas must not be used.

## Fold assignment

Each event's fold is a hash of the seed, the run number, the period/orbit/bunch
crossing numbers from the event header, the name of the input file, and the
event's entry within that file. The header numbers are the event identity for
real data; the file name and local entry cover simulated productions whose
headers carry no or non-unique identifiers. Because all ingredients are
properties of the data set rather than of the job, the assignment is the same
for every replica in a train and reproducible in a later train or a rerun of a
single wagon, as long as the input files are read from the same catalogue
paths (which is the case on the grid).

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
