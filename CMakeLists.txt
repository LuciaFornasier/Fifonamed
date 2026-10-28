cmake_minimum_required(VERSION 4.2)
project(e C)

set(CMAKE_C_STANDARD 11)

add_executable(e main.c)
add_executable(es1
        Es2.c)
add_executable(es3 es3.c)
add_executable(client.c client.c)
add_executable(server.c server.c)
add_executable(cliente2 cliente2.c)
add_executable(server2 server2.c)
add_executable(b b.c)
add_executable(a a.c)
