#!/bin/bash

DATADIR=CERN_Data/ProtoDUNE_DM/TriggerPrimitiveAnalysisData

# TPC 2 default 3ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 2
# TPC 2 2.24ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 2 --tm 140e3 --u
# TPC 2 2ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 2 --tm 125e3 --u
# TPC 2 1.6ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 2 --tm 100e3 --u

# TPC 5 default 3ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 5 --u
# TPC 5 2.24ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 5 --tm 140e3 --u
# TPC 5 2ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 5 --tm 125e3 --u
# TPC 5 1.6ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 5 --tm 100e3 --u

# TPC 6 default 3ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 6 --u
 TPC 6 2.24ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 6 --tm 140e3 --u
# TPC 6 2ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 6 --tm 125e3 --u
# TPC 6 1.6ms window
./build/app/TPTimeCutFilter \
  --tpmu ${HOME}/${DATADIR}/tp_genie_numu_triggeranalysis_fluxupdate_1010_Events.root --tpe ${HOME}/${DATADIR}/tp_genie_nue_triggeranalysis_fluxupdate_1010_Events.root \
  --tpcf ${HOME}/${DATADIR}/tp_low60th_corsika_cosmic_triggeranalysis_1500_Events.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_10kEvents_fluxupdate_Oct24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_10kEvents_fluxupdate_Oct24.root \
  --cut 12E6 --tpc 6 --tm 100e3 --u
