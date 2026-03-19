#include "Ferramenta.h"

int Ferramenta::proximoNumeroSerie = 1; // inicializa o contador a 1

Ferramenta::Ferramenta() {
    numeroSerie = proximoNumeroSerie++; // da automaticamente um nº de serie à ferramente e incrementa logo o 'proximoNumeroSerie'
}

Ferramenta::~Ferramenta() { // vaziu -> não faz nada mais se não destruir-se a si próprio
}

int Ferramenta::getNumeroSerie() const { // getter do nº série da ferramenta
    return numeroSerie;
}

