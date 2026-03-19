#include "FerramentaZ.h"
#include "Posicao.h"
#include "Planta.h"
#include <iostream>

FerramentaZ::FerramentaZ() : Ferramenta(){ // Chama automaticamente o construtor da classe base por defeito
} // nao é inicializado nada, tal como a tesoura (o uso é infinito)

FerramentaZ::~FerramentaZ() { // destrutor vaziu -> não faz nada mais se não destruir-se a si próprio
}

char FerramentaZ::getRepresentacao() const { // getRepresentacao retorna o 'z'
    return 'z';
}

void FerramentaZ::aplicar(Posicao *onde) { // faz o efeito no solo (recebe como parametro um ponteiro para a posicao)
    if (onde == nullptr) { // se a posicao nao existir
        return;
    }

    Planta* planta = onde->getPlanta(); // ponteiro que aponta para a planta que estã na posicao que se recebeu por parametro

    if (planta != nullptr && planta->isDead()) { // se a planta existir e se estiver morta
        std::cout << "Planta " << planta->getRepresentacao() << " eliminada do jardim com sucesso.\n";
        delete planta; // liberta a memória do objeto apontado por 'planta'. Se o destrutor de Planta for virtual, chama primeiro o destrutor da subclasse (e.g. Cacto) e depois o destrutor da classe base (Planta).
        onde->removerPlanta(); // liberta o ponteiro para a planta antiga e neste momento nao aponta para nada -> a posicao fica livre
    }
}

bool FerramentaZ::estaGasta() const { // diz se já acabou
    return false; // sempre falso pois nunca se gasta
}