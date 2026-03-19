#ifndef PRATICALWORK_POO_TESOURA_H
#define PRATICALWORK_POO_TESOURA_H

#include "Ferramenta.h"

class Tesoura : public Ferramenta{ // Herança -> Tesoura é uma ferramenta
public:
    Tesoura(); // construtor da subclasse
    ~Tesoura() override; // Este destrutor é mesmo o destrutor virtual que substitui o da classe base (Ferramenta.h). Override para fzr mesmo override do destrutor virtual da base.

    char getRepresentacao() const override; // para aparecer como 't' no jardim e em listagens
    void aplicar(Posicao *onde) override;
    bool estaGasta() const override;

};

#endif //PRATICALWORK_POO_TESOURA_H