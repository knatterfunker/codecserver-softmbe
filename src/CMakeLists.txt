add_library(softmbe SHARED softmbedriver.cpp softmbedevice.cpp softmbesession.cpp blockingqueue.cpp)
target_link_libraries(softmbe codecserver mbe)
install(TARGETS softmbe
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/codecserver
)
