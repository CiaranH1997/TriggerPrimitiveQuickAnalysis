add_executable(TPTimeCutFilter TPTimeCutFilter.cxx)
add_executable(OnlineTAGenerator OnlineTAGenerator.cxx)

target_link_libraries(TPTimeCutFilter TPAnaSrc ${ROOT_LIBRARIES}) 
target_link_libraries(OnlineTAGenerator TPAnaSrc ${ROOT_LIBRARIES}) 
