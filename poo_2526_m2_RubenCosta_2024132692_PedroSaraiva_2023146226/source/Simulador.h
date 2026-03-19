#ifndef PRATICALWORK_POO_SIMULADOR_H
#define PRATICALWORK_POO_SIMULADOR_H

#include <string>
#include <vector>
#include <map>
#include "Jardim.h"
#include "Jardineiro.h"

using namespace std;

class Simulador {
private:
    Jardim* jardim; // ponteiro para o objeto Jardim
    Jardineiro* jardineiro; // ponteiro para o Jardineiro
    int instanteAtual; // contador de instantes
    bool jardimCriado; // que indica se o jardim já foi criado (true) ou nao (false)
    Ferramenta* criarFerramentaAleatoria(); // metodo privado que cria uma ferramenta aleatória

    std::vector<std::string> dividirString(const std::string& str); // recebe uma string e devolve um vetor com as palavras separadas (para processar comandos)
    bool validarTipoPlanta(char tipo); // verifica se um caractere representa um tipo de planta válido (c, r, e ou x)

    bool cmdAvanca(const std::vector<std::string>& args); // processa o comando "avanca [n]" para avançar n instantes

    bool cmdLPlantas(); // lista todas as plantas do jardim
    bool cmdLPlanta(const std::vector<std::string>& args); // lista informacao sobre uma planta específica numa coordenada
    bool cmdLArea(); // lista todas as posições que têm conteúdo (plantas ou ferramentas)
    bool cmdLSolo(const std::vector<std::string>& args); // lista informacao sobre o solo numa posicao (e opcionalmente numa área com raio)
    bool cmdLFerr(); // lista as ferramentas do jardineiro (tanto na mao como na mochila)

    bool cmdColhe(const std::vector<std::string>& args); // processa o comando para colher uma planta numa posição
    bool cmdPlanta(const std::vector<std::string>& args); // processa o comando para plantar uma nova planta
    bool cmdLarga(); // faz o jardineiro largar a ferramenta que tem na mão (vai para a mochila)
    bool cmdPega(const std::vector<std::string>& args); // faz o jardineiro pegar numa ferramenta pelo número de série (da mochila)
    bool cmdCompra(const std::vector<std::string>& args); // processa a compra de uma nova ferramenta

    bool cmdE(const std::vector<std::string>& args); // move o jardineiro para a esquerda
    bool cmdD(const std::vector<std::string>& args); // move para a direita
    bool cmdC(const std::vector<std::string>& args); // move para cima
    bool cmdB(const std::vector<std::string>& args); // move para baixo
    bool cmdEntra(const std::vector<std::string>& args); // faz o jardineiro entrar no jardim numa posição específica
    bool cmdSai(); // faz o jardineiro sair do jardim

    bool cmdJardim(const std::vector<std::string>& args); // processa o comando que cria o jardim com dimensões específicas

    struct GravaMem { // define uma estrutura para guardar o estado do jardim em memória (para depois ser salvo num ficheiro)
        int linhas, colunas; // guardam as dimensões do jardim

        bool jardineiroNoJardim; // indica se o jardineiro está ou não no jardim
        int jl, jc; // guardam a linha e coluna onde está o jardineiro

        // vetores que guardam o estado de cada posição (usa-se um vetor linear, não uma matriz)
        std::vector<int> agua; // guarda a agua de cada posicao
        std::vector<int> nutrientes; // guarda os nutrientes de cada posicao
        std::vector<char> plantas; // guarda o caractere da planta de cada posicao (se houver). Se não houver, guarda '-'

        // vetor que guarda as 3 ferramentas do jardim
        std::vector<char> ferramentas; // guarda as ferramentas que estao no jardim (ou '-' se não houver)
    };

    std::map<std::string, GravaMem> gravacoes; // é um mapa/tabela: [nome/id , gravação(struct)] que associa um nome/id (string) a uma gravação (GravaMem)

    bool cmdGrava(const std::vector<std::string>& args); // guarda o estado atual do jardim com um nome
    bool cmdRecupera(const std::vector<std::string>& args); // recupera um estado previamente gravado (e apaga-o da memória)
    bool cmdApaga(const std::vector<std::string>& args); // apaga uma gravação sem recuperá-la
    bool cmdExecuta(const std::vector<std::string>& args); // lê e executa comandos de um ficheiro

    void processarInstante(); // faz passar um instante de tempo (plantas reagem, ferramentas atuam, etc.)
    bool lerInteiroSeguro(const std::string& s, int& valor); // tenta converter uma string 's' num inteiro ('valor') de forma segura, devolvendo false se falhar
    void reporFerramentaAleatoria(); // coloca uma nova ferramenta aleatória no jardim (quando uma é apanhada pelo jardineiro)

public:
    Simulador(); // construtor que inicializa o simulador
    ~Simulador(); // destrutor que liberta recursos quando o simulador é destruído

    void executar(); // metodo principal que inicia o loop de comandos do simulador
    bool processarComando(const string& linha); // recebe uma linha de texto e processa o comando
    void mostrarJardim(); // mostra a representação visual do jardim
};

#endif
