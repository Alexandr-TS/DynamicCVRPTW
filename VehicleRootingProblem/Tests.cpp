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
	assert(paths[1] == expected_path_1);
	cout << "Test Global Swap Opt: OK\n";
}

void TestGlobalInsertOpt() {
	auto input = InputData(2, 3, 1000, 
		{ {0, 0}, {0, 3}, {4, 0}, {4, 3} },
		{ {-INF, INF}, {3, 3.5}, {4, 8.5}, {7, 7.5} });
	vector<vector<int>> paths = { {0, 1, 2, 0}, {0, 3, 0} };
	bool ans = GlobalInsertOptimization(paths, input);
	assert(ans);
	vector<int> expected_path_0 = { 0, 2, 0 };
	vector<int> expected_path_1 = { 0, 1, 3, 0 };
	assert(paths[0] == expected_path_0);
	assert(paths[1] == expected_path_1);

	bool second_ans = GlobalInsertOptimization(paths, input);
	assert(second_ans);
	expected_path_0 = { 0, 2, 3, 0 };
	expected_path_1 = { 0, 1, 0 };
	assert(paths[0] == expected_path_0);
	assert(paths[1] == expected_path_1);

	bool third_ans = GlobalInsertOptimization(paths, input);
	assert(!third_ans);

	cout << "Test Global Insert Opt: OK\n";
}

void TestLocalSwapOpt() {
	auto input = InputData(2, 4, 1000,
		{ {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4} },
		{ {-INF, INF}, {1, 1.5}, {2, 2.5}, {3, 3.5}, {4, 4.5} });
	vector<int> path = { 0, 4, 2, 3, 1, 0 };

	bool ans = LocalSwapOptimization(path, input);
	assert(ans);
	vector<int> expected_path = { 0, 1, 2, 3, 4, 0 };
	assert(expected_path == path);

	ans = LocalSwapOptimization(path, input);
	assert(!ans);

	cout << "Test Local Swap Opt: OK\n";
}

void RunTests() {
	TestMath();
	TestGlobalSwapOpt();
	TestGlobalInsertOpt();
	TestLocalSwapOpt();
	cout << "Unit testing have finished successfully\n";
}

