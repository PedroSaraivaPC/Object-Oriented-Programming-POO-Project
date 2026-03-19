#ifndef PRATICALWORK_POO_JARDIM_H
#define PRATICALWORK_POO_JARDIM_H

#include "Posicao.h"
#include "Jardineiro.h"
#include <string>
#include <vector>

class Jardim {
private:
    Posicao** grelha; // matriz bidimensional (ponteiro para ponteiro) -> É um array de linhas onde cada linha é um array de posicoes
    int numLinhas; // numero de linhas da matriz
    int numColunas; // numero de colunas da matriz

public:
    Jardim(int linhas, int colunas); // construtor que recebe como parametro o nr de linhas e colunas
    ~Jardim(); // destrutor

    Posicao* getPosicao(int linha, int coluna); // devolve o endereço da posicao se estiver dentro dos limites, senão: nullptr
    bool coordenadasParaIndice(const std::string& coord, int& linha, int& coluna); // converte e.g. "ab" para (linha, coluna). Rerecebe como parametros: 'coord' onde vem com as coordenadas escritas pelo user; 'linha' que é o endereco duma variavel aqual a funcao vai la guardar o valor da linha; 'coluna' que é o endereco duma variavel aqual a funcao vai la guardar o valor da coluna

    void mostrar(const Jardineiro* j) const; // imprime o jardim na consola e desenha '*' onde o jardineiro está

    // getters para sacar o numero de linhas e colunas do jardim
    int getNumLinhas() const;
    int getNumColunas() const;

    void colocarFerramentaAleatoria(Ferramenta* f); // funcao que trata de colocar uma ferramenta numa posição vazia aleatória

    std::vector<Posicao*> getVizinhosLivres(int l, int c); // devolve um array de ponteiros para posicoes vizinhas livres à posicao que é recebida como parametro (linha e coluna)
};

#endif
