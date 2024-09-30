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

1. `TPTimeCutFilter.cxx`: Generates a series of graphs and histograms describing a process where first an ADC Integral Sum threshold is applied to readout windows in particular NP04 TPC and following this an offline timing filter is applied that rejects events that are not closely aligned with the cathode.

2. `OnlineTAGenerator.cxx`: Approximates the process of TA generation by feeding the TPs to a function that applies the ADC Integral Sum threshold to TPs in a readout window in a particular NP04 TPC. A TTree is then saved that holds all the "TA" information. Note that real TAs are a little bit different than what is produced here. There are also analysis trees produced that relate the true neutrino information to the ADC Integral Sum.

The applications are C++ executable that have command line inputs that describe the data files, the TPC ID and ADC Integral Sum threshold of choice. There are two bash scripts provided that make running these scripts a bit more convenient: `runTimeFilterScript.sh` and `runOnlineTAGenerator.sh`.
