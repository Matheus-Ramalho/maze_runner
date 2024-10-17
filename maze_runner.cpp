#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>

using namespace std;

// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;
std::stack<Position> valid_positions;

// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Erro: Falha ao abrir o arquivo: " << file_name << std::endl;
        return {-1, -1};
    }
    // Lê o número de linhas e colunas do arquivo
    if (!(file >> num_rows >> num_cols)) {
        std::cerr << "Erro: Não encontrado número de linhas e colunas no arquivo." << std::endl;
        return {-1, -1};
    }
    maze.resize(num_rows, std::vector<char>(num_cols));
    Position start_pos = {-1, -1}; // Inicializa a posição de início como inválida
    char caracter;
    // Preenche a matriz com os caracteres do arquivo
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            if (!(file >> caracter)) {
                std::cerr << "Erro: Formato de texto invalido." << std::endl;
                return {-1, -1};
            }
            maze[i][j] = caracter;
            if (caracter == 'e') {
                start_pos = {i, j};
            }
        }
    }
    file.close();
    return {start_pos.row,start_pos.col};
}

// Função para imprimir o labirinto
void print_maze() {
    std::cout << "\033[2J\033[H";
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            std::cout << maze[i][j] << ' ';
        }
        std::cout << '\n';
    }
    return;
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    if(row >= 0 && row < num_rows && col >= 0 && col < num_cols){
        if(maze[row][col] == 'x' || maze[row][col] == 's'){
            return true;
        }
    }
    return false;
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    if(maze[pos.row][pos.col] == 's'){
        return true;
    }
    maze[pos.row][pos.col] = '.';
    print_maze();
    //std::cout << "l " << pos.row << "c " << pos.col << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if(!valid_positions.empty()){//Remove a posição atual (Recursivamente chamada por ser a top) da pilha, ja foi verificada. Se não for vazio.
        valid_positions.pop();
    }
    
    if(is_valid_position(pos.row+1,pos.col)){       //Cima
        valid_positions.push({pos.row+1,pos.col});
    }
        if(is_valid_position(pos.row,pos.col-1)){ //Esquerda
        valid_positions.push({pos.row,pos.col-1});
    }
    if(is_valid_position(pos.row,pos.col+1)){       //Direita
        valid_positions.push({pos.row,pos.col+1});
    }
    if(is_valid_position(pos.row-1,pos.col)){       //Baixo
        valid_positions.push({pos.row-1,pos.col});
    }


    if(valid_positions.empty()){
           return false; 
    }

    if(walk(valid_positions.top())){
        return true;
    }else{
        return false;
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_labirinto>" << std::endl;
        return 1;
    }

    Position initial_pos = load_maze(argv[1]);
    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    bool exit_found = walk(initial_pos);

    if (exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Não foi possível encontrar a saída." << std::endl;
    }

    return 0;
}

// Nota sobre o uso de std::this_thread::sleep_for:
// 
// A função std::this_thread::sleep_for é parte da biblioteca <thread> do C++11 e posteriores.
// Ela permite que você pause a execução do thread atual por um período especificado.
// 
// Para usar std::this_thread::sleep_for, você precisa:
// 1. Incluir as bibliotecas <thread> e <chrono>
// 2. Usar o namespace std::chrono para as unidades de tempo
// 
// Exemplo de uso:
// std::this_thread::sleep_for(std::chrono::milliseconds(50));
// 
// Isso pausará a execução por 50 milissegundos.
// 
// Você pode ajustar o tempo de pausa conforme necessário para uma melhor visualização
// do processo de exploração do labirinto.