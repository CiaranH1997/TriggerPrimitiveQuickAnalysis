#!/bin/bash

DATADIR=CERN_Data/ProtoDUNE_DM

./build/app/OnlineTAGenerator \
  --tpmu ${HOME}/${DATADIR}/tp_numu_600ev_combined_Sep24.root --tpe ${HOME}/${DATADIR}/tp_nue_600ev_combined_Sep24.root \
  --tpcf ${HOME}/${DATADIR}/tp_cosmic_analysis_totalprod_400ev_hists_Aug24.root \
  --anmu ${HOME}/${DATADIR}/genie_numu_analyze_Sep24.root --ane ${HOME}/${DATADIR}/genie_nue_analyze_Sep24.root \
  --cut 12e6 --tpc 5
