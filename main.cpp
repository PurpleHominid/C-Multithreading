//https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php
//https://www.bogotobogo.com/cplusplus/C11/7_C11_Thread_Sharing_Memory.php

#include <iostream>
#include <thread>
#include <string>
#include <functional>
#include <list>
#include <algorithm>
#include <vector>
#include <mutex> //can be used to help avoid a thread based resource race


std::mutex mu;

// a global variable
std::list<int>myList;

// a global instance of std::mutex to protect global variable
std::mutex myMutex;

void addToList(int max, int interval)
{
	// the access to this function is mutually exclusive
	std::lock_guard<std::mutex> guard(myMutex);
	for (int i = 0; i < max; i++) {
		if( (i % interval) == 0) myList.push_back(i);
	}
}

void printList()
{
	// the access to this function is mutually exclusive
	std::lock_guard<std::mutex> guard(myMutex);
	for (auto itr = myList.begin(), end_itr = myList.end(); itr != end_itr; ++itr ) {
		std::cout << *itr << ",";
	}
}


void thread_function1(std::string s) { //receive by value
	mu.lock(); //lock
    std::cout << "\nthread1 function threadid: " << std::this_thread::get_id();
    std::cout << "message is = " << s << std::endl;
	mu.unlock(); //unlock
	s = "BBB";
}

void thread_function2(std::string &s) { //receive by reference
    std::cout << "\nthread2 function threadid: " << std::this_thread::get_id();
    std::cout << "nmessage is = " << s << std::endl;
	s = "CCC";
}

void thread_function3(std::string s) { //receive by move
    std::cout << "\nthread3 function threadid: " << std::this_thread::get_id();
    std::cout << "nmessage is = " << s << std::endl;
	s = "DDD";
}

int main() {
	std::cout << "Threads available= " <<  std::thread::hardware_concurrency() << std::endl;
    std::string s = "AAA";

	//pass by value
    std::thread td1(&thread_function1, s);
    std::cout << "\nmain thread1 message (" <<  td1.get_id() << ") = " << s << std::endl;
    td1.join();

	//pass by reference
    std::thread td2(&thread_function2, std::ref(s));
    std::cout << "\nmain thread2 message (" <<  td1.get_id() << ") = " << s << std::endl;
    td2.join();

	//pass by moving; moving the value means it is not available at the original source
    std::thread td3(&thread_function3, std::move(s));
    std::cout << "\nmain thread3 message (" <<  td1.get_id() << ") = " << s << std::endl;
    td3.join();
    
	//lambda fucntions
    std::thread t([]() {
        std::cout << "lamda thread function\n";
    });

    std::cout << "main thread\n";
    t.join();     // main thread waits for t to finish

	int max = 100;

	std::thread t1(addToList, max, 1);
	std::thread t2(addToList, max, 10);
	std::thread t3(printList);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}

