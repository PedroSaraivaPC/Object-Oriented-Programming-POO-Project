#ifndef PRATICALWORK_POO_ADUBO_H
#define PRATICALWORK_POO_ADUBO_H

#include "Ferramenta.h"

class Adubo : public Ferramenta{ // Herança -> Adubo é uma ferramenta
private:
    int aduboDisponivel; // quantidade de adubo existente. Private porque apenas a classe Adubo mexe

public:
    Adubo(); // construtor da subclasse
    ~Adubo() override; // Este destrutor é mesmo o destrutor virtual que substitui o da classe base (Ferramenta.h). Override para fzr mesmo override do destrutor virtual da base.

    char getRepresentacao() const override; // para aparecer como 'a' no jardim e em listagens
    void aplicar(Posicao *onde) override; // faz o efeito no solo (recebe como parametro um ponteiro para a posicao)
    bool estaGasta() const override; // diz se já acabou
};

#endif //PRATICALWORK_POO_ADUBO_H