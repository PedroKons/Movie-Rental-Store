#ifndef TH_H_INCLUDED
#define TH_H_INCLUDED

#include "movie.h"  // Inclui o arquivo da estrutura movie
#include "list.h"   // Inclui o arquivo da lista encadeada
#include <iostream>
#include <string>
using namespace std;

const int TAM = 8;

// Estrutura da tabela hash que armazena uma lista de filmes
struct tabela {
    TLista<movie> listaFilmes;  // Lista encadeada de filmes
};

// Fun��o para inicializar a tabela
void inicializarTabela(tabela tab[]) {
    for (int i = 0; i < TAM; i++) {
        inicializar_lista(tab[i].listaFilmes);  // Inicializa uma lista em cada posi��o da tabela
    }
}

// Fun��o de hashing padr�o utilizando o id do filme como chave
int funcaoDeHashing(int id) {
    return id % TAM;
}

// Fun��o para inserir um filme na tabela hash
void inserir(tabela tab[], const movie& filme) {
    int posicao = funcaoDeHashing(filme.id);

    // Insere o filme no fim da lista encadeada da posi��o calculada
    insere_fim(tab[posicao].listaFilmes, filme);

    cout << "Filme " << filme.name << " inserido na posicao " << posicao << endl;
}

// Fun��o para buscar um filme pelo id
TElemento<movie>* buscarElemento(tabela tab[], int id) {
    int posicao = funcaoDeHashing(id);
    TElemento<movie>* nav = tab[posicao].listaFilmes.inicio;

    // Percorre a lista encadeada para procurar o filme com o ID correspondente
    while (nav != NULL) {
        if (nav->dado.id == id) {
            cout << "Filme " << nav->dado.name << " encontrado na posicao " << posicao << endl;
            return nav;  // Retorna o ponteiro para o elemento encontrado
        }
        nav = nav->proximo;
    }

    cout << "Filme com ID " << id << " nao encontrado." << endl;
    return NULL;  // Retorna NULL se o filme n�o for encontrado
}

// Fun��o para remover um filme pelo id
void remover(tabela tab[], int id) {
    TElemento<movie>* filmeEncontrado = buscarElemento(tab, id);
    if (filmeEncontrado != NULL) {
        remover(tab[funcaoDeHashing(id)].listaFilmes, filmeEncontrado->dado);  // Remove o filme da lista
        cout << "Filme removido com sucesso!" << endl;
    }
}

void imprimir(tabela tab[]) {
    cout << endl << "Tabela hashing imprimindo filmes..." << endl;
    for (int i = 0; i < TAM; i++) {
        cout << "Posi��o " << i << ": ";
        if (tab[i].listaFilmes.inicio == NULL) {
            cout << "Nenhum filme" << endl;
        } else {
            vizualizar_lista(tab[i].listaFilmes);  // Exibe os filmes na lista encadeada
            cout << endl;
        }
    }
}

#endif // TH_H_INCLUDED
