#include <iostream>
#include <iomanip> 
#include <thread> 
#include <chrono>
#include <ctime>

using namespace std;
using std::chrono::system_clock;
int main()
{
	cout << "The id of current thread is " << this_thread::get_id << endl;

	//sleep while next minute is not reached

	//get current time
	time_t timet = system_clock::to_time_t(system_clock::now());
	//convert it to tm struct
	struct tm * time = localtime(&timet);
	cout << "Current time: " << put_time(time, "%X") << '\n';
	std::cout << "Waiting for the next minute to begin...\n";
	time->tm_min++; time->tm_sec = 0;
	//sleep until next minute is not reached
	this_thread::sleep_until(system_clock::from_time_t(mktime(time)));
	cout << std::put_time(time, "%X") << " reached!\n";
	//sleep for 5 seconds
	this_thread::sleep_for(chrono::seconds(5));
	//get current time
	timet = system_clock::to_time_t(system_clock::now());
	//convert it to tm struct
	time = std::localtime(&timet);
	cout << "Current time: " << put_time(time, "%X") << '\n';
}
