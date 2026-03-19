#ifndef PRATICALWORK_POO_PLANTAEXOTICA_H
#define PRATICALWORK_POO_PLANTAEXOTICA_H

#include "Planta.h"

class Posicao;

class PlantaExotica : public Planta { // Herança -> PlantaExotica é uma Planta
private:
    int instantesSemAgua; // conta os instantes seguidos que ela esteve sem conseguir absorver água do solo (para uma da regra)

public:
    PlantaExotica(); // construtor da subclasse
    ~PlantaExotica() override; // Este destrutor é mesmo o destrutor virtual que substitui o da classe base (Planta.h). Override para fzr mesmo override do destrutor virtual da base.

    void passarInstante(Posicao* ondeEsta) override; // processa o comportamento da planta a cada instante que passa
    char getRepresentacao() const override; // retorna o char da planta (c-cacto|r-roseira|e-ervadaninha|x-plantaExotica)
};

#endif
