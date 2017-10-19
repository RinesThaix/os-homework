cmake_minimum_required(VERSION 3.8)
project(os)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c second/cp.c third/proc_graph.c sixth/matrix.c fifth/proc_thread_graph.c fourth/matrix_process.c seventh/tcp/client_tcp.c seventh/tcp/server_tcp.c seventh/udp/client_udp.c seventh/udp/server_udp.c)
add_executable(os ${SOURCE_FILES})