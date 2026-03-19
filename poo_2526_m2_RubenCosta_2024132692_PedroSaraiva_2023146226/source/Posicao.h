#ifndef PRATICALWORK_POO_POSICAO_H
#define PRATICALWORK_POO_POSICAO_H

#include <string>
class Planta;
class Ferramenta;
class Jardim;

class Posicao {
private:
    int agua; // quantidade de agua nesta posicao do solo
    int nutrientes; // quantidade de nutrientes nesta posicao do solo

    Planta* planta; // ponteiro para a planta (caso tenha nesta posicao)
    Ferramenta* ferramenta; // ponteiro para a ferramenta (caso haja nesta posicao)

    Jardim* jardim; // para se ligar ao jardim e obter mais informacoes sobre o mesmo, como vizinhos, dimensoes, etc.
    int linha; // linha da posicao em si
    int coluna; // coluna da posicao em si

public:
    Posicao(); // construtor da classe
    ~Posicao(); // destrutor da classe

    void setLocalizacao(Jardim* j, int l, int c); // serve para ligar a posição ao jardim e guardar as coordenadas

    // getters que retornam logo a linha, a coluna, e jardim
    int getLinha() const { return linha; }
    int getColuna() const { return coluna; }
    Jardim* getJardim() const { return jardim; }

    int getAgua() const; // devolve a água existente no solo desta posição
    int getNutrientes() const; // devolve os nutrientes do solo desta posição

    Planta* getPlanta() const; // devolve o ponteiro para a planta que está nesta posição (se houver)
    Ferramenta* getFerramenta() const; // devolve o ponteiro para a ferramenta que está nesta posição (se houver)

    void adicionarAgua(int quantidade); // serve para alterar a agua do solo nesta posicao
    void adicionarNutrientes(int quantidade); // serve para alterar os nutrientes do solo nesta posicao

    void colocarPlanta(Planta* p); // serve para colocar uma planta na posição
    void removerPlanta(); // serve para retirar a planta da posicao

    void colocarFerramenta(Ferramenta* f); // serve para colocar uma ferramenta na posição
    void removerFerramenta(); // serve para retirar a ferramenta da posicao

    char getRepresentacao() const; // serve para dizer qual é o caractere a imprimir nesta posicao do jardim
};

#endif
