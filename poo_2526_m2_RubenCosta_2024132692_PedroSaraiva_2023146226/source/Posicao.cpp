#include "Posicao.h"
#include "Planta.h"
#include "Ferramenta.h"
#include "Jardim.h"
#include "Settings.h"

#include <cstdlib>
#include <cctype>
#include <ctime>

Posicao::Posicao()
    : agua(0),
      nutrientes(0),
      planta(nullptr),
      ferramenta(nullptr),
      jardim(nullptr),
      linha(-1),
      coluna(-1)
{
    // gerador de numeros aleatorios
    static bool seed = false; // static para existir apenas uma vez só para o programa inteiro (não uma por objeto)
    if (!seed) {
        srand(time(nullptr)); // inicializa o gerador uma vez (para o programa não começar sempre igual)
        seed = true;
    }

    // gera valores aleatorios da agua e dos nutrientes para cada posicao
    agua = Settings::Jardim::agua_min +
           rand() % (Settings::Jardim::agua_max - Settings::Jardim::agua_min + 1); // entre 80 e 100

    nutrientes = Settings::Jardim::nutrientes_min +
                 rand() % (Settings::Jardim::nutrientes_max - Settings::Jardim::nutrientes_min + 1); // entre 30 e 50
}

Posicao::~Posicao() { // antes de se destruir a si proprio, apaga aa planta ou a ferramente (caso existam nesta posicao)
    if (planta != nullptr) {
        delete planta;      // Liberta a memória da Planta
    }
    if (ferramenta != nullptr) {
        delete ferramenta;  // Liberta a memória da Ferramenta que está no chão
    }
}

void Posicao::setLocalizacao(Jardim* j, int l, int c) { // guarda as coordenadas e o jardim
    jardim = j;
    linha = l;
    coluna = c;
}

int Posicao::getAgua() const { return agua; } // devolve a água existente no solo desta posição
int Posicao::getNutrientes() const { return nutrientes; } // devolve os nutrientes do solo desta posição

Planta* Posicao::getPlanta() const { return planta; } // devolve o ponteiro para a planta que está nesta posição (se houver)
Ferramenta* Posicao::getFerramenta() const { return ferramenta; } // devolve o ponteiro para a ferramenta que está nesta posição (se houver)

void Posicao::adicionarAgua(int quantidade) { // serve para alterar a agua do solo nesta posicao
    agua += quantidade;
    if (agua < 0) agua = 0;
}

void Posicao::adicionarNutrientes(int quantidade) { // serve para alterar os nutrientes do solo nesta posicao
    nutrientes += quantidade;
    if (nutrientes < 0) nutrientes = 0;
}

void Posicao::colocarPlanta(Planta* p) { // serve para colocar uma planta na posição
    planta = p;
}

void Posicao::removerPlanta() { // serve para retirar a planta da posicao
    planta = nullptr;
}

void Posicao::colocarFerramenta(Ferramenta* f) { // serve para colocar uma ferramenta na posição
    ferramenta = f;
}

void Posicao::removerFerramenta() { // serve para retirar a ferramenta da posicao
    ferramenta = nullptr;
}

char Posicao::getRepresentacao() const { // serve para dizer qual é o caractere a imprimir nesta posicao do jardim
    if (planta) return planta->getRepresentacaoFinal();
    if (ferramenta) return ferramenta->getRepresentacao();
    return ' ';
}
