#include "Jardineiro.h"
#include "Jardim.h"
#include "Posicao.h"
#include "Cacto.h"
#include "Roseira.h"
#include "Ervadaninha.h"
#include "PlantaExotica.h"
#include "Ferramenta.h"
#include "Settings.h"
#include <iostream>

using namespace std;

Jardineiro::Jardineiro()
    : linhaAtual(-1),
      colunaAtual(-1),
      estaNoJardim(false),
      ferramentaNaMao(nullptr),
      movimentosRealizados(0),
      entradasRealizadas(0),
      saidasRealizadas(0),
      plantacoesRealizadas(0),
      colheitasRealizadas(0),
      apanheiFerramentaNesteMovimento(false){}

Jardineiro::~Jardineiro() { // antes de se destruir:
    for (Ferramenta* f : ferramentas) // elimina uma por uma -> cada ferramenta que está na mochila
        delete f;

    if (ferramentaNaMao) // se tiver ferramenta na mao, elimina-a
        delete ferramentaNaMao;
}

bool Jardineiro::moverEsquerda(Jardim* jardim) {

    // Não está no jardim
    if (!estaNoJardim) {
        cout << "ERRO: O jardineiro nao esta no jardim.\n";
        return false;
    }

    // Limite de movimentos
    if (movimentosRealizados >= Settings::Jardineiro::max_movimentos) {
        cout << "ERRO: Limite de movimentos por turno atingido.\n";
        return false;
    }

    // Fora dos limites
    if (colunaAtual <= 0) {
        cout << "ERRO: Movimento fora dos limites do jardim.\n";
        return false;
    }

    // Movimento válido
    colunaAtual--; // passa a ser a coluna do lado esquerdo
    movimentosRealizados++; // incrementa os movimentosRealizados
    apanharFerramentaSeExistir(jardim); // e apanha ferramentas se existirem nesta nova posicao
    return true;
}


bool Jardineiro::moverDireita(Jardim* jardim) {

    // Não está no jardim
    if (!estaNoJardim) {
        cout << "ERRO: O jardineiro nao esta no jardim.\n";
        return false;
    }

    // Limite de movimentos
    if (movimentosRealizados >= Settings::Jardineiro::max_movimentos) {
        cout << "ERRO: Limite de movimentos por turno atingido.\n";
        return false;
    }

    // Fora dos limites do jardim
    if (colunaAtual >= jardim->getNumColunas() - 1) {
        cout << "ERRO: Movimento fora dos limites do jardim.\n";
        return false;
    }

    // Movimento válido
    colunaAtual++; // passa a ser a coluna do lado direito
    movimentosRealizados++; // incrementa os movimentosRealizados
    apanharFerramentaSeExistir(jardim); // e apanha ferramentas se existirem nesta nova posicao
    return true;
}


bool Jardineiro::moverCima(Jardim* jardim) {

    // Não está no jardim
    if (!estaNoJardim) {
        cout << "ERRO: O jardineiro nao esta no jardim.\n";
        return false;
    }

    // Limite de movimentos
    if (movimentosRealizados >= Settings::Jardineiro::max_movimentos) {
        cout << "ERRO: Limite de movimentos por turno atingido.\n";
        return false;
    }

    // Fora dos limites
    if (linhaAtual <= 0) {
        cout << "ERRO: Movimento fora dos limites do jardim.\n";
        return false;
    }

    // Movimento válido
    linhaAtual--; // passa a ser a coluna de cima
    movimentosRealizados++; // incrementa os movimentosRealizados
    apanharFerramentaSeExistir(jardim); // e apanha ferramentas se existirem nesta nova posicao
    return true;
}


bool Jardineiro::moverBaixo(Jardim* jardim) {

    // Não está no jardim
    if (!estaNoJardim) {
        cout << "ERRO: O jardineiro nao esta no jardim.\n";
        return false;
    }

    // Limite de movimentos por turno
    if (movimentosRealizados >= Settings::Jardineiro::max_movimentos) {
        cout << "ERRO: Limite de movimentos por turno atingido.\n";
        return false;
    }

    // Fora dos limites do jardim
    if (linhaAtual >= jardim->getNumLinhas() - 1) {
        cout << "ERRO: Movimento fora dos limites do jardim.\n";
        return false;
    }

    // Movimento válido
    linhaAtual++; // passa a ser a coluna de baixo
    movimentosRealizados++; // incrementa os movimentosRealizados
    apanharFerramentaSeExistir(jardim); // e apanha ferramentas se existirem nesta nova posicao
    return true;
}


bool Jardineiro::entrar(Jardim* jardim, int linha, int coluna) { // coloca o jardineiro na posicao [linha, coluna]
    if (entradasRealizadas >= Settings::Jardineiro::max_entradas_saidas) // apenas pode entrar 1 vez por cada instante
        return false;

    if (linha < 0 || linha >= jardim->getNumLinhas() ||
        coluna < 0 || coluna >= jardim->getNumColunas()) // verifica se onde ele quer entrar esta dentro dos limites
        return false;

    // muda os valores da linha e coluna para a nova posicao e se está no jardim ou nao (agora a true)
    linhaAtual = linha;
    colunaAtual = coluna;
    estaNoJardim = true;

    cout << "Jardineiro entrou em " << char('a' + linhaAtual) << char('a' + colunaAtual) <<"!\n"; // passa os indices (linha e coluna) para char(coordenadas)

    entradasRealizadas++; // incrementa as entradas que ele ja fez neste instante

    apanharFerramentaSeExistir(jardim); // apanha ferramentas se existirem nesta nova posicao
    return true;
}

bool Jardineiro::sair() { // jardineiro sai do jardim
    if (!estaNoJardim) // se ele já nao estiver no jardim, sai da funcao
        return false;

    if (saidasRealizadas >= Settings::Jardineiro::max_entradas_saidas) // apenas pode sair 1 vez por cada instante
        return false;

    // muda os valores da linha e coluna para -1 e se está no jardim ou nao (agora a false)
    linhaAtual = -1;
    colunaAtual = -1;
    estaNoJardim = false;

    saidasRealizadas++; // incrementa as saidas que ele ja fez neste instante

    return true;
}


void Jardineiro::adicionarFerramenta(Ferramenta* f) { // adiciona ferramentas na mochila
    if (f) ferramentas.push_back(f); // adiciona ao array de ponteiros para ferramentas que estao na mochila a ferramenta que está a ser recebida por parametro (isto se ela existir)
}

bool Jardineiro::pegarFerramenta(int numeroSerie) { // pega na ferramenta que está na mochila com o n.º de serie recebido por parametro
    for (size_t i = 0; i < ferramentas.size(); i++) { // Percorre todas as ferramentas da mochila. Usa-se size_t, pq 'ferramentas.size()' devolve um size_t, que é um inteiro sem sinal (unsigned - nao tem negativos)
        if (ferramentas[i]->getNumeroSerie() == numeroSerie) { // procura a ferramenta com o nº de série igual ao que está a ser recebeido por parâmetro
            if (ferramentaNaMao){ // se já tem alguma ferramenta na mao
                ferramentas.push_back(ferramentaNaMao); // devolve essa ferramenta que já estava na mão para a mochila
            }
            ferramentaNaMao = ferramentas[i]; // e passa para a mão a ferramenta que ele quer
            ferramentas.erase(ferramentas.begin() + i); // remove da mochila a ferramenta que foi para a mão (na posicao 0+i)
            return true;
        }
    }
    return false;
}

bool Jardineiro::largarFerramenta() { // tira a ferramenta na mao e guarda-a na mochila
    if (!ferramentaNaMao) // se nao tiver nada na mao, sai da funcao
        return false;

    ferramentas.push_back(ferramentaNaMao); // adiciona-se ao array da mochila a ferramenta que estava na mao
    ferramentaNaMao = nullptr; // e na mao fica 'nullptr' (sem ferramenta nenhuma)
    return true;
}

bool Jardineiro::plantar(Jardim* jardim, int linha, int coluna, char tipo) { // planta uma planta do tipo 'tipo' na posicao [linha, coluna]
    if (plantacoesRealizadas >= Settings::Jardineiro::max_plantacoes) // verifica se ja plantou +2 vezes neste instante, se sim, sai da funcao
        return false;

    Posicao* pos = jardim->getPosicao(linha, coluna); // 'pos' fica com um ponteiro para a posicao da [linha, coluna] recebidos por parametro
    if (!pos || pos->getPlanta()) // se essa posicao nao existir OU se já houver alguma planta lá, sai da funcao
        return false;

    Planta* nova = nullptr; // cria uma nova planta nula
    switch (tipo) { // dependendo do tipo recebido por parametro, cria a planta correspondente (c-cacto|r-roseira|e-ervadaninha|x-plantaExotica)
        case 'c': nova = new Cacto(); break;
        case 'r': nova = new Roseira(); break;
        case 'e': nova = new Ervadaninha(); break;
        case 'x': nova = new PlantaExotica(); break;
        default: return false;
    }

    pos->colocarPlanta(nova); // coloca nessa posicao a planta nova
    plantacoesRealizadas++; // e incrementa as plantacoes feitas neste instante
    return true;
}

bool Jardineiro::colher(Jardim* jardim, int linha, int coluna) { // colhe uma planta que esteja na posicao [linha, coluna]
    if (colheitasRealizadas >= Settings::Jardineiro::max_colheitas) // verifica se ja colheu +5 vezes neste instante, se sim, sai da funcao
        return false;

    Posicao* pos = jardim->getPosicao(linha, coluna); // 'pos' fica com um ponteiro para a posicao da [linha, coluna] recebidos por parametro
    if (!pos || !pos->getPlanta()) // se essa posicao nao existir OU se NÃO houver nenhuma planta lá, sai da funcao
        return false;

    delete pos->getPlanta(); // elimina-se a planta
    pos->removerPlanta(); // a posição para de apontar para essa planta (aponta agora para 'nullptr' - vaziu)

    colheitasRealizadas++; // e incrementa as colheitas feitas neste instante
    return true;
}

void Jardineiro::aplicarFerramenta(Jardim* jardim) { // para aplicar a ferramente que tem na mao
    if (!estaNoJardim || !ferramentaNaMao) // se o jardineiro nao estiver no jardim OU se nao tiver nenhuma ferramenta na mao, sai da funcao
        return;

    Posicao* pos = jardim->getPosicao(linhaAtual, colunaAtual); // 'pos' fica com um ponteiro para a posicao atual do jardineiro
    if (!pos) return; // se a posicao nao existir sai da funcao

    ferramentaNaMao->aplicar(pos); // aplica a ferramenta na posicao enviada por parametro

    if (ferramentaNaMao->estaGasta()) { // se a ferramenta estiver gasta, apaga-a e remove-a da mão
        cout << "Ferramenta " << ferramentaNaMao->getNumeroSerie() << " ficou gasta e desapareceu." << endl;
        delete ferramentaNaMao;
        ferramentaNaMao = nullptr;
    }
}

void Jardineiro::resetContadores() { // a cada instante que passa, reseta as seguintes variaveis:
    movimentosRealizados = 0;
    entradasRealizadas = 0;
    saidasRealizadas = 0;
    plantacoesRealizadas = 0;
    colheitasRealizadas = 0;
}

bool Jardineiro::getEstaNoJardim() const { return estaNoJardim; } // devolve bool: se está no jardim (true) ou nao (false)
int Jardineiro::getLinhaAtual() const { return linhaAtual; } // devolve a linha que o jardineiro se encontra atualmente
int Jardineiro::getColunaAtual() const { return colunaAtual; } // devolve a coluna que o jardineiro se encontra atualmente
Ferramenta* Jardineiro::getFerramentaNaMao() const { return ferramentaNaMao; } // devolve um ponteiro para a ferramenta que o jardineiro tem na mao
const vector<Ferramenta*>& Jardineiro::getFerramentas() const { return ferramentas; } // devolve um array de ponteiros para ferramentas que estao guardadas na mochila

void Jardineiro::apanharFerramentaSeExistir(Jardim* jardim) { // para apanhar ferramenta do chao, caso exista
    Posicao* pos = jardim->getPosicao(linhaAtual, colunaAtual); // 'pos' fica com um ponteiro para a posicao atual do jardineiro
    if (!pos) return; // se a posicao nao existir sai da funcao

    Ferramenta* f = pos->getFerramenta(); // f fica com um ponteiro para a ferramenta que está na posicao 'pos' (se nao houver ferramenta nesta posicao, 'f' fica com 'nullptr')
    if (f) { // se existir uma ferramenta (se não for 'nullptr')
        adicionarFerramenta(f); // adiciona a ferramenta à mochila
        pos->removerFerramenta(); // a posição para de apontar para essa ferramenta (aponta agora para 'nullptr' - vaziu)
        cout << "Ferramenta '" << f->getRepresentacao() << "'(nº " << f->getNumeroSerie() << ") apanhada!" << endl;

        apanheiFerramentaNesteMovimento = true; // para o simulador repor a nova ferramenta aleatoria no jardim
    }
}

bool Jardineiro::getApanheiFerramentaNesteMovimento() const { // funcao que devolve se apanhou agora uma ferramenta (true) ou nao (false) - para o simulador
    return apanheiFerramentaNesteMovimento;
}

void Jardineiro::resetFlagFerramentaApanhada() { // mete essa flag a false depois do simulador tratar do assunto (tipo “já gerei outra ferramenta, já está”)
    apanheiFerramentaNesteMovimento = false;
}
