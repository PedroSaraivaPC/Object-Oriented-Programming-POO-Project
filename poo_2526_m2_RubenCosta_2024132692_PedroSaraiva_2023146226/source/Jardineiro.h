#ifndef PRATICALWORK_POO_JARDINEIRO_H
#define PRATICALWORK_POO_JARDINEIRO_H

#include <vector>

class Jardim;
class Ferramenta;

class Jardineiro {
private:
    int linhaAtual; // linha onde se encontra o jardineiro
    int colunaAtual; // coluna onde se encontra o jardineiro
    bool estaNoJardim; // se esta no jardim (true) ou nao (false)

    Ferramenta* ferramentaNaMao; // ferramenta que o jardineiro tem na mao
    std::vector<Ferramenta*> ferramentas; // array de ponteiros para ferramentas que estao guardadas na mochila

    // variaveis para regras do enunciad
    int movimentosRealizados;
    int entradasRealizadas;
    int saidasRealizadas;
    int plantacoesRealizadas;
    int colheitasRealizadas;

    void apanharFerramentaSeExistir(Jardim* jardim); // para apanhar ferramenta do chao, caso exista
    bool apanheiFerramentaNesteMovimento; // se apanhou agora uma ferramenta (true) ou nao (false)

public:
    Jardineiro(); // construtor
    ~Jardineiro(); // destrutor

    // muda de direcao e recebem 'Jardim* jardim' como parametro para saber limites da grelha e para poder apanhar a ferramenta na nova posição (caso haja)
    bool moverEsquerda(Jardim* jardim);
    bool moverDireita(Jardim* jardim);
    bool moverCima(Jardim* jardim);
    bool moverBaixo(Jardim* jardim);

    bool entrar(Jardim* jardim, int linha, int coluna); // coloca o jardineiro na posicao [linha, coluna]
    bool sair(); // jardineiro sai do jardim

    void adicionarFerramenta(Ferramenta* f); // adiciona ferramentas na mochila
    bool pegarFerramenta(int numeroSerie); // pega na ferramenta que está na mochila com o n.º de serie recebido por parametro
    bool largarFerramenta(); // tira a ferramenta na mao e guarda-a na mochila

    bool plantar(Jardim* jardim, int linha, int coluna, char tipo); // planta uma planta do tipo 'tipo' na posicao [linha, coluna]
    bool colher(Jardim* jardim, int linha, int coluna); // colhe uma planta que esteja na posicao [linha, coluna]

    void aplicarFerramenta(Jardim* jardim); // para aplicar a ferramente que tem na mao
    void resetContadores(); // a cada instante que passa, reseta: movimentosRealizados, entradas/saídas, plantações e colheitas

    bool getEstaNoJardim() const; // se está no jardim (true) ou nao (false)
    int getLinhaAtual() const; // devolve a linha que o jardineiro se encontra atualmente
    int getColunaAtual() const; // devolve a coluna que o jardineiro se encontra atualmente
    Ferramenta* getFerramentaNaMao() const; // devolve um ponteiro para a ferramenta que o jardineiro tem na mao
    const std::vector<Ferramenta*>& getFerramentas() const; // devolve um array de ponteiros para ferramentas que estao guardadas na mochila
    bool getApanheiFerramentaNesteMovimento() const; // funcao que devolve se apanhou agora uma ferramenta (true) ou nao (false) - para o simulador
    void resetFlagFerramentaApanhada(); // mete essa flag a false depois do simulador tratar do assunto (tipo “já gerei outra ferramenta, já está”)
};

#endif
