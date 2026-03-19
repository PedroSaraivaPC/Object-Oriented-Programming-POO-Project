#include "Simulador.h"
#include "Regador.h"
#include "Adubo.h"
#include "Tesoura.h"
#include "FerramentaZ.h"
#include "Planta.h"
#include "Cacto.h"
#include "Ervadaninha.h"
#include "Roseira.h"
#include "PlantaExotica.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cctype>

using namespace std;

Simulador::Simulador()
    : jardim(nullptr),
      jardineiro(new Jardineiro()), // jardineiro existe sempre (mesmo quando não está no jardim)
      instanteAtual(0),
      jardimCriado(false) {}

Simulador::~Simulador() { // antes de se destruir a si proprio, apaga o jardim e o jardineiro
    if (jardim) delete jardim;
    if (jardineiro) delete jardineiro;
}

bool Simulador::lerInteiroSeguro(const string& s, int& valor) { // tenta converter uma string 's' num inteiro ('valor') de forma segura
    try {
        size_t idx; // usa-se 'size_t', pq 's.size()' devolve um 'size_t' e pq 'stoi' pede um 'size_t' para o índice. Size_t é um inteiro sem sinal (unsigned - nao tem negativos)
        valor = stoi(s, &idx); // converte a string para inteiro e guarda em 'valor', e guarda em idx a posição onde a conversão parou
        return idx == s.size(); // verifica se converteu a string toda (se idx chegou ao fim da string)
    }
    catch (...) { // se houver alguma exceção, retorna false
        return false;
    }
}

vector<string> Simulador::dividirString(const string& str) { // divide uma string em palavras separadas por espaços
    vector<string> palavras; // cria um vetor vazio 'palavras' para guardar as palavras
    istringstream iss(str); // cria um stream da string (permite ler dela como se fosse input)
    string t;
    while (iss >> t) palavras.push_back(t); // lê palavra a palavra do stream (o operador >> ignora espaços automaticamente). Cada palavra lida é adicionada ao vetor 'palavras'
    return palavras; // retorna o array
}

bool Simulador::validarTipoPlanta(char t) { // verifica se o caractere é 'c' (cacto), 'r' (roseira), 'e' (ervadaninha) ou 'x' (exótica)
    return t=='c'||t=='r'||t=='e'||t=='x'; // se sim (true), se nao (false)
}

void Simulador::mostrarJardim() { // mostra a representação visual do jardim
    if (jardim) jardim->mostrar(jardineiro); // se o jardim existir (não for 'nullptr'), chama a funcao 'mostrar' do objeto 'jardim', enviando o jardineiro como parametro
    else cout << "Ainda não existe jardim.\n"; // se nao
}

void Simulador::processarInstante() { // faz passar um instante de tempo
    if (!jardimCriado || !jardim) // se o jardim ainda nao foi criado ou se nao existe se quer, sai da funcao
        return;

    for (int l = 0; l < jardim->getNumLinhas(); ++l) { // percorre todas as linhas do jardim
        for (int c = 0; c < jardim->getNumColunas(); ++c) { // // percorre todas as colunas do jardim
            Posicao* pos = jardim->getPosicao(l, c); // saca cada posicao individual
            if (!pos) continue; // se a posicao nao existir, passa para a proxima posicao

            Planta* p = pos->getPlanta(); // p é um ponteiro para a planta que está nesta posicao (se houver)
            if (p && !p->isDead()) { // se houver uma planta nessa posição e ela não estiver morta
                p->passarInstante(pos); // chama e.g. Cacto::passarInstante() e não Planta::passarInstante() por causa do virtual: "= 0"

                if (p->isDead()) { // se a planta, depois desta alteracao, morrer
                    cout << "Planta em " << (char)('a'+l) << (char)('a'+c) << " morreu!\n"; // passa os valores da linha e coluna para coordenada
                }
            }
        }
    }

    jardineiro->aplicarFerramenta(jardim); // após processar todas as plantas, aplica a ferramenta que o jardineiro tem na mão (se tiver)
    jardineiro->resetContadores(); // todos os contadores do jardineiro resetam
    instanteAtual++; // incrementa o contador do instante atual
}

void Simulador::executar() {
    cout << "Simulador iniciado.\n";
    string linha;

    while (true) { // loop infinito (só termina com o comando "fim")
        cout << "> ";
        getline(cin, linha); // 'linha' guarda a linha toda que o user escreveu
        if (linha == "fim") { // se a linha for "fim", sai do programa
            cout << "A terminar programa....";
            break;
        }

        processarComando(linha); // se nao for para fechar o programa, processa o comando normal
    }
}

bool Simulador::processarComando(const string& linha) {
    auto t = dividirString(linha); // 't' é um array de strings com as palavras vindas de 'linha'
    if (t.empty()) return true; // se vier vaziu, devolve true e volta a ler o 'cin'(character input - STDIN em SO) na linha de comandos

    string cmd = t[0]; // comando principal(t[0]) guardado em 'cmd'
    bool alterou = false; // flag que indica se o comando alterou o jardim (se sim, vai mostrar o jardim no fim)

    // 'jardim' e 'executa' são os únicos comandos que podem ser executados antes de o jardim ser criado
    if (cmd == "jardim") alterou = cmdJardim(t);
    else if (cmd == "executa") alterou = cmdExecuta(t);

    else {
        if (!jardimCriado) { // só deixa avancar para um comando "normal" se o jardim já estiver criado
            cout << "ERRO: Crie o jardim primeiro.\n";
            return false;
        }

        if (cmd == "avanca") alterou = cmdAvanca(t);

        // alguns fazem "return ="(aquelas funcoes que não mudam visualmente o jardim) e outros "alterou ="(que caso corra tudo bem, altera visualmente o jardim, retorna true e mostra-o novamente)
        else if (cmd == "lplantas") return cmdLPlantas();
        else if (cmd == "lplanta") return cmdLPlanta(t);
        else if (cmd == "larea") return cmdLArea();
        else if (cmd == "lsolo") return cmdLSolo(t);
        else if (cmd == "lferr") return cmdLFerr();

        else if (cmd == "colhe") alterou = cmdColhe(t);
        else if (cmd == "planta") alterou = cmdPlanta(t);
        else if (cmd == "larga") return cmdLarga();
        else if (cmd == "pega") return cmdPega(t);
        else if (cmd == "compra") return cmdCompra(t);

        else if (cmd == "a") alterou = cmdE(t);
        else if (cmd == "d") alterou = cmdD(t);
        else if (cmd == "w") alterou = cmdC(t);
        else if (cmd == "s") alterou = cmdB(t);
        else if (cmd == "entra") alterou = cmdEntra(t);
        else if (cmd == "sai") alterou = cmdSai();

        else if (cmd == "grava") alterou = cmdGrava(t);
        else if (cmd == "recupera") alterou = cmdRecupera(t);
        else if (cmd == "apaga") return cmdApaga(t);

        else { // caso o comando nao seja reconhecido
            cout << "ERRO: Comando invalido. Comandos disponiveis:\n";
            cout << "\tjardim <lin> <col>\n";
            cout << "\texecuta <ficheiro>\n";

            cout << "\n\tavanca <n>\n";

            cout << "\n\tentra <coord>\n";
            cout << "\tsai\n";
            cout << "\tw | a | s | d\n";

            cout << "\n\tlplantas\n";
            cout << "\tlplanta <coord>\n";
            cout << "\tlarea\n";
            cout << "\tlsolo <coord>\n";
            cout << "\tlferr\n";

            cout << "\n\tplanta <coord> <c|r|e|x>\n";
            cout << "\tcolhe <coord>\n";

            cout << "\n\tcompra <g|a|z|t>\n";
            cout << "\tpega <ferram>\n";
            cout << "\tlarga\n";

            cout << "\n\tgrava <nome>\n";
            cout << "\trecupera <nome>\n";
            cout << "\tapaga <nome>\n";

            cout << "\n\tfim\n";
            return false;
        }
    }

    if (alterou) // se as funcoes que devolvem true/false e guardam em 'alterou' retornarem true, mostra o jardim
        mostrarJardim();

    return alterou;
}

bool Simulador::cmdJardim(const vector<string>& t) {

    if (jardimCriado) { // caso o jardim já tenha sido criado
        cout << "ERRO: O jardim ja foi criado.\n";
        return false;
    }

    if (t.size() != 3) { // caso o comando venha mal formatado(diferente de 3 "palavras")
        cout << "ERRO: jardim <linhas> <colunas>\n";
        return false;
    }

    int l, c;
    if (!lerInteiroSeguro(t[1], l) || !lerInteiroSeguro(t[2], c)) { // tenta passar de string para inteiros e guarda em 'l' e 'c' (stoi)
        cout << "ERRO: Valores invalidos.\n";
        return false;
    }

    if (l < 1 || l > 26 || c < 1 || c > 26) { // verifica se está dentro do limite permitido
        cout << "ERRO: tamanhos invalidos (tamanho máximo é 26x26). \n";
        return false;
    }

    jardim = new Jardim(l, c); // cria o jardim
    jardimCriado = true; // flag a true para indicar que o jardim já foi criado

    for (int i = 0; i < 3; i++) { // colocar 3 ferramentas aleatórias no jardim
        Ferramenta* f = criarFerramentaAleatoria(); // gera a ferramenta nova com 25% de probabilidade para cada uma
        jardim->colocarFerramentaAleatoria(f); // e coloca-a no jardim
    }

    cout << "Jardim criado com 3 ferramentas aleatorias!\n";
    return true;
}

bool Simulador::cmdExecuta(const vector<string>& t) {
    if (t.size() != 2) { // valida se foi passado o nome do ficheiro / se está mal formatado (diferente de 2 "palavras")
        cout << "ERRO: executa <ficheiro>\n";
        return false;
    }

    ifstream f(t[1]); // tenta abrir o ficheiro com o nome: 't[1]'
    if (!f.is_open()) { // se nao conseguiu abrir
        cout << "ERRO: Nao foi possivel abrir o ficheiro '" << t[1] << "'.\n";
        return false;
    }

    string linha;
    while (getline(f, linha)) { // 'linha' fica com cada linha do ficheiro 'f'
        processarComando(linha); // cada linha/comando é processada
    }

    return true;
}

bool Simulador::cmdAvanca(const vector<string>& t) {
    int n = 1;
    if (t.size() == 2) { // entra só caso 't' tenha 2 "palavras"
        if (!lerInteiroSeguro(t[1], n) || n < 1) { // tenta passar de string para inteiro, guardando em 'n' o nº de instantes (se der erro, entra no if). Se o valor guardado em 'n' for negativo, também entra no if
            cout << "ERRO: avanca <n>\n";
            return false;
        }
    } else {
        cout << "ERRO: avanca <n>\n";
        return false;
    }
    for (int i = 0; i < n; i++) processarInstante(); // para cada instante, processa o mesmo
    return true;
}

bool Simulador::cmdColhe(const vector<string>& t) {
    if (t.size() != 2) { // valida se foram passadas as coordenadas
        cout << "ERRO: colhe <l><c>\n";
        return false;
    }

    int l, c;
    if (!jardim->coordenadasParaIndice(t[1], l, c)) { // tenta converter as coordenadas (e.g. "ab") para índices numéricos('l' e 'c'), caso dê erro entra no if
        cout << "ERRO: Coordenadas invalidas.\n";
        return false;
    }

    Posicao* p = jardim->getPosicao(l, c); // pega na posicao dessa coordenada
    if (!p || !p->getPlanta()) { // se nao existir a posicao ou se nao houver la planta, entra no if
        cout << "ERRO: Nao existe planta nessa posicao.\n";
        return false;
    }

    bool ok = jardineiro->colher(jardim, l, c); // colhe a planta
    if (!ok) { // se antingir o limite de 5 colheitas num instante
        cout << "ERRO: Limite de colheitas por turno atingido.\n";
        return false;
    }
    cout << "[DEBUG]: A planta foi colhida com sucesso!\n";
    return true;
}

bool Simulador::cmdPlanta(const vector<string>& args) {
    if (args.size() != 3) { // valida se foram passados 3 argumentos (comando + coordenadas + tipo)
        cout << "ERRO: planta <l><c> <tipo>\n";
        return false;
    }

    string xy = args[1]; // xy guarda as coordenadas

    if (xy.size() != 2 || xy[0] < 'a' || xy[0] > 'z' || xy[1] < 'a' || xy[1] > 'z') { // valida manualmente se as coordenadas são 2 letras minúsculas. Se não forem, entra no if
        cout << "ERRO: Coordenadas invalidas.\n";
        return false;
    }

    int linha, coluna;
    if (!jardim->coordenadasParaIndice(xy, linha, coluna)) { // passa as coordenadas para numeros(guardando em 'l' e 'c'). Se der erro, entra no if
        cout << "ERRO: Fora dos limites.\n";
        return false;
    }

    char tipo = args[2][0]; // pega no tipo da planta vindo de 'args[2]'
    if (!validarTipoPlanta(tipo)) { // valida se o tipo está correto (c, r, e ou x)
        cout << "ERRO: Planta invalida. Existem as seguintes plantas:";
        cout << "\n\tErvadaninha - e \n\tCacto - c \n\tPlanta Exotica - x \n\tRoseira - r\n";
        return false;
    }

    cout << "[DEBUG] PLANTAR em (" << linha << "," << coluna << ") tipo '" << tipo << "'\n";

    Posicao* p = jardim->getPosicao(linha, coluna); // pega na posicao das coordenadas

    if (p->getPlanta() != nullptr) { // se já existir uma planta na posicao entra no if
        cout << "ERRO: Ja existe uma planta nessa posicao.\n";
        return false;
    }

    bool ok = jardineiro->plantar(jardim, linha, coluna, tipo); // planta a planta e o resultado bool fica em 'ok'

    if (!ok) { // se retornar false
        cout << "ERRO: Limite de plantacoes por turno atingido.\n";
        return false;
    }

    cout << "Planta '" << tipo << "' colocada em " << xy << "\n";
    return true;
}

bool Simulador::cmdLarga() {
    if (!jardineiro->getFerramentaNaMao()) { // se o jardineiro nao tiver ferramenta na mao, entra no if
        cout << "ERRO: O jardineiro nao tem ferramenta na mao.\n";
        return false;
    }
    cout << "Ferramenta '" << jardineiro->getFerramentaNaMao()->getRepresentacao() << "' colocada na mochila!\n"; // vai buscar a letra da ferramente que o jardinheiro tem na mao

    jardineiro->largarFerramenta(); // larga a ferramenta
    return true;
}

bool Simulador::cmdPega(const vector<string>& t) {
    if (t.size() != 2) { // se n tiver 2 argumentos, entra no if
        cout << "ERRO: pega <num_serie>\n";
        return false;
    }

    int n;
    try {
        n = stoi(t[1]); // tenta passar o valor da string para o inteiro 'n'
    } catch (...) {
        cout << "ERRO: Numero de ferramenta invalido.\n";
        return false;
    }

    /*if (!jardineiro->getEstaNoJardim()) { // se o jardineiro nao estiver no jardim, entra no if
        cout << "ERRO: O jardineiro nao está no jardim.\n";
        return false;
    }*/

    Ferramenta* naMao = jardineiro->getFerramentaNaMao(); // naMao fica a apontar para a ferramenta que o jardineiro tem na mao
    if (naMao && naMao->getNumeroSerie() == n) { // se a ferramenta existir e já estiver na mão do jardineiro
        cout << "ERRO: Essa ferramenta já se encontra na sua mão.\n";
        return false;
    }

    const auto& ferramentas = jardineiro->getFerramentas(); // 'ferramentas' fica com um array de ponteiros para ferramentas que estão na mocihla
    Ferramenta* ferr = nullptr; // inicializa 'ferr' a null

    for (Ferramenta* f : ferramentas) { // percorre cada ferramenta que está na mochila
        if (f->getNumeroSerie() == n) { // se encontrar lá a ferramenta com o n.º de série que o user escreveu, guarda-a em 'ferr'
            ferr = f;
            break;
        }
    }

    if (!ferr) { // se nao existir
        cout << "ERRO: Ferramenta nº " << n << " nao encontrada.\n";
        return false;
    }

    Ferramenta* antigaNaMao = jardineiro->getFerramentaNaMao(); // guarda em 'antigaNaMao' a ferramenta que tem na mão (nullptr se não tiver nd na mao) para apenas escrever que foi guardado na mochila

    bool ok = jardineiro->pegarFerramenta(n); // pega na ferramenta com o nº de serie 'n'
    if (!ok) { // se deu erro
        cout << "ERRO: Nao foi possivel pegar a ferramenta.\n";
        return false;
    }

    if (antigaNaMao) { // se ele tinha ja uma ferramenta na mao, entra no if
        cout << "Ferramenta '" << antigaNaMao->getRepresentacao() << "' (nº " << antigaNaMao->getNumeroSerie() << ") guardada na mochila.\n";
    }

    cout << "Ferramenta '" << ferr->getRepresentacao() << "' (nº " << n << ") já se encontra na mão!\n";

    return true;
}

bool Simulador::cmdCompra(const vector<string>& t) {
    if (t.size() != 2) { // valida se foi passado o tipo de ferramenta
        cout << "ERRO: compra <tipo>\n";
        return false;
    }

    /*if (!jardineiro->getEstaNoJardim()) { // cheka se o jardineiro está no jardim, se nao, nao deixa compra
        cout << "ERRO: O jardineiro nao esta no jardim.\n";
        return false;
    }*/

    char tipo = t[1][0]; // pega no tipo que está guardado em 't[1]'
    Ferramenta* nova = nullptr;

    if (tipo == 'g') nova = new Regador(); // caso seja um regador
    else if (tipo == 'a') nova = new Adubo(); // caso seja um adubo
    else if (tipo == 'z') nova = new FerramentaZ(); // caso seja uma ferramentaZ
    else if (tipo == 't') nova = new Tesoura(); // caso seja uma tesoura
    else { // caso nao exista esse tipo de ferramenta
        cout << "ERRO: Ferramenta invalida. Existem as seguintes ferramentas:";
        cout << "\n\tAdubo - a \n\tRegador - g \n\tFerramentaZ - z \n\tTesoura - t\n";
        return false;
    }

    jardineiro->adicionarFerramenta(nova); // adiciona a ferramenta à mochila
    cout << "Ferramenta comprada.\n";
    return true;
}

bool Simulador::cmdE(const vector<string>&) {
    jardineiro->resetFlagFerramentaApanhada(); // reseta a flag de que apanhou ferramenta no chao a false

    bool resultado = jardineiro->moverEsquerda(jardim); // chama o metodo moverEsquerda do jardineiro, que valida limites e efetua o movimento

    if (resultado && jardineiro->getApanheiFerramentaNesteMovimento()) { // se andou para a esquerda e apanhou uma ferramenta entra no if
        reporFerramentaAleatoria();
    }

    return resultado;
}

bool Simulador::cmdD(const vector<string>&) {
    jardineiro->resetFlagFerramentaApanhada(); // reseta a flag de que apanhou ferramenta no chao a false

    bool resultado = jardineiro->moverDireita(jardim); // chama o metodo moverDireita do jardineiro, que valida limites e efetua o movimento

    if (resultado && jardineiro->getApanheiFerramentaNesteMovimento()) { // se andou para a dirieta e apanhou uma ferramenta entra no if
        reporFerramentaAleatoria();
    }

    return resultado;
}

bool Simulador::cmdC(const vector<string>&) {
    jardineiro->resetFlagFerramentaApanhada(); // reseta a flag de que apanhou ferramenta no chao a false

    bool resultado = jardineiro->moverCima(jardim); // chama o metodo moverCima do jardineiro, que valida limites e efetua o movimento

    if (resultado && jardineiro->getApanheiFerramentaNesteMovimento()) { // se andou para cima e apanhou uma ferramenta entra no if
        reporFerramentaAleatoria();
    }

    return resultado;
}


bool Simulador::cmdB(const vector<string>&) {
    jardineiro->resetFlagFerramentaApanhada(); // reseta a flag de que apanhou ferramenta no chao a false

    bool resultado = jardineiro->moverBaixo(jardim); // chama o metodo moverBaixo do jardineiro, que valida limites e efetua o movimento

    if (resultado && jardineiro->getApanheiFerramentaNesteMovimento()) { // se andou para baixo e apanhou uma ferramenta entra no if
        reporFerramentaAleatoria();
    }

    return resultado;
}

bool Simulador::cmdEntra(const vector<string>& t) {
    if (t.size() != 2) { // valida se foram passadas as coordenadas
        cout << "ERRO: entra <l><c>\n";
        return false;
    }

    int l, c;
    if (!jardim->coordenadasParaIndice(t[1], l, c)) { // converte as coordenadas (t[1]) para índices numéricos('l' e 'c')
        cout << "ERRO: Coordenadas invalidas.\n";
        return false;
    }

    /*if (jardineiro->getEstaNoJardim()) { // apenas deixa entrar se ele não estiver no jardim
        cout << "AVISO: O jardineiro ja esta no jardim.\n";
        return false;
    }*/

    jardineiro->resetFlagFerramentaApanhada(); // reseta a flag de que apanhou ferramenta no chao a false, pois pode vir a apanha-la quando entrar

    bool ok = jardineiro->entrar(jardim, l, c); // tenta entrar na posicao

    if (ok) { // se entrou
        if (jardineiro->getApanheiFerramentaNesteMovimento()) { // se apanhou uma ferramenta ao entrar na posicao
            reporFerramentaAleatoria();
        }
    } else { // se nao entrou
        cout << "ERRO: Limite de entradas por turno atingido.\n";
        return false;
    }

    return true;
}

bool Simulador::cmdSai() {
    if (!jardineiro->getEstaNoJardim()) { // se nao estiver no jardim
        cout << "ERRO: O jardineiro nao esta no jardim.\n";
        return false;
    }

    bool ok = jardineiro->sair(); // tenta sair

    if (!ok) { // se nao sair
        cout << "ERRO: Limite de entradas/saidas por turno atingido.\n";
        return false;
    }

    cout << "Jardineiro saiu do jardim!\n";
    return true;
}

bool Simulador::cmdLPlantas() {
    cout << "=== LISTA DE TODAS AS PLANTAS ===\n";

    for (int l = 0; l < jardim->getNumLinhas(); l++) { // percorre todas as linhas
        for (int c = 0; c < jardim->getNumColunas(); c++) { // percorre todas as colunas

            Posicao* p = jardim->getPosicao(l, c); // pega cada posicao individual
            if (!p) continue; // se a posicao nao existir continua no ciclo para a prox posicao

            Planta* pl = p->getPlanta(); // pega na planta que está nesta posicao (se houver)
            if (pl) { // se houver planta na posicao
                char linha = 'a' + l; // passa o numero da linha para letra
                char coluna = 'a' + c; // passa o numero da coluna para letra

                cout << linha << coluna << " -> Planta '" << pl->getRepresentacao()
                     << "' | Água: " << pl->getAgua()
                     << " | Nutrientes: " << pl->getNutrientes()
                     << " | Idade: " << pl->getIdade()
                     << " | Solo(Á:" << p->getAgua()
                     << ", N:" << p->getNutrientes() << ")\n";
            }
        }
    }
    return true;
}

bool Simulador::cmdLPlanta(const vector<string>& args) {
    if (args.size() != 2) { // valida se foi passada a coordenada
        cout << "ERRO: lplanta <coord>\n";
        return false;
    }


    int l, c;
    if (!jardim->coordenadasParaIndice(args[1], l, c)) { // converte as coordenadas (args[1]) para índices numéricos('l' e 'c')
        cout << "ERRO: Coordenadas inválidas.\n";
        return false;
    }

    Posicao* pos = jardim->getPosicao(l, c); // pega na posicao com aquela 'l' e 'c'
    Planta* pl = pos->getPlanta(); // pega na planta que está na posicao (se nao houver: 'nullptr')

    if (!pl) { // se não houver planta nessa posicao
        cout << "Não existe planta em " << args[1] << ".\n";
        return true;
    }

    cout << "=== PLANTA EM " << args[1] << " ===\n";
    cout << "Tipo: " << pl->getRepresentacao() << "\n";
    cout << "Água interna: " << pl->getAgua() << "\n";
    cout << "Nutrientes internos: " << pl->getNutrientes() << "\n";
    cout << "Idade: " << pl->getIdade() << "\n";
    cout << "Solo -> Água: " << pos->getAgua() << ", Nutrientes: " << pos->getNutrientes() << "\n";

    return true;
}

bool Simulador::cmdLArea() {
    cout << "=== POSIÇÕES COM CONTEÚDO ===\n";

    for (int l = 0; l < jardim->getNumLinhas(); l++) { // percorre todas as linhas do jardim
        for (int c = 0; c < jardim->getNumColunas(); c++) { // percorre todas as colunas do jardim

            Posicao* pos = jardim->getPosicao(l, c); // 'pos' vai apontar para cada posicao
            bool temPlanta = pos->getPlanta() != nullptr; // true se tiver planta na posicao
            bool temFerr = pos->getFerramenta() != nullptr; // true se tiver feramenta posicao

            if (!temPlanta && !temFerr) // se nao tiver planta nem ferramenta, continua no loop
                continue;

            cout << (char)('a' + l) << (char)('a' + c) << " -> "; // passa 'l' e 'c' (inteiros) para coordenada em string/char

            if (temPlanta) // se tiver planta
                cout << "| Planta '" << pos->getPlanta()->getRepresentacao() << "' ";

            if (temFerr) // se tiver ferramenta
                cout << "| Ferramenta '" << pos->getFerramenta()->getRepresentacao() << "' ";

            cout << "| Solo (Água:" << pos->getAgua() << ", Nutrientes:" << pos->getNutrientes() << ")\n";
        }
    }
    return true;
}

bool Simulador::cmdLSolo(const vector<string>& args) {
    if (args.size() < 2 || args.size() > 3) { // valida se foram passados 2 ou 3 argumentos (coordenada obrigatória, raio opcional)
        cout << "ERRO: lsolo <coord> [raio]\n";
        return false;
    }

    int linha, coluna;
    if (!jardim->coordenadasParaIndice(args[1], linha, coluna)) { // converte as coordenadas (args[1]) para índices numéricos('l' e 'c')
        cout << "ERRO: Coordenadas inválidas.\n";
        return false;
    }

    int raio = 0; // inicializa raio a 0
    if (args.size() == 3) { // se tiver 3 argumentos (para pesquisar com raio) entra no if
        if (!lerInteiroSeguro(args[2], raio) || raio < 0) { // tenta guardar em 'raio' o valor do raio (args[2]). Se der erro ou se o raio for negativo, entra no if
            cout << "ERRO: Raio invalido.\n";
            return false;
        }
    }

    cout << "=== SOLO EM " << args[1] << " COM RAIO " << raio << " ===\n";

    for (int l = linha - raio; l <= linha + raio; l++) { // vai percorrer a sua linha + aquelas que o raio indica (e.g. 2 de raio: 2 linhas para cima + 2 linhas para baixo + a sua linha ainda)
        for (int c = coluna - raio; c <= coluna + raio; c++) { // vai percorrer a sua coluna + aquelas que o raio indica (e.g. 2 de raio: 2 colunas para a esquerda + 2 colunas para a direita + a sua coluna ainda)

            Posicao* pos = jardim->getPosicao(l, c); // 'pos' vai apontar para cada posicao
            if (!pos) continue; // se a posicao nao existir, continua para outro posicao no loop

            cout << (char)('a' + l) << (char)('a' + c) // passa 'l' e 'c' (inteiros) para coordenada em string/char
                 << " -> Água:" << pos->getAgua()
                 << ", Nutrientes:" << pos->getNutrientes();

            if (pos->getPlanta()) // se na posicao houver planta
                cout << ", Planta:" << pos->getPlanta()->getRepresentacao();

            if (pos->getFerramenta()) // se na posicao houver ferramenta
                cout << ", Ferramenta:" << pos->getFerramenta()->getRepresentacao();

            cout << "\n";
        }
    }

    return true;
}
bool Simulador::cmdLFerr() {
    cout << "=== FERRAMENTAS DO JARDINEIRO ===\n";

    cout << "Na mão: ";
    if (jardineiro->getFerramentaNaMao()) // se tiver ferramenta na mão entra no if
        cout << jardineiro->getFerramentaNaMao()->getRepresentacao()
             << " (nº " << jardineiro->getFerramentaNaMao()->getNumeroSerie() << ")\n";
    else // se nao, entra no else
        cout << "(nenhuma)\n";

    cout << "\nMochila:\n";
    const auto& ferr = jardineiro->getFerramentas(); // 'ferr' fica com um array de ponteiros para ferramentas que estão na mocihla
    if (ferr.empty()) // se a mochila estiver fazia
        cout << "(vazio)\n";

    for (auto f : ferr) {
        cout << "Ferramenta '" << f->getRepresentacao()
             << "' (nº " << f->getNumeroSerie() << ")\n";
    }
    return true;
}

Ferramenta* Simulador::criarFerramentaAleatoria() {
    int r = rand() % 4; // gera um número aleatório entre 0 e 3 e guarda-o em 'r'

    if (r == 0) return new Regador(); // 25%
    if (r == 1) return new Adubo(); // 25%
    if (r == 2) return new FerramentaZ(); // 25%
    return new Tesoura(); // 25%
}

void Simulador::reporFerramentaAleatoria() {
    if (!jardim) return; // verifica se o jardim existe, se nao, sai da funcao

    Ferramenta* nova = criarFerramentaAleatoria(); // cria a ferramenta nova aleatoriamente
    jardim->colocarFerramentaAleatoria(nova); // coloca a nova ferramenta no jardim

    cout << "Nova ferramenta '" << nova->getRepresentacao() << "' apareceu no jardim!\n";
}


bool Simulador::cmdGrava(const vector<string>& t) {
    if (t.size() != 2) { // verifica se foram passados exatamente 2 argumentos (comando + nome)
        cout << "Sintaxe: grava <nome>\n";
        return false;
    }

    auto it = gravacoes.find(t[1]); // 'it' fica automaticamente com o tipo da variável 'gravacoes'(std::map<std::string, GravaMem>). Procura na tabela a gravação pelo nome/id: t[1]. Se não existir, 'find' devolve exatamente 'gravacoes.end()'
    if (it != gravacoes.end()) { // se 'it' for DIFERENTE a 'gravacoes.end()' é porque ENCONTROU, logo, 'it' aponta para o elemento da tabela com o nome/id: t[1]
        cout << "Gravação já existente. Insira um nome diferente de gravação.\n";
        return false;
    }

    GravaMem g; // cria a struct 'GravaMem' para armazenar o estado do jardim
    g.linhas = jardim->getNumLinhas(); // guarda as linhas atuais do jardim na struct
    g.colunas = jardim->getNumColunas(); // guarda as colunas atuais do jardim na struct

    g.jardineiroNoJardim = jardineiro->getEstaNoJardim(); // se o jardineiro está no jardim ou não
    g.jl = jardineiro->getLinhaAtual(); // guarda a linha onde está o jardineiro
    g.jc = jardineiro->getColunaAtual(); // guarda a coluna onde está o jardineiro

    int total = g.linhas * g.colunas; // calcula o numero total de posicoes no jardim para criar o array de cada informacao para CADA posicao
    g.agua.resize(total);                // redimensiona o array para 'total' valores(posicoes)
    g.nutrientes.resize(total);          // redimensiona o array para 'total' valores(posicoes)
    g.plantas.resize(total);             // redimensiona o array para 'total' valores(posicoes)
    g.ferramentas.resize(total);         // redimensiona o array para 'total' valores(posicoes)

    int k = 0; // indice de cada array
    for (int l = 0; l < g.linhas; l++) { // percorre todas as linhas do jardim
        for (int c = 0; c < g.colunas; c++) { // percorre todas as colunas do jardim
            Posicao* p = jardim->getPosicao(l, c); // pega em cada posicao

            g.agua[k] = p->getAgua(); // guarda a agua que estava nesta posicao no valor do array
            g.nutrientes[k] = p->getNutrientes(); // guarda os nutrientes que estavam nesta posicao no valor do array
            g.plantas[k] = p->getPlanta() ? p->getPlanta()->getRepresentacao() : '-'; // se existir uma planta nesta posicao, guarda a letra da planta (e.g. 'c'-cacto), se nao houver, guarda '-'

            if (p->getFerramenta()) { // se houver ferramenta nesta posicao
                g.ferramentas[k] = p->getFerramenta()->getRepresentacao(); // guarda a letra da ferramenta
            } else { // se nao houver
                g.ferramentas[k] = '-'; // guarda '-'
            }

            k++;
        }
    }

    gravacoes[t[1]] = g; // guarda na tabela 'gravacoes': [t[1], g] | t[1] = nome/id da gravacao | g = struct 'GravaMem'
    cout << "O estado do jardim foi gravado em memoria (gravação: '" << t[1] << "'):\n";
    return true;
}

bool Simulador::cmdRecupera(const vector<string>& t) {
    if (t.size() != 2) { // verifica se foram passados exatamente 2 argumentos (comando + nome)
        cout << "Sintaxe: recupera <nome>\n";
        return false;
    }

    auto it = gravacoes.find(t[1]); // 'it' fica automaticamente com o tipo da variável 'gravacoes'(std::map<std::string, GravaMem>). Procura na tabela a gravação pelo nome/id: t[1]. Se não existir, 'find' devolve exatamente 'gravacoes.end()'
    if (it == gravacoes.end()) { // se it for exatamente igual a 'gravacoes.end()' é porque nao encontrou, caso contrario, 'it' apontaria para o elemento da tabela com o nome/id: t[1]
        cout << "Erro: gravação: '" << t[1] << "' inexistente.\n";
        return false;
    }

    GravaMem g = it->second; // copia a gravação encontrada para 'g'. it->second é a gravação que está na segunda posicao desta iteracao na tabela: ["nome/id - gravação(struct)"]
    gravacoes.erase(it); // remove-a da tabela/mapa

    if (jardim) delete jardim; // apaga o jardim atual (se existir) e cria um novo com as dimensões guardadas
    jardim = new Jardim(g.linhas, g.colunas);

    int k = 0; // indice de cada array
    for (int l = 0; l < g.linhas; l++) { // percorre todas as linhas do jardim da gravacao
        for (int c = 0; c < g.colunas; c++) { // percorre todas as colunas do jardim da gravacao
            Posicao* p = jardim->getPosicao(l, c); // pega em cada posicao

            p->adicionarAgua(g.agua[k] - p->getAgua()); // adiciona a diferença de água entre o que estava guardado no array e entre o que já está na posicao (isto, para cada posicao)
            p->adicionarNutrientes(g.nutrientes[k] - p->getNutrientes()); // adiciona a diferença de nutrientes entre o que estava guardado no array e entre o que já está na posicao (isto, para cada posicao)

            if (g.plantas[k] != '-') { // se houver planta nesta posicao (ser != de '-') -> restaura plantas
                Planta* nova = nullptr;
                if (g.plantas[k] == 'c') nova = new Cacto();
                if (g.plantas[k] == 'r') nova = new Roseira();
                if (g.plantas[k] == 'e') nova = new Ervadaninha();
                if (g.plantas[k] == 'x') nova = new PlantaExotica();
                p->colocarPlanta(nova); // adiciona a planta a esta posicao
            }

            if (g.ferramentas[k] != '-') { // se houver ferramenta nesta posicao (ser != de '-') -> restaura ferramentas
                Ferramenta* novaFerr = nullptr;
                if (g.ferramentas[k] == 'g') novaFerr = new Regador();
                else if (g.ferramentas[k] == 'a') novaFerr = new Adubo();
                else if (g.ferramentas[k] == 't') novaFerr = new Tesoura();
                else if (g.ferramentas[k] == 'z') novaFerr = new FerramentaZ();
                p->colocarFerramenta(novaFerr); // adiciona a ferramenta a esta posicao
            }

            k++;
        }
    }

    if (g.jardineiroNoJardim) { // se o jardineiro, na gravacao, estava no jardim
        jardineiro->sair(); // neste jardim (atual) sai (caso esteja)
        jardineiro->resetContadores(); // reseta os contadores
        jardineiro->entrar(jardim, g.jl, g.jc); // e volta a entrar na posicao que estava quando foi gravado
    }
    else { // se o jardineiro, na gravacao, não estava no jardim
        jardineiro->sair(); // sai agora (caso esteja ou nao) para garantir que nao fica dentro do jardim
    }

    cout << "Estado do jardim recuperado da memória (gravação '" << t[1] << "' apagada):\n";
    return true;
}


bool Simulador::cmdApaga(const vector<string>& t) {
    if (t.size() != 2) { // verifica se foram passados exatamente 2 argumentos (comando + nome)
        cout << "Sintaxe: apaga <nome>\n";
        return false;
    }

    if (gravacoes.erase(t[1]) == 0) { // apaga da tabela/mapa a gravaçao/iteração onde o nome/id da gravação for: t[1]. Se a gravação não existir, entra no if
        cout << "Erro: gravação: '" << t[1] << "' inexistente.\n";
        return false;
    }

    cout << "Gravação apagada.\n";
    return true;
}

