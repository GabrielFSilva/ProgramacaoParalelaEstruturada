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
 *  Description: This program generates two files (inputA.txt and inputB.txt) that will
 *  be the input stream to fill up the A and B matrices to apply dot product over fixed
 * 	size and write the results in an output file (output.txt)
 *  Version: 2.0 (02/09/2018)
 *  Compilation Command: g++ -std=c++1y dot_product_stream.cpp -o exe
 ****************************************************************************
*/

#include "pch.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>

#define MX 10

//#define N (204)
#define N (30)
//#define GN (N*10)

#define GN (N)

using namespace std::chrono;

void gen_input() {

	std::ofstream A_out;
	A_out.open("inputA.txt", std::ios::out);
	if (A_out.fail()) {
		std::cerr << "Error in: " << "inputA.txt" << std::endl;
		A_out.close();
		return;
	}
	std::ofstream B_out;
	B_out.open("inputB.txt", std::ios::out);
	if (B_out.fail()) {
		std::cerr << "Error in: " << "inputB.txt" << std::endl;
		B_out.close();
		return;
	}
	for (long int a = 0; a < GN; a++) {
		for (long int i = 0; i < N; ++i) {
			A_out << " ";
			A_out << i + 1;
			B_out << " ";
			B_out << i * 3;
		}
	}
	A_out.close();
	B_out.close();

	std::cout << "Input Finished." << std::endl;
}

void val(long int **A, long int **B, long int **C, long int valA, long int valB) {
	for (long int i = 0; i < MX; i++) {
		for (long int j = 0; j < MX; j++) {
			A[i][j] = valA;
			B[i][j] = valB;
			C[i][j] = 0;
		}
	}
}
std::ofstream stream_out;
std::ifstream stream_inA;
std::ifstream stream_inB;
void dp() {
	stream_inA.open("inputA.txt", std::ios::in);
	if (stream_inA.fail()) {
		std::cerr << "Error in: " << "inputA.txt" << std::endl;
		stream_inA.close();
		return;
	}

	stream_inB.open("inputB.txt", std::ios::in);
	if (stream_inB.fail()) {
		std::cerr << "Error in: " << "inputB.txt" << std::endl;
		stream_inB.close();
		return;
	}

	stream_out.open("output.txt", std::ios::out);
	if (stream_out.fail()) {
		std::cerr << "Error in: " << "output.txt" << std::endl;
		stream_out.close();
		return;
	}
	while (!stream_inB.eof()) {

		long int **A = new long int*[MX];
		long int **B = new long int*[MX];
		long int **C = new long int*[MX];

		for (long int i = 0; i < MX; i++) {
			A[i] = new long int[MX];
			B[i] = new long int[MX];
			C[i] = new long int[MX];
		}

		long int valA;
		long int valB;
		stream_inA >> valA;
		stream_inB >> valB;

		val(A, B, C, valA, valB);

		for (long int i = 0; i < MX; i++) {
			for (long int j = 0; j < MX; j++) {
				for (long int k = 0; k < MX; k++) {
					C[i][j] += (A[i][k] * B[k][j]);
				}
			}
		}
		delete A;
		delete B;
		for (long int i = 0; i < MX; i++) {
			for (long int j = 0; j < MX; j++) {
				stream_out << C[i][j];
				stream_out << " ";

			}
			stream_out << "\n";
		}
		stream_out << "----------------------------------\n";
		delete C;
	}
	stream_inA.close();
	stream_inB.close();
	stream_out.close();

}


int main(int argc, char const *argv[]) {
	gen_input();
	dp();
	return 0;
}