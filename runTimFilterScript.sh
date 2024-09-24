#!/bin/bash

DATADIR=CERN_Data/ProtoDUNE_DM

./build/app/TPTimeCutFilter \
  --tpnf ${HOME}/${DATADIR}/tp_numu_600ev_combined_Sep24.root ${HOME}/${DATADIR}/tp_nue_600ev_combined_Sep24.root \
  --tpcf ${HOME}/${DATADIR}/tp_cosmic_analysis_totalprod_400ev_hists_Aug24.root \
  --annf ${HOME}/${DATADIR}/genie_numu_analyze_Sep24.root ${HOME}/${DATADIR}/genie_nue_analyze_Sep24.root
