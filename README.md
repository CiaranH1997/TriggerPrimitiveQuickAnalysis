# TriggerPrimitiveQuickAnalysis
A C++ framework to quickly analyse NP04 trigger primitives from simulation or data. Test different cuts on trigger information that could be used in the online trigger or offline data filter.

## Compiling

This is a CMake C++ project, so you will need a reasonably new version of CMake and gcc to compile this program. The minimum required CMake is 3.16. You will also require ROOT C++ in your environment. To compile do the following:

```
git clone https://github.com/CiaranH1997/TriggerPrimitiveQuickAnalysis.git

mkdir build

cmake ../

make
```

Hopefully that is quite easy!

## The Code

The code is structured so that there is a general `Event` object and different types of events like neutrino or cosmic events are derived from this base class. It would be relatively simple to extend this structure to have different BSM `Event` objects as well. The neutrino event object (`NeutrinoEvent`) contains true information about the neutrino, as well as its trigger primitive (TP) information. 

The neutrino events are always combined with a random cosmic background that comes from teh simulation.

## Data

The code assumes you have two separate neutrino files -- one for $\nu_{e}$ events and one for $\nu_{\mu}$ events -- as well as a cosmic file. I have testes this with a cosmic MC files, but I presume this works fine with cosmic data as well. The files are TP analysis files that contain the `TPTree` TTree describing the TPs in a readout window.

## Running the Code

There are currently just two application that the user can run. 

1. `TPTimeCutFilter.cxx`: Generates a series of graphs and histograms describing a process where first an ADC Integral Sum threshold is applied to readout windows in particular NP04 TPC and following this an offline timing filter is applied that rejects events that are not closely aligned with the cathode. The user inputs the ADC Integral Sum threshold (`--cut` option) and time window to be considered (`--tm`). The time window in this case should be some fraction of the total 3~ms drift window.

2. `OnlineTAGenerator.cxx`: Approximates the process of TA generation by feeding the TPs to a function that applies the ADC Integral Sum threshold to TPs in a readout window in a particular NP04 TPC. A TTree is then saved that holds all the "TA" information. Note that real TAs are a little bit different than what is produced here. There are also analysis trees produced that relate the true neutrino information to the ADC Integral Sum. The user inputs the ADC Integral Sum threshold (`--cut` option) and time window to be considered (`--tm`). The time window in this case should be some fraction of the total 3~ms drift window.

3. `ADCSimpleWindowReplicator.cxx`: The trigger algorithm ADCSimpleWindow is an online trigger algorithm for NP04 that essentialy applies the time filter algorithm to the online stream of TPs. This executable simulates the online application of ADCSimpleWindow to the TPs held in the input file and provides efficiencies, histograms and TTrees analysing the output of this algorithm. The user inputs the ADC Integral Sum threshold (`--cut` option) and time window to be considered (`--tm`), which this time is the small time window (of the order 20k ticks) associated with this algorithm.

The applications are C++ executable that have command line inputs that describe the data files, the TPC ID and ADC Integral Sum threshold of choice. There are bash scripts provided that make running these scripts a bit more convenient: `runTimeFilterScript.sh`, `runOnlineTAGenerator.sh` and `runADCSimpleWindow.sh`.
