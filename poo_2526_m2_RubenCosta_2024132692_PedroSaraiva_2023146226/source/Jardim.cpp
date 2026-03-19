#include "Jardim.h"
#include "Ferramenta.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Jardim::Jardim(int linhas, int colunas) {
        numLinhas = linhas; // guarda o nº de linhas recebidas por parametro
        numColunas = colunas; // guarda o nº de colunas recebidas por parametro

        // cria a matriz bidimensional
        grelha = new Posicao*[numLinhas]; // cria um array com numLinhas(e.g. 10) ponteiros do tipo 'Posicao*' onde cada elemento aponta para uma linha
        for (int i = 0; i < numLinhas; i++) { // corre todas as linhas
                grelha[i] = new Posicao[numColunas]; // cria a linha 'i': um array de numColunas(e.g. 10) objetos 'Posicao' onde cada elemento são posicoes (nao ponteiros)
                for (int c = 0; c < numColunas; c++) { // corre todas as colunas
                        grelha[i][c].setLocalizacao(this, i, c);  // atribui dados a CADA posicao, enviando como parametro o jardim em si como objeto(this), a linha(i) e a coluna(c)
                }
        }
}

Jardim::~Jardim() { // antes de se destruir apaga a matriz bidimensional
        for (int i = 0; i < numLinhas; i++) // corre todas as linhas
                delete[] grelha[i]; // apaga cada linha da matriz
        delete[] grelha; // apaga a matriz
}

Posicao* Jardim::getPosicao(int linha, int coluna) { // devolve o endereço da posicao se estiver dentro dos limites, senão: nullptr
        if (linha >= 0 && linha < numLinhas &&
            coluna >= 0 && coluna < numColunas)
                return &grelha[linha][coluna];
        return nullptr;
}

bool Jardim::coordenadasParaIndice(const string& coord, int& linha, int& coluna) { // converte e.g. "ab" para (linha, coluna). Rerecebe como parametros: 'coord' onde vem com as coordenadas escritas pelo user; 'linha' que é o endereco duma variavel aqual a funcao vai la guardar o valor da linha; 'coluna' que é o endereco duma variavel aqual a funcao vai la guardar o valor da coluna
        if (coord.length() != 2) return false; // se as coordenadas nao forem 2 letras
        linha = coord[0] - 'a'; // tabela ascii -> passar para numero (subtrai a posicao do 'a')
        coluna = coord[1] - 'a'; // tabela ascii -> passar para numero (subtrai a posicao do 'a')
        return linha >= 0 && linha < numLinhas &&
               coluna >= 0 && coluna < numColunas; // retorna true se estiver dentro dos limites e false se nao
}

void Jardim::mostrar(const Jardineiro* j) const { // imprime o jardim na consola e desenha '*' onde o jardineiro está

        // Letras das colunas (em cima): A B C D
        cout << "   ";
        for (int c = 0; c < numColunas; c++)
                cout << char('A' + c) << " ";
        cout << "\n";

        // Moldura superior: +----+
        cout << "  +";
        for (int c = 0; c < numColunas * 2; c++)
                cout << "-";
        cout << "+\n";

        // Conteúdo do jardim
        for (int l = 0; l < numLinhas; l++) {
                cout << char('A' + l) << " |"; // para passar do nº da linha para letra em maiuscula (somar a posicao do 'A')

                for (int c = 0; c < numColunas; c++) {
                        char simbolo;

                        // segue a ordem pedida no enunciado: jardineiro->plantas->ferramentas
                        if (j && j->getEstaNoJardim() &&
                            j->getLinhaAtual() == l &&
                            j->getColunaAtual() == c)
                                simbolo = '*';
                        else
                                simbolo = grelha[l][c].getRepresentacao();

                        cout << simbolo << "|";
                }

                cout << "\n";
        }

        // Moldura inferior: +----+
        cout << "  +";
        for (int c = 0; c < numColunas * 2; c++)
                cout << "-";
        cout << "+\n";

        // Letras das colunas (em baixo): A B C D
        cout << "   ";
        for (int c = 0; c < numColunas; c++)
                cout << char('A' + c) << " ";
        cout << "\n";
}

// getters para sacar o numero de linhas e colunas do jardim
int Jardim::getNumLinhas() const {
        return numLinhas;
}
int Jardim::getNumColunas() const {
        return numColunas;
}

void Jardim::colocarFerramentaAleatoria(Ferramenta* f) { // funcao que trata de colocar uma ferramenta numa posição vazia aleatória
        if (!f) return; // se f for nullptr, sai logo

        int tentativas = 100; // faz até 100 tentativas para não ficar preso num loop infinito se o jardim estiver cheio

        while (tentativas--) {
                // escolhe célula aleatória
                int l = rand() % numLinhas;
                int c = rand() % numColunas;

                Posicao* p = &grelha[l][c]; // cria uma posicao com a linha e coluna gerada aleatoriamente anteriormente
                if (!p) continue; // se nao existir, continua no loop e tenta noutra posicao

                if (!p->getFerramenta() && !p->getPlanta()) { // se a posicao nao tiver ferramenta nem tiver planta
                        p->colocarFerramenta(f); // coloca a ferramenta nessa posicao
                        return;
                }
        }
        // fallback: se não encontrou posição para colocar a ferramenta, elimina a mesma para nao haver memory leak (ferramenta ficava "sem dono")
        delete f;
}

std::vector<Posicao*> Jardim::getVizinhosLivres(int l, int c) { // devolve um array de ponteiros para posicoes vizinhas livres à posicao que é recebida como parametro (linha e coluna)
        std::vector<Posicao*> v; // array de ponteiros para posicoes
        for (int dl = -1; dl <= 1; dl++) { // percorre 3 linhas: -1 0 1 (sendo 0 a linha onde está a posicao atual)
                for (int dc = -1; dc <= 1; dc++) { // percorre 3 colunas: -1 0 1 (sendo 0 a coluna onde está a posicao atual)
                        if (dl == 0 && dc == 0) continue; // ignora a posicao onde está a posicao atual (0 0)
                        int nl = l + dl; // nl -> é a soma da linha atual + o '-1 0 1' vindo do for (dl)
                        int nc = c + dc; // nc -> é a soma da coluna atual + o '-1 0 1' vindo do for (dc)

                        if (nl >= 0 && nl < numLinhas && nc >= 0 && nc < numColunas) { // se essa posicao final estiver entre os limites do jardim
                                Posicao* p = getPosicao(nl, nc); // pega na posicao
                                if (p->getPlanta() == nullptr) // e verifica se tem lá uma planta, se não tiver planta (se tiver vaziu), adiciona essa posicao ao array de ponteiros de posicoes vazias
                                        v.push_back(p);
                        }
                }
        }
        return v; // retorna o array de ponteiros para posicoes vizinhas livres à posicao atual
}
