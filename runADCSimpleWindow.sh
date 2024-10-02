#!/bin/bash

#DATADIR=CERN_Data/ProtoDUNE_DM
DATADIR=CERN_Data/ProtoDUNE_DM/TriggerPrimitiveAnalysisData

# TPC 2 default 3ms window
./build/app/ADCSimpleWindowReplicator \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_low60th_fluxupdate_1010_Events.root \
  --tpe ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_low60th_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 2 --tm 20e3
# TPC 5 default 3ms window
#./build/app/ADCSimpleWindowReplicator \
#  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
#  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
#  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
#  --cut 12E6 --tpc 5 --tm 20e3 --u
# TPC 6 default 3ms window
#./build/app/ADCSimpleWindowReplicator \
#  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
#  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
#  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
#  --cut 12E6 --tpc 6 --tm 20e3 --u
