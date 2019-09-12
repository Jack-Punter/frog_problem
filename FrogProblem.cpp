#include <random>
#include <iostream>
#include <chrono>
#include <thread>

void simulate(double* retVal, int numIterations)
{
	std::random_device rd;
	std::mt19937_64 rng(rd());
	double average = 0.0;
	for (int i = 0; i < numIterations; i++)
	{
		int count = 0;
		int options = 10;
		while (options > 0)
		{
			std::uniform_int_distribution<int> uDist(1, options);
			int choice = uDist(rng);
			options -= choice;
			count++;
			if (options < 0)
				std::cout << "something is wrong\n";
		}
		average += double(count) / double(numIterations);
	}
	*retVal = average;
}

int main()
{
	

	const int numIterations = 500000000;
	double averages[4] = { 0.0 };
	std::thread execs[4];
	auto t0 = std::chrono::high_resolution_clock::now();
	
	for (int i = 0; i < 4; i++)
	{
		execs[i] = std::thread(simulate, &averages[i], numIterations);
	}

	for (int i = 0; i < 4; i++)
	{
		execs[i].join();
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::time_point_cast<std::chrono::seconds>(t0).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::seconds>(t1).time_since_epoch().count();

	double average = (averages[0] + averages[1] + averages[2] + averages[3]) / 4.0;
	std::cout << "simulations took " << end - start << "s to run " << numIterations*4 << " iterations\n";
	std::cout << "Frog took on average " << average << " jumps to cross\n";
	std::cin.get();
}