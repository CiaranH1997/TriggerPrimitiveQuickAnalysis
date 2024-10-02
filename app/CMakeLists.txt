add_executable(TPTimeCutFilter TPTimeCutFilter.cxx)
add_executable(OnlineTAGenerator OnlineTAGenerator.cxx)
add_executable(ADCSimpleWindowReplicator ADCSimpleWindowReplicator.cxx)

target_link_libraries(TPTimeCutFilter TPAnaSrc ${ROOT_LIBRARIES}) 
target_link_libraries(OnlineTAGenerator TPAnaSrc ${ROOT_LIBRARIES}) 
target_link_libraries(ADCSimpleWindowReplicator TPAnaSrc ${ROOT_LIBRARIES}) 
