set(SRCS
    poller.cc
    server.cc
    ../InitSocket/InitSocket.cc
    ../Acceptor.cc
    ServerWithPoller_test.cc
)

#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -std=c++11")
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)

add_executable(ServerWithPoller_test ${SRCS})