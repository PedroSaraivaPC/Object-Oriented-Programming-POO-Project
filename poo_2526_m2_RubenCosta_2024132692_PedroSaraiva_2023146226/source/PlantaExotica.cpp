#include "PlantaExotica.h"
#include "Posicao.h"
#include <algorithm>

PlantaExotica::PlantaExotica()
    : Planta(15, 10, Beleza::BONITA), // passa como parametro para o construtor da Planta.h os valores da 'aguaInicial', 'nutrientesInicial' e 'b'(beleza)
      instantesSemAgua(0) {}

PlantaExotica::~PlantaExotica() {} // vaziu -> não faz nada mais se não destruir-se a si próprio

void PlantaExotica::passarInstante(Posicao* ondeEsta) { // processa o comportamento da planta a cada instante que passa (recebendo um ponteiro para a sua posicao como parametro)
    idade++; // aumenta a idade da planta a cada instante

    // perde 3 unidades de água
    aguaAcumulada -= 3;
    nutrientesAcumulados -= 1;

    // absorcao da agua
    int aguaAbs = std::min(ondeEsta->getAgua(), 10); // compara a agua atual do solo com o máximo que a Planta Exotica pode absorver (10) e extrai o menor valor desses 2 (isto serve para quando o solo tem menos que 10 de agua, a Planta Exotica fica com esse valor que sobra)
    aguaAcumulada += aguaAbs; // soma à agua atual da planta
    ondeEsta->adicionarAgua(-aguaAbs); // retira essa agua ao solo

    // absorção dos nutrientes
    int nutrAbs = std::min(ondeEsta->getNutrientes(), 2); // compara os nutrientes atuais do solo com o máximo que a Planta Exotica pode absorver (2) e extrai o menor valor desses 2 (isto serve para quando o solo tem menos que 2 nutrientes, a Planta Exotica fica com esse valor que sobra)
    nutrientesAcumulados += nutrAbs; // soma aos nutrientes atuais da planta
    ondeEsta->adicionarNutrientes(-nutrAbs); // retira esses nutrientes que foram adicionados à planta do solo

    if (aguaAbs == 0) { // se a planta não absorveu agua entra no if
        instantesSemAgua++; // incrementa o contador de instantes que já não absorve água
        if (instantesSemAgua >= 3) { // se for o 3o instante seguido que a planta já não absorve água, morre
            morrer();
            return;
        }
    } else instantesSemAgua = 0;

    if (nutrientesAcumulados < 1) { // se os nutrientes acumulados for < 1 a planta tambem morre
        morrer();
    }
}

char PlantaExotica::getRepresentacao() const {
    return 'x';
}
