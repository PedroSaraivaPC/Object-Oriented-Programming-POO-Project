#ifndef PRATICALWORK_POO_ERVADANINHA_H
#define PRATICALWORK_POO_ERVADANINHA_H

#include "Planta.h"

class Posicao;

class Ervadaninha : public Planta { // Herança -> Ervadaninha é uma Planta
private:
    int instantesSemMultiplicar; // controlar o cooldown - conta quantos instantes passaram desde a última multiplicação

public:
    Ervadaninha(); // construtor da subclasse
    ~Ervadaninha() override; // Este destrutor é mesmo o destrutor virtual que substitui o da classe base (Planta.h). Override para fzr mesmo override do destrutor virtual da base.

    void passarInstante(Posicao* ondeEsta) override; // processa o comportamento da planta a cada instante que passa
    char getRepresentacao() const override;  // retorna o char da planta (c-cacto|r-roseira|e-ervadaninha|x-plantaExotica)
};

#endif