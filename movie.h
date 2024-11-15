#ifndef MOVIE_H_INCLUDED
#define MOVIE_H_INCLUDED

#include <string>
using namespace std;

// Estrutura para armazenar informações de aluguel
struct rental_info {
    string renter;   // Nome da pessoa que alugou o filme
    string due_date; // Data de devolução
    bool is_rented;  // Status do filme (alugado ou não)
};

// Estrutura para armazenar informações de filmes
struct movie {
    int id;
    string name;
    string categories;
    string actors;
    string director;
    bool rented;     // Campo que indica se o filme está alugado (antigo)
    rental_info rental;  // Novo campo com informações detalhadas de aluguel
};

#endif // MOVIE_H_INCLUDED
