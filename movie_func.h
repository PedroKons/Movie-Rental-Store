#ifndef MOVIE_FUNC_H_INCLUDED
#define MOVIE_FUNC_H_INCLUDED

#include <fstream>
#include "th.h"
#include <string>
#include "json.hpp"
#include <ctime>
#include <cstdlib>
using json = nlohmann::json;
using namespace std;

void menu(){
    cout << "------------LOCADORA------------" << endl;
    cout << endl << "Menu Principal" << endl << endl;
    cout << "1- Cadastrar Filme" << endl;
    cout << "2- Atualizar Filme" << endl;
    cout << "3- Remover Filme" << endl;
    cout << "4- Sistema de aluguel" << endl;
    cout << "5- Listagem por ator ou diretor" << endl;
    cout << "6- Listagem por categoria" << endl;
    cout << "7- Listagem de filmes com atraso de devolucao" << endl;
    cout << "8- Historicos de retiradas" << endl;
    cout << "9- Sair" << endl;
}

void register_movies(tabela tableHash[], const string& fileName) {
    movie newMovie;

    cout << "Digite o ID do filme: ";
    cin >> newMovie.id;

    cin.ignore(); // Limpar o buffer para evitar problemas com getline

    cout << "Digite o nome do filme: ";
    getline(cin, newMovie.name);

    cout << "Digite a categoria do filme: ";
    getline(cin, newMovie.categories);

    cout << "Digite os atores principais: ";
    getline(cin, newMovie.actors);

    cout << "Digite o diretor do filme: ";
    getline(cin, newMovie.director);

    // Inicializa o campo de aluguel
    newMovie.rental.is_rented = false;
    newMovie.rental.renter = "";
    newMovie.rental.due_date = "";

    // Inserir o filme na tabela hash
    inserir(tableHash, newMovie);

    // Ler o arquivo JSON
    json moviesJson;
    std::ifstream file(fileName);
    if (file.is_open()) {
        file >> moviesJson;
        file.close();
    }

    // Adicionar o novo filme no JSON
    json newMovieJson = {
        {"id", newMovie.id},
        {"name", newMovie.name},
        {"categories", newMovie.categories},
        {"actors", newMovie.actors},
        {"director", newMovie.director},
        {"rental", {
            {"renter", newMovie.rental.renter},
            {"due_date", newMovie.rental.due_date},
            {"is_rented", newMovie.rental.is_rented}
        }}
    };

    moviesJson["movies"].push_back(newMovieJson);

    // Salvar o JSON atualizado
    std::ofstream outFile(fileName);
    outFile << moviesJson.dump(4); // 4 espaços de indentação para facilitar a leitura
    outFile.close();

    cout << "Filme cadastrado e adicionado ao arquivo JSON com sucesso!" << endl;
}

void update_to_movie(tabela tableHash[], const string& fileName) {
    int id;
    cout << "Digite o ID do filme que deseja atualizar: ";
    cin >> id;

    TElemento<movie>* filmeEncontrado = buscarElemento(tableHash, id);
    if (filmeEncontrado != NULL) {
        movie& movie_current = filmeEncontrado->dado;  // Referência ao filme encontrado
        cout << "Filme encontrado: " << endl;
        cout << "Nome: " << movie_current.name << endl;
        cout << "Categorias: " << movie_current.categories << endl;
        cout << "Atores: " << movie_current.actors << endl;
        cout << "Diretor: " << movie_current.director << endl;
        cout << "Alugado: " << (movie_current.rental.is_rented ? "Sim" : "Nao") << endl;

        cin.ignore(); // Limpar o buffer

        cout << "Digite o novo nome do filme (ou pressione Enter para manter o nome atual): ";
        string new_name;
        getline(cin, new_name);
        if (!new_name.empty()) {
            movie_current.name = new_name;
        }

        cout << "Digite as novas categorias (ou pressione Enter para manter as categorias atuais): ";
        string new_categories;
        getline(cin, new_categories);
        if (!new_categories.empty()) {
            movie_current.categories = new_categories;
        }

        cout << "Digite os novos atores (ou pressione Enter para manter os atores atuais): ";
        string new_actors;
        getline(cin, new_actors);
        if (!new_actors.empty()) {
            movie_current.actors = new_actors;
        }

        cout << "Digite o novo diretor (ou pressione Enter para manter o diretor atual): ";
        string new_director;
        getline(cin, new_director);
        if (!new_director.empty()) {
            movie_current.director = new_director;
        }

        // Atualizar o arquivo JSON
        json moviesJson;
        std::ifstream file(fileName);
        if (file.is_open()) {
            file >> moviesJson;
            file.close();
        }

        // Procurar e atualizar o filme no JSON
        for (auto& item : moviesJson["movies"]) {
            if (item["id"].get<int>() == id) {
                item["name"] = movie_current.name;
                item["categories"] = movie_current.categories;
                item["actors"] = movie_current.actors;
                item["director"] = movie_current.director;

                // Mantém o status de aluguel
                item["rental"]["renter"] = movie_current.rental.renter;
                item["rental"]["due_date"] = movie_current.rental.due_date;
                item["rental"]["is_rented"] = movie_current.rental.is_rented;
                break;
            }
        }

        // Salvar o JSON atualizado
        std::ofstream outFile(fileName);
        outFile << moviesJson.dump(4);
        outFile.close();

        cout << "Filme atualizado e alterado no arquivo JSON com sucesso!" << endl;
    } else {
        cout << "Filme com ID " << id << " nao encontrado!" << endl;
    }
}

void remove_to_movie(tabela tableHash[], const string& fileName) {
    int id;
    cout << "Digite o ID do filme que deseja remover: ";
    cin >> id;

    remover(tableHash, id);  // Remove o filme da tabela

    // Atualizar o arquivo JSON
    json moviesJson;
    std::ifstream file(fileName);
    if (file.is_open()) {
        file >> moviesJson;
        file.close();
    }

    // Procurar e remover o filme do JSON
    for (auto it = moviesJson["movies"].begin(); it != moviesJson["movies"].end(); ++it) {
        if ((*it)["id"].get<int>() == id) {
            moviesJson["movies"].erase(it);  // Remove o filme encontrado no JSON
            break;
        }
    }

    // Salvar o JSON atualizado
    std::ofstream outFile(fileName);
    outFile << moviesJson.dump(4);
    outFile.close();

    cout << "Filme removido do sistema e do arquivo JSON com sucesso!" << endl;
}

void readJsonAndInsertIntoTable(tabela tableHash[], const string& fileName) {
    // Abre o arquivo JSON
    std::ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Nao foi possível abrir o arquivo JSON." << endl;
        return;
    }

    // Lê o conteúdo do arquivo para um objeto JSON
    json moviesJson;
    file >> moviesJson;

    // Acessa o array de filmes sob a chave "movies"
    for (const auto& item : moviesJson["movies"]) { //obs: auto se molda dependendo do tipo da variavel
        movie newMovie;
        newMovie.id = item["id"].get<int>();
        newMovie.name = item["name"].get<string>();
        newMovie.categories = item["categories"].get<string>();
        newMovie.actors = item["actors"].get<string>();
        newMovie.director = item["director"].get<string>();

        // Carregar as informações de aluguel
        newMovie.rental.renter = item["rental"]["renter"].get<string>();
        newMovie.rental.due_date = item["rental"]["due_date"].get<string>();
        newMovie.rental.is_rented = item["rental"]["is_rented"].get<bool>();

        // Inserir o filme na tabela hash
        inserir(tableHash, newMovie);
    }

    cout << "Filmes carregados com sucesso!" << endl;
}

void list_by_actor_or_director(tabela tableHash[], const string& searchType) {
    string searchQuery;

    // Solicita ao usuário que informe o nome do ator ou diretor para busca
    cout << "Digite o nome do " << searchType << ": ";
    cin.ignore(); // Limpa o buffer do cin
    getline(cin, searchQuery);

    bool found = false;
    cout << endl << "Filmes encontrados para o " << searchType << " '" << searchQuery << "':" << endl;

    // Percorre toda a tabela hash
    for (int i = 0; i < TAM; i++) {
        TElemento<movie>* nav = tableHash[i].listaFilmes.inicio;

        while (nav != NULL) {
            // Verifica se a busca é por ator ou diretor e se há correspondência
            if ((searchType == "ator" && nav->dado.actors.find(searchQuery) != string::npos) || //string::npos retorno do find se nao encontrado
                (searchType == "diretor" && nav->dado.director.find(searchQuery) != string::npos)) {
                cout << "ID: " << nav->dado.id << endl;
                cout << "Nome: " << nav->dado.name << endl;
                cout << "Categorias: " << nav->dado.categories << endl;
                cout << "Atores: " << nav->dado.actors << endl;
                cout << "Diretor: " << nav->dado.director << endl;
                cout << "Alugado: " << (nav->dado.rented ? "Sim" : "Não") << endl;
                cout << "-------------------------" << endl;
                found = true;
            }
            nav = nav->proximo;
        }
    }

    if (!found) {
        cout << "Nenhum filme encontrado para o " << searchType << " '" << searchQuery << "'." << endl;
    }
}

void list_by_category(tabela tableHash[]) {
    string searchCategory;

    // Solicita ao usuário que informe a categoria
    cout << "Digite a categoria do filme: ";
    cin.ignore(); // Limpa o buffer do cin
    getline(cin, searchCategory);

    bool found = false;
    cout << endl << "Filmes encontrados na categoria '" << searchCategory << "':" << endl;

    // Percorre toda a tabela hash
    for (int i = 0; i < TAM; i++) {
        TElemento<movie>* nav = tableHash[i].listaFilmes.inicio;

        while (nav != NULL) {
            // Verifica se a categoria do filme contém a categoria informada
            if (nav->dado.categories.find(searchCategory) != string::npos) {
                cout << "ID: " << nav->dado.id << endl;
                cout << "Nome: " << nav->dado.name << endl;
                cout << "Categorias: " << nav->dado.categories << endl;
                cout << "Atores: " << nav->dado.actors << endl;
                cout << "Diretor: " << nav->dado.director << endl;
                cout << "Alugado: " << (nav->dado.rented ? "Sim" : "Não") << endl;
                cout << "-------------------------" << endl;
                found = true;
            }
            nav = nav->proximo;
        }
    }

    if (!found) {
        cout << "Nenhum filme encontrado na categoria '" << searchCategory << "'." << endl;
    }
}

void add_to_rental_history(const movie& rented_movie, const string& fileName) {
    // Abrir o arquivo JSON do histórico de aluguéis
    json historyJson;
    std::ifstream file(fileName);
    if (file.is_open()) {
        file >> historyJson;
        file.close();
    }

    // Adicionar o filme alugado ao histórico
    json newEntry = {
        {"id", rented_movie.id},
        {"name", rented_movie.name},
        {"renter", rented_movie.rental.renter},
        {"due_date", rented_movie.rental.due_date},
        {"rental_date", ""}  // Pode adicionar um campo com a data de aluguel se necessário
    };

    historyJson["rental_history"].push_back(newEntry);

    // Salvar o arquivo JSON atualizado
    std::ofstream outFile(fileName);
    outFile << historyJson.dump(4);  // 4 espaços de indentação
    outFile.close();

    cout << "Aluguel do filme '" << rented_movie.name << "' adicionado ao histórico!" << endl;
}

void rent_movie(tabela tableHash[], const string& fileName, const string& historyFileName) {
    int id;
    cout << "Digite o ID do filme que deseja alugar: ";
    cin >> id;

    TElemento<movie>* filmeEncontrado = buscarElemento(tableHash, id);
    if (filmeEncontrado != NULL) {
        movie& movie_current = filmeEncontrado->dado;  // Referência ao filme encontrado

        if (movie_current.rental.is_rented) {
            cout << "Este filme já está alugado por " << movie_current.rental.renter << "." << endl;
        } else {
            // Solicitar informações de quem vai alugar
            cout << "Digite o nome da pessoa que está alugando o filme: ";
            cin.ignore(); // Limpa o buffer do cin
            getline(cin, movie_current.rental.renter);

            cout << "Digite a data de devolução (formato DD/MM/AAAA): ";
            getline(cin, movie_current.rental.due_date);

            // Atualiza o status do filme para alugado
            movie_current.rental.is_rented = true;

            // Atualizar o arquivo JSON com os dados de aluguel
            json moviesJson;
            std::ifstream file(fileName);
            if (file.is_open()) {
                file >> moviesJson;
                file.close();
            }

            // Procurar e atualizar o filme no JSON
            for (auto& item : moviesJson["movies"]) {
                if (item["id"].get<int>() == id) {
                    item["rental"]["renter"] = movie_current.rental.renter;
                    item["rental"]["due_date"] = movie_current.rental.due_date;
                    item["rental"]["is_rented"] = movie_current.rental.is_rented;
                    break;
                }
            }

            // Salvar o JSON atualizado
            std::ofstream outFile(fileName);
            outFile << moviesJson.dump(4);
            outFile.close();

            // Adicionar o filme ao histórico de aluguéis
            add_to_rental_history(movie_current, historyFileName);

            cout << "Filme '" << movie_current.name << "' alugado com sucesso para " << movie_current.rental.renter << "!" << endl;
        }
    } else {
        cout << "Filme com ID " << id << " não encontrado!" << endl;
    }
}

void return_movie(tabela tableHash[], const string& fileName) {
    int id;
    cout << "Digite o ID do filme que deseja devolver: ";
    cin >> id;

    TElemento<movie>* filmeEncontrado = buscarElemento(tableHash, id);
    if (filmeEncontrado != NULL) {
        movie& movie_current = filmeEncontrado->dado;  // Referência ao filme encontrado

        if (movie_current.rental.is_rented) {
            // Devolver o filme
            movie_current.rental.is_rented = false;
            cout << "Filme '" << movie_current.name << "' devolvido por " << movie_current.rental.renter << "!" << endl;

            // Limpa os dados de aluguel
            movie_current.rental.renter = "";
            movie_current.rental.due_date = "";

            // Atualizar o arquivo JSON
            json moviesJson;
            std::ifstream file(fileName);
            if (file.is_open()) {
                file >> moviesJson;
                file.close();
            }

            // Procurar e atualizar o filme no JSON
            for (auto& item : moviesJson["movies"]) {
                if (item["id"].get<int>() == id) {
                    item["rental"]["renter"] = movie_current.rental.renter;
                    item["rental"]["due_date"] = movie_current.rental.due_date;
                    item["rental"]["is_rented"] = movie_current.rental.is_rented;
                    break;
                }
            }

            // Salvar o JSON atualizado
            std::ofstream outFile(fileName);
            outFile << moviesJson.dump(4);
            outFile.close();

        } else {
            cout << "Este filme não está alugado no momento." << endl;
        }
    } else {
        cout << "Filme com ID " << id << " não encontrado!" << endl;
    }
}

// Função para comparar se a data de devolução já passou
bool is_overdue(const string& due_date) {
    int due_day, due_month, due_year;

    // Extrai o dia, mês e ano da string de data
    sscanf(due_date.c_str(), "%d/%d/%d", &due_day, &due_month, &due_year);

    // Obtém a data atual
    time_t t = time(0);
    struct tm* now = localtime(&t);

    int current_day = now->tm_mday;
    int current_month = now->tm_mon + 1; // Janeiro é mês 0, por isso somamos 1
    int current_year = now->tm_year + 1900; // O ano retornado é o número de anos desde 1900, então somamos 1900

    // Verifica se a data atual é posterior à data de devolução
    if (current_year > due_year) {
        return true;
    } else if (current_year == due_year) {
        if (current_month > due_month) {
            return true;
        } else if (current_month == due_month) {
            if (current_day > due_day) {
                return true;
            }
        }
    }

    return false;
}

void list_overdue_movies(tabela tableHash[]) {
    bool found = false;
    cout << "Lista de filmes com devolução em atraso:" << endl;

    // Percorre toda a tabela hash
    for (int i = 0; i < TAM; i++) {
        TElemento<movie>* nav = tableHash[i].listaFilmes.inicio;

        while (nav != NULL) {
            // Se o filme está alugado e a data de devolução já passou
            if (nav->dado.rental.is_rented && is_overdue(nav->dado.rental.due_date)) {
                cout << "Filme: " << nav->dado.name << endl;
                cout << "Alugado por: " << nav->dado.rental.renter << endl;
                cout << "Data de devolução: " << nav->dado.rental.due_date << endl;
                cout << "-------------------------" << endl;
                found = true;
            }
            nav = nav->proximo;
        }
    }

    if (!found) {
        cout << "Nenhum filme com atraso de devolução encontrado." << endl;
    }
}

void list_rental_history(const string& historyFileName) {
    // Abrir o arquivo JSON do histórico de aluguéis
    json historyJson;
    std::ifstream file(historyFileName);
    if (!file.is_open()) {
        cout << "Não foi possível abrir o arquivo de histórico." << endl;
        return;
    }

    file >> historyJson;
    file.close();

    // Verificar se há histórico
    if (historyJson["rental_history"].empty()) {
        cout << "Nenhum histórico de aluguel encontrado." << endl;
        return;
    }

    // Exibir o histórico de aluguéis
    cout << "Histórico de filmes alugados:" << endl;
    for (const auto& entry : historyJson["rental_history"]) {
        cout << "ID do Filme: " << entry["id"] << endl;
        cout << "Nome: " << entry["name"] << endl;
        cout << "Alugado por: " << entry["renter"] << endl;
        cout << "Data de devolução: " << entry["due_date"] << endl;
        cout << "-------------------------" << endl;
    }
}

void clear_view(){
    system("cls");
}
void start_enginner() {

    tabela tableHash[TAM];
    inicializarTabela(tableHash);

    string fileName = "database.json";

    readJsonAndInsertIntoTable(tableHash, fileName);

    int option;
    bool quit = false;

    while (!quit) {
        menu();
        cout << "Qual a opcao que você deseja: ";
        cin >> option;

        switch (option) {
            case 1:
                // Cadastrar Filme
                clear_view();
                cout << "Cadastro de Filmes" << endl;
                register_movies(tableHash, fileName);
                break;

            case 2:
                // Atualizar Filme
                clear_view();
                cout << "Atualizar Filme" << endl;
                update_to_movie(tableHash, fileName);
                break;

            case 3:
                // Remover Filme
                clear_view();
                cout << "Remover Filme" << endl;
                remove_to_movie(tableHash, fileName);
                break;

            case 4:
                // Sistema de aluguel
                clear_view();
                int aluguelOption;
                cout << "1. Alugar Filme" << endl;
                cout << "2. Devolver Filme" << endl;
                cout << "Escolha uma opcao: ";
                cin >> aluguelOption;
                if (aluguelOption == 1) {
                    rent_movie(tableHash, fileName, "rental_history.json");  // Adicionar o nome do arquivo de histórico
                } else if (aluguelOption == 2) {
                    return_movie(tableHash, fileName);
                } else {
                    cout << "Opcao invalida." << endl;
                }
                break;

            case 5:
                // Listagem por ator ou diretor
                clear_view();
                int subOption;
                cout << "Deseja buscar por:\n1. Ator\n2. Diretor\nEscolha uma opcao: ";
                cin >> subOption;
                if (subOption == 1) {
                    list_by_actor_or_director(tableHash, "ator");
                } else if (subOption == 2) {
                    list_by_actor_or_director(tableHash, "diretor");
                } else {
                    cout << "Opcao invalida." << endl;
                }
                break;

            case 6:
                // Listagem por categoria
                clear_view();
                cout << "Listagem por categoria" << endl;
                list_by_category(tableHash);
                break;

            case 7:
                // Listagem por atraso de devolução
                clear_view();
                cout << "Listagem de filmes com atraso de devolucao" << endl;
                list_overdue_movies(tableHash);
                break;

            case 8:
                // Historicos de retiradas
                clear_view();
                cout << "Historicos de retiradas" << endl;
                list_rental_history("rental_history.json");
                break;

            case 9:
                // Sair
                clear_view();
                cout << "Saindo do sistema..." << endl;
                quit = true;
                break;

            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    }
}

#endif // MOVIE_FUNC_H_INCLUDED
