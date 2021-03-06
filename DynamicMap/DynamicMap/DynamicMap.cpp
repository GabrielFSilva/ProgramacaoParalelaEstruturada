// DynamicMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <utility>
#include <string>
#include <list>

using namespace std;const int N = 100;

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
			this->d_condition.wait(lock, [=] { 
				return !this->d_queue.empty() || this->d_queue.size() <= 0; 
			});
			if (d_queue.size() <= 0)
				return -1;
			T rc(std::move(this->d_queue.back()));
			this->d_queue.pop_back();
			return rc;
		}
	};

}

int main()
{
	comm::queue<int> q;
	list<int> mylist;
	for (int i = 1; i <= 20000; i++)
	{
		q.push(i);
		mylist.push_front(false);
	}
	std::thread th[N];
	for (size_t i = 0; i < N; i++) {
		th[i] = std::thread([&q, &mylist, i] {
			int value = q.pop();
			while (value >= 0)
			{
				//value = new int(value * value);
				//string debug = "Thread: " + to_string(i) + " " + to_string(value * value);
				//cout << debug << endl;
				list<int>::iterator it = mylist.begin();
				advance(it, value - 1);
				*it += 1;
				value = q.pop();
			}
			cout << "dead";
			return;
		});
	}
	for (size_t i = 0; i < N; i++) {
		th[i].join();
	}	for (std::list<int>::iterator it2 = mylist.begin(); it2 != mylist.end(); ++it2)
		std::cout << ' ' << *it2;
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
