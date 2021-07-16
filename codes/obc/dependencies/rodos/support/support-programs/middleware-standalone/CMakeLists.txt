add_library(rodos-standalone-middleware
    src/independent/checksumes.cpp
    src/independent/gateway.cpp
    src/independent/genericIO.cpp
    src/independent/linkinterfaceudp.cpp
    src/independent/stream-bytesex.cpp
    src/independent/udp.cpp
    src/linux/hw_udp.cpp)
target_include_directories(rodos-standalone-middleware PUBLIC api src/linux)
