#!/bin/bash

DATADIR=CERN_Data/ProtoDUNE_DM/TriggerPrimitiveAnalysisData

./build/app/OnlineTAGenerator \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 6 --u
