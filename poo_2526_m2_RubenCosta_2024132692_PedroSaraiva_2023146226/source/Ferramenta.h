#ifndef PRATICALWORK_POO_FERRAMENTA_H
#define PRATICALWORK_POO_FERRAMENTA_H

class Posicao;

class Ferramenta {
protected: // acessível pelas subclasses
    int numeroSerie; // seu numero de serie
    static int proximoNumeroSerie; // contador global de séries

public:
    Ferramenta(); // construtor base (atribui o nº de serie)
    virtual ~Ferramenta(); // destrutor virtual (importante para apagar via ponteiro base (Ferramenta* f = new Regador(); | delete f;). Apaga a subclasse e só depois a Ferramenta.h)

    // funções puras (tornam Ferramenta.h uma classe abstrata) -> obrigatórias nas subclasses (são apenas as subclasses que chamam estas funções)
    virtual char getRepresentacao() const = 0; // para aparecer como 'g' no jardim e em listagens
    virtual void aplicar(Posicao* onde) = 0; // faz o efeito no solo (recebe como parametro um ponteiro para a posicao)
    virtual bool estaGasta() const = 0; // diz se já acabou

    int getNumeroSerie() const; // getter do n.º série da ferramenta
};

#endif //PRATICALWORK_POO_FERRAMENTA_H