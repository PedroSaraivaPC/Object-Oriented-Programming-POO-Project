#ifndef PRATICALWORK_POO_ROSEIRA_H
#define PRATICALWORK_POO_ROSEIRA_H

#include "Planta.h"

class Posicao;

class Roseira : public Planta { // Herança -> Roseira é uma Planta
private:
    int aguaAbsorvidaVida; // guarda o total de agua que esta Roseira absorveu ao longo da vida para as regras
    int nutrientesAbsorvidosVida; // guarda o total de nutrientes que esta Roseira absorveu ao longo da vida para as regras

public:
    Roseira(); // construtor da subclasse
    ~Roseira() override; // Este destrutor é mesmo o destrutor virtual que substitui o da classe base (Planta.h). Override para fzr mesmo override do destrutor virtual da base.

    void passarInstante(Posicao* ondeEsta) override; // processa o comportamento da planta a cada instante que passa
    char getRepresentacao() const override; // retorna o char da planta (c-cacto|r-roseira|e-ervadaninha|x-plantaExotica)
};

#endif
