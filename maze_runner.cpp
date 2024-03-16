#include <stdio.h>
#include <stack>
#include <fstream>
#include <string>
#include <iostream>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	std::ifstream file(file_name);
	std::string fline;
	std::getline(file, fline);
	int index = fline.find(" ");
	std::string str = fline.substr(0,index);
	std::string str1 = fline.substr(index, fline.size());
	num_rows = stoi(str);
	num_cols = stoi(str1);
	maze = new char*[num_rows];
	for (int i = 0; i < num_rows; ++i){
    	maze[i] = new char[num_cols];
	}
	// Abre o arquivo para leitura (fopen)
	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols

	// Aloca a matriz maze (malloc)
	for (int j = 0; j < num_rows; ++j){
		std::string line;
		std::getline(file, line);
		int index=0;
		for(std::string::iterator it = line.begin(); it != line.end(); ++it) {
			maze[j][index]=*it;
			index++;
		}
	}
	file.close();
	
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			if(maze[i][j] == 'e'){
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

bool check_for_exit(pos_t pos){
	if(maze[pos.i][pos.j] == 's'){
		return true;
	}
	return false;
}

bool check_for_valid_pos(pos_t pos){
	pos_t next_pos;
	if( pos.i+2 <= num_rows && (maze[pos.i+1][pos.j]=='x' || maze[pos.i+1][pos.j]=='s')){
		next_pos.i = pos.i + 1;
		next_pos.j = pos.j;
		if(check_for_exit(next_pos)){
			valid_positions.push(next_pos);
			return true;
		}
		valid_positions.push(next_pos);
	}
	if(pos.i-1 >= 0 && (maze[pos.i-1][pos.j]=='x' || maze[pos.i-1][pos.j]=='s')){
		next_pos.i = pos.i - 1;
		next_pos.j = pos.j;
		if(check_for_exit(next_pos)){
			valid_positions.push(next_pos);
			return true;
		}
		valid_positions.push(next_pos);
	}
	if(pos.j + 2 <= num_cols && (maze[pos.i][pos.j + 1]=='x' || maze[pos.i][pos.j + 1]=='s')){
		next_pos.i = pos.i;
		next_pos.j = pos.j + 1;
		if(check_for_exit(next_pos)){
			valid_positions.push(next_pos);
			return true;
		}
		valid_positions.push(next_pos);
	}
	if(pos.j -1 >= 0  && (maze[pos.i][pos.j - 1]=='x' || maze[pos.i][pos.j - 1]=='s')){
		next_pos.i = pos.i;
		next_pos.j = pos.j - 1;
		if(check_for_exit(next_pos)){
			valid_positions.push(next_pos);
			return true;
		}
		valid_positions.push(next_pos);
	}
	return false;
	
}
// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	pos_t cur_pos = pos;
	pos_t next_pos;
	check_for_valid_pos(cur_pos);
	next_pos = valid_positions.top();
	valid_positions.pop();
	maze[cur_pos.i][cur_pos.j] = 'o';
	maze[next_pos.i][next_pos.j] = '.';
	while (true){
		if (!valid_positions.empty()) {
			maze[cur_pos.i][cur_pos.j] = '.';
			maze[next_pos.i][next_pos.j] = 'o';
			cur_pos = next_pos;
			next_pos = valid_positions.top();
			valid_positions.pop();
			print_maze();
		}
		if(check_for_valid_pos(next_pos)){
			maze[cur_pos.i][cur_pos.j] = '.';
			maze[next_pos.i][next_pos.j] = 'o';
			print_maze();
			cur_pos = next_pos;
			next_pos = valid_positions.top();
			maze[next_pos.i][next_pos.j] = 'o';
			maze[cur_pos.i][cur_pos.j] = '.';
			valid_positions.pop();
			print_maze();
			return true;
		}
		
	}
	
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
		// Marcar a posição atual com o símbolo '.'
		// Limpa a tela
		// Imprime o labirinto
		
		/* Dado a posição atual, verifica quais sao as próximas posições válidas
			Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		 	e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		 	cada uma delas no vetor valid_positions
		 		- pos.i, pos.j+1
		 		- pos.i, pos.j-1
		 		- pos.i+1, pos.j
		 		- pos.i-1, pos.j
		 	Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	 	*/

		
	
		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		/*
		if (!valid_positions.empty()) {
			pos_t next_position = valid_positions.top();
			valid_positions.pop();
		}*/
	return false;
}

void free_memory(){
	for (int i = 0; i < num_rows; ++i){
    	delete [] maze[i];
	}
delete [] maze;
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze("../data/maze4.txt");
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	free_memory();
	// Tratar o retorno (imprimir mensagem)
	return 0;
}
