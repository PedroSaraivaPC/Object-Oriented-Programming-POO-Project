#include "Roseira.h"
#include "Posicao.h"
#include "Jardim.h"
#include "Settings.h"
#include <algorithm>
#include <vector>
#include <cstdlib>

Roseira::Roseira()
    : Planta(Settings::Roseira::inicial_agua, Settings::Roseira::inicial_nutrientes, Beleza::BONITA), // passa como parametro para o construtor da Planta.h os valores da 'aguaInicial', 'nutrientesInicial' e 'b'(beleza)
      aguaAbsorvidaVida(0),
      nutrientesAbsorvidosVida(0) {}

Roseira::~Roseira() {} // vaziu -> não faz nada mais se não destruir-se a si próprio

void Roseira::passarInstante(Posicao* ondeEsta) { // processa o comportamento da planta a cada instante que passa (recebendo um ponteiro para a sua posicao como parametro)
    idade++; // aumenta a idade da planta a cada instante

    // perde 4 unidades de água e 4 de nutrientes
    aguaAcumulada -= Settings::Roseira::perda_agua;
    nutrientesAcumulados -= Settings::Roseira::perda_nutrientes;

    // absorcao da agua
    int aguaAbs = std::min(ondeEsta->getAgua(), Settings::Roseira::absorcao_agua); // compara a agua atual do solo com o máximo que a Roseira pode absorver (5) e extrai o menor valor desses 2 (isto serve para quando o solo tem menos que 5 de agua, a Roseira fica com esse valor que sobra)
    aguaAcumulada += aguaAbs; // soma à agua atual da planta
    aguaAbsorvidaVida += aguaAbs; // soma à agua que absorveu a vida toda (regras do enunciado)
    ondeEsta->adicionarAgua(-aguaAbs); // retira essa agua ao solo

    // absorção dos nutrientes
    int nutrAbs = std::min(ondeEsta->getNutrientes(), Settings::Roseira::absorcao_nutrientes); // compara os nutrientes atuais do solo com o máximo que a Roseira pode absorver (8) e extrai o menor valor desses 2 (isto serve para quando o solo tem menos que 8 nutrientes, a Roseira fica com esse valor que sobra)
    nutrientesAcumulados += nutrAbs; // soma aos nutrientes atuais da planta
    nutrientesAbsorvidosVida += nutrAbs; // soma aos nutrientes que absorveu a vida toda (regras do enunciado)
    ondeEsta->adicionarNutrientes(-nutrAbs); // retira esses nutrientes que foram adicionados à planta do solo

    // morre se a agua atual chegar a 0, ou se os nutrientes atuais chegarem a 0, ou se os nutrientes atuais chegarem a 200
    if (aguaAcumulada < Settings::Roseira::morre_agua_menor || nutrientesAcumulados < Settings::Roseira::morre_nutrientes_menor || nutrientesAcumulados > Settings::Roseira::morre_nutrientes_maior) {
        ondeEsta->adicionarAgua(aguaAbsorvidaVida / 2); // ao morrer deixa no solo metade da água que absorveu durante a sua vida
        ondeEsta->adicionarNutrientes(nutrientesAbsorvidosVida / 2); // ao morrer deixa no solo metade dos nutrientes que absorveu durante a sua vida
        morrer();
        return;
    }

    // tambem morre se todas as posicoes vizinhas tiverem uma planta
    Jardim* j = ondeEsta->getJardim(); // vai buscar o jardim
    if (!j) return; // ve se existe

    int l = ondeEsta->getLinha();
    int c = ondeEsta->getColuna();

    //auto livres = j->getVizinhosLivres(ondeEsta->getLinha(), ondeEsta->getColuna());
    auto livres = j->getVizinhosLivres(l, c); // livres é um array com os ponteiros para as posicoes vizinhas livres

    // multiplicacao
    if (nutrientesAcumulados > Settings::Roseira::multiplica_nutrientes_maior && !livres.empty()) { // se nutrientes atuais >100 e se existir +1 vizinho livre
        Posicao* filhoPos = livres[rand() % livres.size()]; // escolhe aleatoriamente uma posicao livre para meter a Roseira filha

        Roseira* novaRos = new Roseira(); // gera o nova roseira com os valores iniciais do construtor

        int metadeAgua = aguaAcumulada / 2; // a nova planta fica com metade da água que a planta original tem
        novaRos->setAgua(metadeAgua); // altera a 'aguaAcumulada' de 25 para metade da original
        aguaAcumulada = metadeAgua; // a roseira original fica também com metade da água que tinha (igual á roseira filha)

        nutrientesAcumulados = Settings::Roseira::original_nutrientes; // a roseira original passa ainda a ter 100 unidades de nutrientes

        filhoPos->colocarPlanta(novaRos); // coloca a nova roseira na posicao anteriormente aleatoriamente escolhida

        livres = j->getVizinhosLivres(l, c); // recalcula o array livres depois da multiplicação (o estado mudou)
    }

    if (livres.empty()) { // se não houver vizinhos livres (todos os vizinhos têm plantas), morre (exceto se for jardim 1x1)
        if (j->getNumLinhas() > 1 || j->getNumColunas() > 1) {
            ondeEsta->adicionarAgua(aguaAbsorvidaVida / 2); // ao morrer deixa no solo metade da água que absorveu durante a sua vida
            ondeEsta->adicionarNutrientes(nutrientesAbsorvidosVida / 2); // ao morrer deixa no solo metade dos nutrientes que absorveu durante a sua vida
            morrer();
        }
    }
}

char Roseira::getRepresentacao() const { // retorna o char da planta (c-cacto|r-roseira|e-ervadaninha|x-plantaExotica)
    return 'r';
}