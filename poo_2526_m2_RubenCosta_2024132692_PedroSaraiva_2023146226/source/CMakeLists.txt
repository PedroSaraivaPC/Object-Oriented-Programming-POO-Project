cmake_minimum_required(VERSION 3.20)

project(PraticalWork_POO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(PraticalWork_POO
        main.cpp
        Posicao.cpp
        Jardim.cpp
        Planta.cpp
        Cacto.cpp
        Roseira.cpp
        Ferramenta.cpp
        Settings.cpp
        Settings.h
        Posicao.h
        Jardim.h
        Planta.h
        Cacto.h
        Roseira.h
        Ferramenta.h
        Settings.cpp
        Regador.cpp
        Regador.h
        Adubo.cpp
        Adubo.h
        Tesoura.cpp
        Tesoura.h
        FerramentaZ.cpp
        FerramentaZ.h
        Ervadaninha.cpp
        Ervadaninha.h
        PlantaExotica.cpp
        PlantaExotica.h
        Jardineiro.cpp
        Jardineiro.h
        Simulador.cpp
        Simulador.h
)