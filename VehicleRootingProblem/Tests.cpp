#include "Tests.h"

using namespace std;

void TestMath() {
	auto rez1 = Math::GenDouble(0.5, 0.6);
	auto rez2 = Math::GenDouble(0.5, 0.6);
	assert(rez1 != rez2);
	assert(0.5 <= rez1 && rez1 <= 0.6);

	auto rez3 = Math::GenInt(3, 3);
	assert(rez3 == 3);

	auto rez4 = Math::PowInt(0.5, 3);
	assert(abs(0.125 - rez4) < 1e-6);
	cout << "TestMath: OK\n";
}

void TestGlobalSwapOpt() {
	auto input = InputData(2, 3, 1000, 
		{ {0, 0}, {0, 3}, {4, 0}, {4, 3} },
		{ {-INF, INF}, {3, 3.5}, {4, 8.5}, {7, 7.5} });
	vector<vector<int>> paths = { {0, 1, 2, 0}, {0, 3, 0} };
	bool ans = GlobalSwapOptimization(paths, input);
	assert(ans);
	vector<int> expected_path_0 = { 0, 1, 3, 0 };
	vector<int> expected_path_1 = { 0, 2, 0 };
	assert(paths[0] == expected_path_0);
	assert(paths[1l] == expected_path_1);
	cout << "Test Global Swap Opt: OK\n";
}

void RunTests() {
	TestMath();
	TestGlobalSwapOpt();
	cout << "Unit testing have finished successfully\n";
}

