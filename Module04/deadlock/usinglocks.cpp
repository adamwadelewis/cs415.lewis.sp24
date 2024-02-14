// 
// File:   usinglocks.cpp
// Author: Your Glorious Instructor
// Purpose:
// Avoiding deadlocks by using std::lock to simultaneously lock two
// mutexes.
//
// Based upon: https://www.bogotobogo.com/cplusplus/C11/9_C11_DeadLock.php
//
#include <mutex>

using namespace std;

class MyObjectClass {};

void swap(MyObjectClass& lhs,MyObjectClass& rhs);

class X
{
private:
	MyObjectClass myObj;
	std::mutex m;
public:
	X(MyObjectClass const& obj):myObj(obj){}
	friend void swap(X& lhs, X& rhs)
	{
		// the arguments are checked to ensure they are different instances, 
		// because attempting to acquire a lock on a std::mutex 
		// when we already hold it is undefined behavior. 
		if(&lhs;==&rhs;) return;

		// the call to std::lock() locks the two mutexes
		std::lock(lhs.m,rhs.m);

		// two std::lock_guard instances are constructed one for each mutex.
		std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock);
		std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock);

		swap(lhs.myObj, rhs.myObj);
	}
};