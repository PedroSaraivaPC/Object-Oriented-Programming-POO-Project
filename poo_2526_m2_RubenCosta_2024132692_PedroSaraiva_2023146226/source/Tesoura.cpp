#include "Tesoura.h"
#include "Posicao.h"
#include "Planta.h"
#include <iostream>

using namespace std;

Tesoura::Tesoura() : Ferramenta(){ // Chama automaticamente o construtor da classe base por defeito
} // nao é inicializado nada, tal como a ferramentaZ (o uso é infinito)

Tesoura::~Tesoura() { // destrutor vaziu -> não faz nada mais se não destruir-se a si próprio
}

char Tesoura::getRepresentacao() const { // getRepresentacao retorna o 't'
    return 't';
}

void Tesoura::aplicar(Posicao *onde) { // (recebe como parametro um ponteiro para a posicao)
    if (onde == nullptr) { // se a posicao nao existir
        return;
    }

    Planta* planta = onde->getPlanta(); // ponteiro que aponta para a planta que estã na posicao que se recebeu por parametro

    if (planta != nullptr && planta->getBeleza() == Beleza::FEIA) { // se a planta existir e se a beleza dela for do tipo "FEIA"
        delete planta; // liberta a memória do objeto apontado por 'planta'. Se o destrutor de Planta for virtual, chama primeiro o destrutor da subclasse (e.g. Cacto) e depois o destrutor da classe base (Planta).
        onde->removerPlanta(); // liberta o ponteiro para a planta antiga e neste momento nao aponta para nada -> a posicao fica livre
    }
}

bool Tesoura::estaGasta() const {
    return false; // sempre falso pois nunca se gasta
}
