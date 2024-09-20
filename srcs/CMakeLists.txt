add_library(TPAnaSrc TP.cxx Event.cxx NeutrinoEvent.cxx CosmicEvent.cxx)

target_include_directories(TPAnaSrc PUBLIC ${CMAKE_SOURCE_DIR}/inc)
