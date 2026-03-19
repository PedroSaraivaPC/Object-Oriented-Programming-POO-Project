#include "Adubo.h"
#include "Posicao.h"
#include "Settings.h"
#include <algorithm>
#include <iostream>

using namespace std;

Adubo::Adubo() : Ferramenta() { // Chama automaticamente o construtor da classe base por defeito
    aduboDisponivel = Settings::Adubo::capacidade; // Inicializa o adubo “cheio”, com a capacidade definida em Settings.
}

Adubo::~Adubo() { // destrutor vaziu -> não faz nada mais se não destruir-se a si próprio
}

char Adubo::getRepresentacao() const { // getRepresentacao retorna o 'a'
    return 'a';
}

void Adubo::aplicar(Posicao *onde) { // (recebe como parametro um ponteiro para a posicao)
    if (onde == nullptr || estaGasta()) { // se a posicao nao existir ou o adubo estiver gasto
        return;
    }

    int dose = Settings::Adubo::dose; // extrai a quant de dose definida nas settings
    if (aduboDisponivel >= dose) { // cheka se tem mais adubo do que a quantidade da dose a colocar no solo, se sim mete
        onde->adicionarNutrientes(dose);
        aduboDisponivel -= dose;
    } else { // se nao, mete apenas o que tem disponivel
        onde->adicionarNutrientes(aduboDisponivel);
        aduboDisponivel = 0;
    }
}

bool Adubo::estaGasta() const {
    return aduboDisponivel <= 0; // se (aduboDisponivel <= 0) a funcao retorna true, se nao, retorna false
}
