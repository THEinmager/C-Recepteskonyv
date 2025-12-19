cmake_minimum_required(VERSION 4.0)
project(recepteskonyv C)

set(CMAKE_C_STANDARD 99)

add_executable(recepteskonyv main.c
        osszetevok.c
        osszetevok.h
        receptek.c
        receptek.h
        fajlkezelo.c
        fajlkezelo.h
        menu.c
        menu.h
        adatszerkezet.h
)
