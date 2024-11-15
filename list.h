#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <iostream>
using namespace std;

template<typename T>
struct TElemento {
    T dado;
    TElemento<T>* proximo;
};

template<typename T>
struct TLista {
    TElemento<T>* inicio;
    TElemento<T>* fim;
};

// Inicializa a lista
template<typename T>
bool inicializar_lista(TLista<T>& lista) {
    lista.inicio = NULL;
    lista.fim = NULL;
    return true;
}

// Cria um novo elemento
template<typename T>
TElemento<T>* novo_elemento(T dado) {
    TElemento<T>* novo = new TElemento<T>;
    novo->dado = dado;
    novo->proximo = NULL;
    return novo;
}

// Insere um elemento no fim da lista
template<typename T>
bool insere_fim(TLista<T>& lista, T dado) {
    TElemento<T>* novo = novo_elemento(dado);
    if (lista.inicio == NULL) {
        lista.inicio = novo;
        lista.fim = novo;
    } else {
        lista.fim->proximo = novo;
        lista.fim = novo;
    }
    return true;
}

// Remove um elemento da lista
template<typename T>
bool remover(TLista<T>& lista, T dado) {
    TElemento<T>* nav = lista.inicio;
    TElemento<T>* ant = NULL;
    if (lista.inicio == NULL) {
        return false;
    }

    // Caso o elemento esteja no início
    if (lista.inicio->dado.name == dado.name) {
        TElemento<T>* temp = lista.inicio;
        lista.inicio = lista.inicio->proximo;
        if (lista.inicio == NULL) {
            lista.fim = NULL;
        }
        delete temp;
        return true;
    }

    // Procura o elemento no meio ou fim da lista
    while (nav != NULL && nav->dado.name != dado.name) {
        ant = nav;
        nav = nav->proximo;
    }

    // Se o elemento não foi encontrado
    if (nav == NULL) {
        return false;
    }

    // Remove o elemento
    ant->proximo = nav->proximo;
    if (nav == lista.fim) {
        lista.fim = ant;
    }
    delete nav;
    return true;
}

template<typename T>
void vizualizar_lista(TLista<T> lista) {
    TElemento<T>* nav = lista.inicio;
    if (nav == NULL) {
        cout << "Nenhum filme cadastrado nesta posicao." << endl;
        return;
    }
    while (nav != NULL) {
        // Exibe todas as informações do filme
        cout << "ID: " << nav->dado.id << endl;
        cout << "Nome: " << nav->dado.name << endl;
        cout << "Categorias: " << nav->dado.categories << endl;
        cout << "Atores: " << nav->dado.actors << endl;
        cout << "Diretor: " << nav->dado.director << endl;
        cout << "Alugado: " << (nav->dado.rented ? "Sim" : "Não") << endl;
        cout << "-------------------------" << endl;
        nav = nav->proximo; // Move para o próximo elemento da lista
    }
}

#endif // LIST_H_INCLUDED
