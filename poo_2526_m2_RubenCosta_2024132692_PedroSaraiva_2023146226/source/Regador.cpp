#include "Regador.h"
#include "Posicao.h"
#include "Settings.h"
#include <algorithm>
#include <iostream>

using namespace std;

Regador::Regador() : Ferramenta() { // Chama automaticamente o construtor da classe base por defeito
    aguaDisponivel = Settings::Regador::capacidade; // Inicializa o regador “cheio”, com a capacidade definida em Settings.
}

Regador::~Regador() { // destrutor vaziu -> não faz nada mais se não destruir-se a si próprio
}

char Regador::getRepresentacao() const { // getRepresentacao retorna o 'g'
    return 'g';
}

void Regador::aplicar(Posicao *onde) { // (recebe como parametro um ponteiro para a posicao)
    if (onde == nullptr || estaGasta()) { // se a posicao nao existir ou o regador estiver vaziu
        return;
    }

    int dose = Settings::Regador::dose; // extrai a quant de dose definida nas settings
    if (aguaDisponivel >= dose) { // cheka se tem mais água no regador do que a quantidade da dose a colocar no solo, se sim mete
        onde->adicionarAgua(dose);
        aguaDisponivel -= dose;
    } else { // se nao, mete apenas o que tem disponivel
        onde->adicionarAgua(aguaDisponivel);
        aguaDisponivel = 0;
    }
}

bool Regador::estaGasta() const {
    return aguaDisponivel <= 0; // se (aguaDisponivel <= 0) a funcao retorna true, se nao, retorna false
}