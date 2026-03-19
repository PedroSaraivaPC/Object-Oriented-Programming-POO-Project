#include "Ervadaninha.h"
#include "Settings.h"
#include "Posicao.h"
#include "Jardim.h"
#include <algorithm>
#include <vector>
#include <cstdlib>

Ervadaninha::Ervadaninha()
    : Planta(Settings::ErvaDaninha::inicial_agua, Settings::ErvaDaninha::inicial_nutrientes, Beleza::FEIA), // passa como parametro para o construtor da Planta.h os valores da 'aguaInicial', 'nutrientesInicial' e 'b'(beleza)
      instantesSemMultiplicar(0) {}

Ervadaninha::~Ervadaninha() {} // vaziu -> não faz nada mais se não destruir-se a si próprio

void Ervadaninha::passarInstante(Posicao* ondeEsta) { // processa o comportamento da planta a cada instante que passa (recebendo um ponteiro para a sua posicao como parametro)
    idade++; // aumenta a idade da planta a cada instante
    instantesSemMultiplicar++; // cooldown de multiplicação

    // absorção da agua (1 água por instante)
    int aguaAbs = std::min(ondeEsta->getAgua(), Settings::ErvaDaninha::absorcao_agua); // compara a agua atual do solo com o máximo que a ErvaDaninha pode absorver (1) e extrai o menor valor desses 2 (isto serve para quando o solo tem menos que 1 de agua, a ErvaDaninha fica com esse valor que sobra(neste caso 0))
    aguaAcumulada += aguaAbs; // adiciona essa agua atual/acumulada da ErvaDaninha
    ondeEsta->adicionarAgua(-aguaAbs); // retira essa agua ao solo

    // absorção dos nutrientes (1 nutriente por instante)
    int nutrAbs = std::min(ondeEsta->getNutrientes(), Settings::ErvaDaninha::absorcao_nutrientes); // exatamente a mesma coisa que a agua
    nutrientesAcumulados += nutrAbs;
    ondeEsta->adicionarNutrientes(-nutrAbs);

    // morte por velhice (60 instantes)
    if (idade >= Settings::ErvaDaninha::morre_instantes) { // se a idade for igual ou ultrapassar de 60 a planta morre
        morrer();
        return;
    }

    // multiplicação invasiva (mata o que estiver na posicao nova)
    if (nutrientesAcumulados > Settings::ErvaDaninha::multiplica_nutrientes_maior && instantesSemMultiplicar >= Settings::ErvaDaninha::multiplica_instantes) // se tiver nutrientes acumulados >30 e se passaram 5 instantes desde a última vez
    {
        Jardim* j = ondeEsta->getJardim(); // pega no jardim
        if (!j) return; // ve se ele existe

        std::vector<Posicao*> vizinhos; // array de ponteiros para posicoes vizinhas à posicao atual (Livres ou Ocupadas)
        int l = ondeEsta->getLinha(); // linha da posicao atual
        int c = ondeEsta->getColuna(); // coluna da posicao atual

        for(int dl = -1; dl <= 1; dl++) { // ve as 3 linhas
            for(int dc = -1; dc <= 1; dc++) { // ve as 3 colunas
                if(dl == 0 && dc == 0) continue; // Ignora a própria posição (o centro)

                Posicao* p = j->getPosicao(l + dl, c + dc);
                if(p != nullptr) { // Se a posição existe dentro do jardim
                    vizinhos.push_back(p); // adiciona ao array 'vizinhos' as posicoes validas
                }
            }
        }

        if (!vizinhos.empty()) { // se nao houver nenhuma posicao válida(livre ou ocupada) sai e a ervadaninha nao se multiplica
            Posicao* alvo = vizinhos[rand() % vizinhos.size()]; // escolhe aleatoriamente uma posicao para meter a ervadaninha

            if (alvo->getPlanta()) { // se houver planta na posicao escolhida, mata-a
                delete alvo->getPlanta(); // liberta memória da planta antiga (chama primeiro o destrutor da subclasse (Ervadaninha) e depois o destrutor da classe base (Planta))
                alvo->removerPlanta(); // liberta o ponteiro para a planta antiga e neste momento nao aponta para nada -> a posicao fica livre
            }

            alvo->colocarPlanta(new Ervadaninha()); // coloca a nova ervadaninha, esta nasce com valores base (definidos no construtor)

            instantesSemMultiplicar = 0; // reseta o contador do cooldown
        }
    }
}

char Ervadaninha::getRepresentacao() const { // retorna o char da planta (c-cacto|r-roseira|e-ervadaninha|x-plantaExotica)
    return 'e';
}