#ifndef MOVIE_H_INCLUDED
#define MOVIE_H_INCLUDED

#include <string>
using namespace std;

// Estrutura para armazenar informa��es de aluguel
struct rental_info {
    string renter;   // Nome da pessoa que alugou o filme
    string due_date; // Data de devolu��o
    bool is_rented;  // Status do filme (alugado ou n�o)
};

// Estrutura para armazenar informa��es de filmes
struct movie {
    int id;
    string name;
    string categories;
    string actors;
    string director;
    bool rented;     // Campo que indica se o filme est� alugado (antigo)
    rental_info rental;  // Novo campo com informa��es detalhadas de aluguel
};

#endif // MOVIE_H_INCLUDED
