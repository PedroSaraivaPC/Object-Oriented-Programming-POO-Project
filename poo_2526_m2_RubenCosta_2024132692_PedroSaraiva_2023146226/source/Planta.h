#ifndef PRATICALWORK_POO_PLANTA_H
#define PRATICALWORK_POO_PLANTA_H

#include <iostream>

class Posicao;

enum class Beleza { // classe enumerada para distinguir a beleza das plantas
    FEIA,
    NEUTRA,
    BONITA
};

class Planta {
protected:
    int aguaAcumulada; // água atual dentro da planta
    int nutrientesAcumulados; // nutrientes atuais dentro da planta

    // Para regras do enunciado (e.g. “deixar no solo X do que absorveu ao longo da vida”)
    int aguaTotalAbsorvida; // da planta
    int nutrientesTotaisAbsorvidos; // da planta

    Beleza beleza; // beleza da planta
    bool morta; // bool de se está viva ou morta (para isDead() e para a FerramentaZ)
    int idade; // idade da planta (incrementa a cada instante que passa)

public:
    Planta(int aguaInicial, int nutrientesInicial, Beleza b); // valores iniciais de cada planta (subclasse) que cada uma delas decide (nem é o jardineiro) no seu construtor
    virtual ~Planta(); // destrutor virtual (importante para apagar via ponteiro base (Planta* p = new Cacto(); | delete p;). Apaga a subclasse e só depois a Planta.h)

    // funções puras (tornam Planta.h uma classe abstrata) -> obrigatórias nas subclasses (são apenas as subclasses que chamam estas funções)
    virtual void passarInstante(Posicao* ondeEsta) = 0; // processa o comportamento da planta a cada instante que passa
    virtual char getRepresentacao() const = 0; // retorna o char da planta (c-cacto|r-roseira|e-ervadaninha|x-plantaExotica)

    // getters
    char getRepresentacaoFinal() const;
    bool isDead() const;
    Beleza getBeleza() const;
    int getAgua() const;
    int getNutrientes() const;
    int getIdade() const;

    // setters
    void setAgua(int a); // alterar a 'aguaAcumulada' para o que está a receber como parametro
    void setNutrientes(int n); // alterar os 'nutrientesAcumulados' para o que está a receber como parametro
    void morrer(); // altera o 'morta' para true quando a planta morre
};

#endif
