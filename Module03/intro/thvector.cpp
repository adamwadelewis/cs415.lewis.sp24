//
// File:   thvector.cpp
// Author: Adam.Lewis@athens.edu
// Purpose:
// Illustrate use of STL containers with threads.
//
#include<iostream>
#include<thread>
#include<vector>
#include<functional>
#include<string>
// Let's be lazy
using namespace std;
//
// void printIt(int, string &)
//
// Let's just do something silly as our thread function
//
void printIt(int index, string &title) {
  string msg = to_string(index) + " : " + title;
  cout << msg  << endl;
};
// int main()
//
// Now let's build a vector of threads and do something with them 
//
int main() {
  vector<string> texts = {
    "Dr. Lewis is evil",
    "But he is fair and just in his evil",
    "Yet he is indeed an evil professor <evil laugh here>"
  };
  vector<thread> threads;
  for (int i = 0; i < texts.size(); ++i) {
    threads.push_back(thread(printIt, std::ref(i), std::ref(texts[i])));
  }
  for (auto &th : threads) {
    th.join();
  }
  return 0;
}
