// PrimeNumbersTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <iostream>
//#include <getopt.h>
#include <cstring>

int workers = 12;

namespace comm {

	template <typename T>
	class queue
	{
	private:
		std::mutex              d_mutex;
		std::condition_variable d_condition;
		std::deque<T>         d_queue;
	public:
		void push(T const& value) {
			{
				std::unique_lock<std::mutex> lock(this->d_mutex);
				d_queue.push_front(value);
			}
			this->d_condition.notify_one();
		}
		T pop() {
			std::unique_lock<std::mutex> lock(this->d_mutex);
			this->d_condition.wait(lock, [=] { return !this->d_queue.empty(); });
			T rc(std::move(this->d_queue.back()));
			this->d_queue.pop_back();
			return rc;
		}
	};

}

/*int update_args(int argc, char *argv[], int &n) {
	int option;
	while ((option = getopt(argc, argv, "n:r:h")) != -1) {
		switch (option) {
		case 'n':
			n = atoi(optarg);
			break;
		case 'r':
			workers = atoi(optarg);
			break;
		case 'h':
			std::cout << "\nUsage: " << argv[0] << " [options]...\n\n" <<
				"Options:\n" <<
				" -n <integer>             it will find all prime numbers from 1 to this number\n" <<
				" -r <integer>             it will set the number of replicas\n" << std::endl;
			return -1;
			break;
		case '?':
			std::cerr << strerror(errno) << " Run  " << argv[0] << " -h for usage. " << std::endl;
			return -1;
			break;
		}
	}
	if (optind != argc) {
		std::cerr << strerror(errno) << " Too many arguments. Run  " << argv[0] << " -h for usage. " << std::endl;
		return -1;
	}
	return 0;
}*/
int prime_number(int n) {
	int total = 0;
	for (int i = 2; i <= n; i++) {
		int prime = 1;
		for (int j = 2; j < i; j++) {
			if (i % j == 0) {
				prime = 0;
				break;
			}
		}
		{total = total + prime; }
	}
	return total;
}
int main(int argc, char *argv[]) {
	int n = 500000;
	//if (update_args(argc, argv, n)) { exit(3); }
	std::cout << "\n PRIME APP (CPP version) \n Author: Dalvan Griebler <dalvangriebler@gmail.com>" << std::endl;
	std::cout << " Number of Replicas: " << workers << std::endl;
	std::cout << "\nLast Value(N): " << n << std::endl;

	auto t_start = std::chrono::high_resolution_clock::now();

	int total_primes = prime_number(n);

	auto t_end = std::chrono::high_resolution_clock::now();

	std::cout << "\n PRIME APP (CPP version) \n Normal end of execution." << std::endl;

	std::cout << "Execution Time(seconds): " << std::chrono::duration<double>(t_end - t_start).count() << " Primes: " << total_primes << std::endl;

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
