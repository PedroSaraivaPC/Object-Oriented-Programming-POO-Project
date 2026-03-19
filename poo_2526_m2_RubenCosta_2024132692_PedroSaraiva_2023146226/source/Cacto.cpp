#include "Cacto.h"
#include "Posicao.h"
#include "Settings.h"
#include "Jardim.h"
#include <algorithm>

Cacto::Cacto()
    : Planta(0, 0, Beleza::NEUTRA), // passa como parametro para o construtor da Planta.h os valores da 'aguaInicial', 'nutrientesInicial' e 'b'(beleza)
      instantesAguaExcessiva(0),
      instantesNutrientesBaixos(0),
      nutrientesAbsorvidos(0) {}

Cacto::~Cacto() {} // vaziu -> não faz nada mais se não destruir-se a si próprio

void Cacto::passarInstante(Posicao* ondeEsta) { // processa o comportamento da planta a cada instante que passa (recebendo um ponteiro para a sua posicao como parametro)
    idade++; // aumenta a idade da planta a cada instante

    // absorção da agua
    int aguaSolo = ondeEsta->getAgua(); // vai buscar a agua atual do solo
    int aguaAbs = aguaSolo * Settings::Cacto::absorcao_agua_percentagem / 100; // calcula os 25% de água no solo
    aguaAcumulada += aguaAbs; // adiciona esses 25% à agua atual/acumulada do cacto
    ondeEsta->adicionarAgua(-aguaAbs); // retira esses 25% ao solo

    // absorção dos nutrientes
    int nutrSolo = ondeEsta->getNutrientes(); // vai buscar os nutrientes atuais do solo
    int nutrAbs = std::min(nutrSolo, Settings::Cacto::absorcao_nutrientes); // compara os nutrientes atuais do solo com o máximo que o cacto pode absorver e extrai o menor valor desses 2 (isto serve para quando o solo tem menos que 5 nutrientes, o cacto fica com esse valor que sobra)
    nutrientesAcumulados += nutrAbs; // soma aos nutrientes atuais da planta
    nutrientesAbsorvidos += nutrAbs; // soma aos nutrientes que absorveu a vida toda (regra do enunciado)
    ondeEsta->adicionarNutrientes(-nutrAbs); // retira esses nutrientes que foram adicionados à planta do solo

    // morte por água a mais no solo durante N instantes seguidos
    if (ondeEsta->getAgua() > Settings::Cacto::morre_agua_solo_maior) { // cheka se a agua atual é >100
        instantesAguaExcessiva++; // incrementa o instante onde a agua está a cima de 100
        if (instantesAguaExcessiva >= Settings::Cacto::morre_agua_solo_instantes) { // se chegar aos 3 instantes seguidos a planta morre e adiciona ao solo os nutrientes que o cacto absorveu a vida toda (regra do enunciado)
            ondeEsta->adicionarNutrientes(nutrientesAbsorvidos);
            morrer();
            return;
        }
    } else instantesAguaExcessiva = 0;

    // morte por nutrientes baixos no solo durante N instantes seguidos
    if (ondeEsta->getNutrientes() < Settings::Cacto::morre_nutrientes_solo_menor) { // cheka se os nutrientes no solo forem <1
        instantesNutrientesBaixos++; // incrementa o instante onde os nutrientes sao 0
        if (instantesNutrientesBaixos > Settings::Cacto::morre_nutrientes_solo_instantes) { // se passar dos 3 instantes seguidos a planta morre e adiciona ao solo os nutrientes que o cacto absorveu a vida toda (regra do enunciado)
            ondeEsta->adicionarNutrientes(nutrientesAbsorvidos);
            morrer();
            return;
        }
    } else instantesNutrientesBaixos = 0;

    // multiplicação (cria um novo cacto num vizinho vazio)
    if (nutrientesAcumulados > Settings::Cacto::multiplica_nutrientes_maior && aguaAcumulada > Settings::Cacto::multiplica_agua_maior) // se a quantidade acumulada de nutrientes no cato for >100 e a água acumulada >50
    {
        Jardim* j = ondeEsta->getJardim(); // pega no jardim
        if (!j) return; // ve se ele existe

        auto livres = j->getVizinhosLivres(ondeEsta->getLinha(), ondeEsta->getColuna()); // livres é um array com os ponteiros para as posicoes vizinhas livres
        if (livres.empty()) return; // se nao houver nenhuma posicao vazia sai e o cacto nao se multiplica

        Posicao* destino = livres[rand() % livres.size()]; // escolhe aleatoriamente uma posicao livre para meter o cacto filho

        // Divida em metade a quantidade de agua e nutrientes para o cacto antigo e o novo cacto
        int aguaFilho = aguaAcumulada / 2;
        int nutrFilho = nutrientesAcumulados / 2;
        aguaAcumulada -= aguaFilho;
        nutrientesAcumulados -= nutrFilho;

        Cacto* filho = new Cacto(); // gera o novo cacto com os valores iniciais do construtor
        filho->setAgua(aguaFilho); // altera a 'aguaAcumulada'
        filho->setNutrientes(nutrFilho); // altera os 'nutrientesAcumulados'

        destino->colocarPlanta(filho); // coloca o novo cacto na posicao anteriormente aleatoriamente escolhida
    }
}

char Cacto::getRepresentacao() const { // retorna o char da planta (c-cacto|r-roseira|e-ervadaninha|x-plantaExotica)
    return 'c';
}
