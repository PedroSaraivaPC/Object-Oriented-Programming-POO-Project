#ifndef PRATICALWORK_POO_REGADOR_H
#define PRATICALWORK_POO_REGADOR_H

#include "Ferramenta.h"

class Regador : public Ferramenta{ // Herança -> Regador é uma ferramenta
private:
    int aguaDisponivel; // quantidade de água existente no regador. Private porque apenas a classe Regador mexe

public:
    Regador(); // construtor da subclasse
    ~Regador() override; // Este destrutor é mesmo o destrutor virtual que substitui o da classe base (Ferramenta.h). Override para fzr mesmo override do destrutor virtual da base.

    char getRepresentacao() const override; // para aparecer como 'g' no jardim e em listagens
    void aplicar(Posicao *onde) override; // faz o efeito no solo (recebe como parametro um ponteiro para a posicao)
    bool estaGasta() const override; // diz se já acabou
};

#endif //PRATICALWORK_POO_REGADOR_H