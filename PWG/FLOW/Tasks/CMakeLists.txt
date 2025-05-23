# **************************************************************************
# * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
# *                                                                        *
# * Author: The ALICE Off-line Project.                                    *
# * Contributors are mentioned in the code where appropriate.              *
# *                                                                        *
# * Permission to use, copy, modify and distribute this software and its   *
# * documentation strictly for non-commercial purposes is hereby granted   *
# * without fee, provided that the above copyright notice appears in all   *
# * copies and that both the copyright notice and this permission notice   *
# * appear in the supporting documentation. The authors make no claims     *
# * about the suitability of this software for any purpose. It is          *
# * provided "as is" without express or implied warranty.                  *
# **************************************************************************/

#Module
set(MODULE PWGflowTasks)
add_definitions(-D_MODULE_="${MODULE}")

# Module include folder
include_directories(${AliPhysics_SOURCE_DIR}/PWG/FLOW/Tasks)

# Additional includes - alphabetical order except ROOT
include_directories(${ROOT_INCLUDE_DIRS}
                    ${AliPhysics_SOURCE_DIR}/CORRFW
                    ${AliPhysics_SOURCE_DIR}/OADB
                    ${AliPhysics_SOURCE_DIR}/OADB/COMMON/MULTIPLICITY
                    ${AliPhysics_SOURCE_DIR}/PWG/FLOW/Base
                    ${AliPhysics_SOURCE_DIR}/PWG/muon
                    ${AliPhysics_SOURCE_DIR}/PWG/Tools
                    ${AliPhysics_SOURCE_DIR}/PWG/DevNanoAOD
                    ${AliPhysics_SOURCE_DIR}/PWG/EMCAL/EMCALbase
                    ${AliPhysics_SOURCE_DIR}/PWG/EMCAL/EMCALtasks
                    ${AliPhysics_SOURCE_DIR}/PWG/EMCAL/EMCALtrigger
                    ${AliPhysics_SOURCE_DIR}/PWGGA/GammaConv
                    ${AliPhysics_SOURCE_DIR}/PWG/CaloTrackCorrBase
                    ${AliPhysics_SOURCE_DIR}/PWG/Cocktail
                    ${AliPhysics_SOURCE_DIR}/PWG/FLOW
                    ${AliPhysics_SOURCE_DIR}/PWG/JETFW
                    ${AliPhysics_SOURCE_DIR}/PWG/TRD
                    ${AliPhysics_SOURCE_DIR}/PWGJE/EMCALJetTasks
                    ${AliPhysics_SOURCE_DIR}/TENDER/Tender
                    ${AliPhysics_SOURCE_DIR}/TENDER/TenderSupplies
                    ${AliPhysics_SOURCE_DIR}/PWGGA/PHOSTasks/PHOS_PbPb
                    ${AliPhysics_SOURCE_DIR}/PWGGA/PWGGAUtils
                    ${AliPhysics_SOURCE_DIR}/PWGGA/Common
                    ${AliPhysics_SOURCE_DIR}/PWGGA/GammaConvBase
		    ${AliPhysics_SOURCE_DIR}/PWGCF/Correlations/JCORRAN/Base
  )

# Sources - alphabetical order
set(SRCS
  AliFlowEventSimpleMaker.cxx
  AliFlowEvent.cxx
  AliFlowEventCuts.cxx
  AliFlowTrack.cxx
  AliFlowCandidateTrack.cxx
  AliFlowTrackCuts.cxx
  AliAnalysisTaskScalarProduct.cxx
  AliAnalysisTaskSimpleSP.cxx
  AliAnalysisTaskMCEventPlane.cxx
  AliAnalysisTaskLYZEventPlane.cxx
  AliAnalysisTaskCRC.cxx
  AliAnalysisTaskCRCZDC.cxx
  AliAnalysisTaskQvec.cxx
  AliAnalysisTaskCumulants.cxx
  AliAnalysisTaskQCumulants.cxx
  AliAnalysisTaskLeeYangZeros.cxx
  AliAnalysisTaskFittingQDistribution.cxx
  AliAnalysisTaskFlowEvent.cxx
  AliAnalysisTaskFlowStrange.cxx
  AliAnalysisTaskFlowStrangee.cxx
  AliAnalysisTaskFlowEventforRP.cxx
  AliAnalysisTaskMixedHarmonics.cxx
  AliAnalysisTaskFlowModes.cxx
  AliAnalysisTaskFlowModesMC.cxx
  AliAnalysisTaskFlowModes_pPb.cxx
  AliAnalysisTaskNestedLoops.cxx
  AliAnalysisTaskQAflow.cxx
  AliAnalysisTaskPIDflowQA.cxx
  AliAnalysisTaskQAPmdflow.cxx
  AliFlowBayesianPID.cxx
  AliAnalysisTaskPhiFlow.cxx
  AliAnalysisTaskFilterFE.cxx
  AliAnalysisTaskVnV0.cxx
  AliFlowVZEROResults.cxx
  AliFlowVZEROQA.cxx
  AliAnalysisTaskFlowEPCascade.cxx
  AliAnalysisTaskMultiparticleCorrelations.cxx
  AliAnalysisTaskPIDconfig.cxx
  AliAnalysisTaskMultiparticleFemtoscopy.cxx
  AliAnalysisTaskForStudents.cxx
  AliAnalysisTaskZDCGainEq.cxx
  AliAnalysisTaskPiKpK0Lamba.cxx
  AliAnalysisTaskCmeEse.cxx
  AliAnalysisTaskCMEV0.cxx
  AliAnalysisTaskCMEV0PID.cxx
  AliAnalysisTaskCMEMC.cxx
  AliAnalysisTaskStudentsCM.cxx
  AliAnalysisTaskStudentsML.cxx
  AliAnalysisTaskTwoMultiCorrelations.cxx
  AliAnalysisTaskStudentsMW.cxx
  AliAnalysisTaskCVE.cxx
  AliAnalysisTaskNBodyFemtoscopy.cxx
  AliAnalysisTaskCMW.cxx
  AliAnalysisTaskCVEUNI.cxx
  AliAnalysisTaskCMWPU.cxx
  AliAnalysisTaskCMWPU2018.cxx
  AliAnalysisTaskCMWPU2018eqAch.cxx
  AliAnalysisTaskCMWPU2018eqAchautocorr.cxx
  AliAnalysisTaskCMWPU2018dca.cxx
  AliAnalysisTaskCMWPU2018eqAchdca.cxx
  AliAnalysisTaskCMWPU2018eqAchmerge.cxx
  AliAnalysisTaskCMWPU2018eqAchCL.cxx
  AliAnalysisTaskCMWPU2018eqAchCLdca.cxx
  AliAnalysisTaskCMWPU2018eqAchmergedca.cxx
  AliAnalysisTaskCVEUNIpt.cxx
  AliAnalysisTaskCMWPUeqAch.cxx
  AliAnalysisTaskCMWPUuncAch.cxx
  AliAnalysisTaskv2pt.cxx
  AliAnalysisTaskv2vscent.cxx
  AliAnalysisTaskMuPa.cxx
  AliAnalysisTaskAR.cxx
  AliAnalysisTaskEbECumulants.cxx
  AliAnalysisTaskGammaDeltaPID.cxx
  AliAnalysisTaskCMWESE.cxx
  AliAnalysisTaskEPCalib.cxx
  AliAnalysisTaskCMWESEsyst.cxx
  AliAnalysisTaskGammaDeltaPIDSaveQvec.cxx
  AliAnalysisTaskGammaDeltaPIDSaveQvecEvent.cxx
  AliAnalysisTaskCMXQA.cxx
  AliAnalysisTaskCMWPU2018eqAchCL.cxx
  AliAnalysisTaskLambdaProtonCVE.cxx
  AliAnalysisTaskGammaDeltaPIDSaveQvecSimple.cxx
  AliAnalysisTaskGammaDeltaPIDSaveQvecEventSimple.cxx
  AliAnalysisTaskCME2018NUA.cxx
  AliAnalysisTaskGammaNonIsotropicCorr.cxx
  AliAnalysisTaskGammaNonIsotropicCorrUsingWeights.cxx
  AliAnalysisTaskCVEPIDCME.cxx
  AliAnalysisTaskCVEVZEROCalib.cxx
  AliAnalysisTaskChargeV1.cxx
  AliAnalysisTaskGammaCaloV2.cxx
  AliAnalysisTaskPi0EtaV2.cxx
  AliAnalysisTaskGammaConvFlowV1.cxx
)

# Headers from sources
string(REPLACE ".cxx" ".h" HDRS "${SRCS}")

# Generate the dictionary
# It will create G_ARG1.cxx and G_ARG1.h / ARG1 = function first argument
get_directory_property(incdirs INCLUDE_DIRECTORIES)
generate_dictionary("${MODULE}" "${MODULE}LinkDef.h" "${HDRS}" "${incdirs}")

# Add a shared library
add_library_tested(${MODULE} SHARED  ${SRCS} G__${MODULE}.cxx)

# Generate the ROOT map
# Dependecies
set(LIBDEPS PWGflowBase PWGmuon ANALYSIS ANALYSISalice AOD ESD STEERBase PWGDevNanoAOD)
generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}LinkDef.h")

# Generate a PARfile target for this library
add_target_parfile(${MODULE} "${SRCS}" "${HDRS}" "${MODULE}LinkDef.h" "${LIBDEPS}")

# Linking the library
target_link_libraries(${MODULE} ${LIBDEPS})

# Public include folders that will be propagated to the dependecies
target_include_directories(${MODULE} PUBLIC ${incdirs})

# System dependent: Modify the way the library is build
if(${CMAKE_SYSTEM} MATCHES Darwin)
    set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(${CMAKE_SYSTEM} MATCHES Darwin)

# Installation
install(TARGETS ${MODULE}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)
install(FILES ${HDRS} DESTINATION include)
