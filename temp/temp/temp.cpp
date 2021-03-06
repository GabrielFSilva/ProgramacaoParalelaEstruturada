/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  As a special exception, you may use this file as part of a free software
 *  library without restriction.  Specifically, if other files instantiate
 *  templates or use macros or inline functions from this file, or you compile
 *  this file and link it with other files to produce an executable, this
 *  file does not by itself cause the resulting executable to be covered by
 *  the GNU General Public License.  This exception does not however
 *  invalidate any other reasons why the executable file might be covered by
 *  the GNU General Public License.
 *
 ****************************************************************************
 *  Authors: Dalvan Griebler <dalvangriebler@gmail.com>
 *
 *  Copyright: GNU General Public License
 *  Description: This is a simple matrix multiplication algorithm.
 *  File Name: matrix_multi.cpp
 *  Version: 1.0 (25/05/2018)
 *  Compilation Command: g++ -std=c++1y matrix_multi.cpp -o exe
 ****************************************************************************
*/

#include "pch.h"
#include <iostream>
#include <chrono>
#include <tbb/tbb.h>
//#include <ctime>
//Matrix sizes
#define MX	750

using namespace tbb;
using namespace std::chrono;

//all the matrix
long int **matrix1, **matrix2, **matrix;

void val() {
	long int i, j;
	for (i = 0; i < MX; i++) {
		for (j = 0; j < MX; j++) {
			matrix1[i][j] = 4;
			matrix2[i][j] = 5;
			matrix[i][j] = 0;
		}
	}
}
void printMatrix(long int **matrix) {
	int i, j;
	for (i = 0; i < MX; i++) {
		printf("\n");
		for (j = 0; j < MX; j++) {
			printf("%ld ", matrix[i][j]);
		}
	}
	printf("\n");
}
//matrix multiplication algorithm
void multiply() {
	for (long int i = 0; i < MX; i++) {
		for (long int j = 0; j < MX; j++) {
			for (long int k = 0; k < MX; k++) {
				matrix[i][j] += (matrix1[i][k] * matrix2[k][j]);
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	tbb::task_scheduler_init init(4);

	double timersSum = 0;

	std::cout << "Hello World!\n";
	matrix = new long int*[MX];
	matrix1 = new long int*[MX];
	matrix2 = new long int*[MX];

	for (long int i = 0; i < MX; i++) {
		matrix[i] = new long int[MX];
		matrix1[i] = new long int[MX];
		matrix2[i] = new long int[MX];
	}
	for (int rep = 0; rep < 10; rep++)
	{
		//assigning fixed values to the matrix			
		val();
		steady_clock::time_point t1 = steady_clock::now();

		//matrix multiplication algorithm call
		multiply();

		steady_clock::time_point t2 = steady_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

		std::cout << "It took me " << time_span.count() << " seconds.";
		std::cout << std::endl;
		timersSum += time_span.count();

	}
	printf("SIZE= %d\n", MX);
	std::cout << "The average time was " << timersSum / 10 << " seconds.";
	//printing the resultant matrix (you may comment when bigger sizes will be set-up)
	printMatrix(matrix);

	delete matrix;
	delete matrix1;
	delete matrix2;
}
// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
