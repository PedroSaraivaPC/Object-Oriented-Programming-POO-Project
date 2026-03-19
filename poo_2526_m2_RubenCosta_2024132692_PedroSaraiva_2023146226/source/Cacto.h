#ifndef PRATICALWORK_POO_CACTO_H
#define PRATICALWORK_POO_CACTO_H

#include "Planta.h"

class Posicao;
class Jardim;

class Cacto : public Planta { // Herança -> Cacto é uma Planta
private:
    int instantesAguaExcessiva; // conta quantos instantes seguidos o solo esteve com água “a mais” (para a regra de morte por excesso de água)
    int instantesNutrientesBaixos; // conta quantos instantes seguidos o solo esteve com poucos nutrientes (para a regra de morte por falta de nutrientes)
    int nutrientesAbsorvidos; // guarda o total de nutrientes que este cacto absorveu ao longo da vida para depois “devolver ao solo quando morre” (regra do enunciado)

public:
    Cacto(); // construtor da subclasse
    ~Cacto() override; // Este destrutor é mesmo o destrutor virtual que substitui o da classe base (Planta.h). Override para fzr mesmo override do destrutor virtual da base.

    void passarInstante(Posicao* ondeEsta) override; // processa o comportamento da planta a cada instante que passa
    char getRepresentacao() const override; // retorna o char da planta (c-cacto|r-roseira|e-ervadaninha|x-plantaExotica)
};

#endif
