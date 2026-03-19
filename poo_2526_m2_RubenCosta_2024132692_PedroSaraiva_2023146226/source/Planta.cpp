#include "Planta.h"
#include <iostream>

Planta::Planta(int aguaInicial, int nutrientesInicial, Beleza b)
    : aguaAcumulada(aguaInicial),
      nutrientesAcumulados(nutrientesInicial),
      aguaTotalAbsorvida(0),
      nutrientesTotaisAbsorvidos(0),
      beleza(b),
      morta(false),
      idade(0) {
}

Planta::~Planta() {} // vaziu -> não faz nada mais se não destruir-se a si próprio

// getters (enviam/retornam a informacao desejada)
char Planta::getRepresentacaoFinal() const {
    char c = getRepresentacao();
    if (morta) {
        return toupper(c);  // minuscula para maiuscula -> planta morta e pronta a ser utilizada pela FerramentaZ
    }
    return c;
}

bool Planta::isDead() const {
    return morta;
}

Beleza Planta::getBeleza() const {
    return beleza;
}

int Planta::getAgua() const {
    return aguaAcumulada;
}

int Planta::getNutrientes() const {
    return nutrientesAcumulados;
}

int Planta::getIdade() const {
    return idade;
}

// setters (altera os valores de 'aguaAcumulada' e 'nutrientesAcumulados' para o que está a vir como parametro)
void Planta::setAgua(int a) {
    aguaAcumulada = a;
}

void Planta::setNutrientes(int n) {
    nutrientesAcumulados = n;
}

void Planta::morrer() { // altera o 'morta' para true quando a planta morre
    morta = true;
}
