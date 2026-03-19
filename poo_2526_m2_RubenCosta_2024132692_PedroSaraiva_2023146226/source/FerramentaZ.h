#ifndef PRATICALWORK_POO_FERRAMENTAZ_H
#define PRATICALWORK_POO_FERRAMENTAZ_H

#include "Ferramenta.h"

class FerramentaZ : public Ferramenta{ // Herança -> FerramentaZ é uma ferramenta
public:
    FerramentaZ(); // construtor da subclasse
    ~FerramentaZ() override; // Este destrutor é mesmo o destrutor virtual que substitui o da classe base (Ferramenta.h). Override para fzr mesmo override do destrutor virtual da base.

    char getRepresentacao() const override; // para aparecer como 'z' no jardim e em listagens
    void aplicar(Posicao *onde) override; // faz o efeito no solo (recebe como parametro um ponteiro para a posicao)
    bool estaGasta() const override; // diz se já acabou
};

#endif //PRATICALWORK_POO_FERRAMENTAZ_H