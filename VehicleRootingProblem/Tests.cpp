#include "Tests.h"
#include "Optimizations.h"
#include "SolverAntColony.h"
#include "SolverGenetic.h"
#include "SolverBruteForce.h"

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
	cout << "________________________________________TestMath: OK\n";
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
	cout << "________________________________________Test Global Swap Opt: OK\n";
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

	cout << "________________________________________Test Global Insert Opt: OK\n";
}

void TestLocalSwapOpt1() {
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

	cout << "________________________________________Test Local Swap Opt: OK\n";
}

void TestLocalSwapOpt2() {
	auto input = InputData(2, 4, 1000,
		{ {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4} },
		{ {-INF, INF}, {1, 1.5}, {2, 2.5}, {3, 3.5}, {4, 4.5} });
	vector<int> path = { 0, 4, 2, 3, 1 };

	bool ans = LocalSwapOptimization(path, input);
	assert(ans);
	vector<int> expected_path = { 0, 1, 2, 3, 4 };
	assert(expected_path == path);

	ans = LocalSwapOptimization(path, input);
	assert(!ans);

	cout << "________________________________________Test Local Swap Opt: OK\n";
}

void TestAntColonyAlgo() {
	auto input = InputData(2, 4, 1000,
		{ {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4} },
		{ {-INF, INF}, {1, 1.5}, {2, 2.5}, {3, 3.5}, {4, 4.5} });
	vector<double> args = { 3, 4, 0.4, 2, 3, 2, 5, 0.5, 0 };
	auto solution = SolverAntColony::Run(input, args);

	assert(solution.SolutionExists);
	vector<int> expected_path = { 1, 2, 3, 4 };
	assert(solution.Paths[0] == expected_path);
	assert(abs(solution.MaxPathLength - 8) < EPS);
	assert(abs(solution.SumOfPathLengths - 8) < EPS);

	input = InputData(2, 3, 1000, 
		{ {0, 0}, {0, 3}, {4, 0}, {4, 3} },
		{ {-INF, INF}, {3, 3.5}, {4, 8.5}, {7, 7.5} });
	auto solution2 = SolverAntColony::Run(input, args);

	assert(solution2.SolutionExists);

	cout << "________________________________________Test Ant Colony Algo: OK\n";
}

void TestGeneticAlgo() {
	auto input = InputData(2, 4, 1000,
		{ {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4} },
		{ {-INF, INF}, {1, 1.5}, {2, 2.5}, {3, 3.5}, {4, 4.5} });
	vector<double> args = { 30, 1500, 500, 0.5, 0.05, 0 };

	auto solution = SolverGenetic::Run(input, args);

	assert(solution.SolutionExists);
	vector<int> expected_path = { 1, 2, 3, 4 };
	assert(solution.Paths[0] == expected_path);
	assert(abs(solution.MaxPathLength - 8) < EPS);
	assert(abs(solution.SumOfPathLengths - 8) < EPS);

	input = InputData(2, 3, 1000, 
		{ {0, 0}, {0, 3}, {4, 0}, {4, 3} },
		{ {-INF, INF}, {3, 3.5}, {4, 8.5}, {7, 7.5} });
	auto solution2 = SolverGenetic::Run(input, args);

	assert(solution2.SolutionExists);

	cout << "________________________________________Test Genetic Algo: OK\n";
}

void TestBruteForceAlgo() {
	auto input = InputData(2, 4, 1000,
		{ {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4} },
		{ {-INF, INF}, {1, 1.5}, {2, 2.5}, {3, 3.5}, {4, 4.5} });
	vector<double> args = {};

	auto solution = SolverBruteForce::Run(input, args);

	assert(solution.SolutionExists);
	vector<int> expected_path = { 1, 2, 3, 4 };
	assert(solution.Paths[0] == expected_path);
	assert(abs(solution.MaxPathLength - 8) < EPS);
	assert(abs(solution.SumOfPathLengths - 8) < EPS);

	input = InputData(2, 3, 1000, 
		{ {0, 0}, {0, 3}, {4, 0}, {4, 3} },
		{ {-INF, INF}, {3, 3.5}, {4, 8.5}, {7, 7.5} });
	auto solution2 = SolverBruteForce::Run(input, args);

	assert(solution2.SolutionExists);

	cout << "________________________________________Test Genetic Algo: OK\n";
}

void RunTests() {
	TestMath();
	TestGlobalSwapOpt();
	TestGlobalInsertOpt();
	TestLocalSwapOpt1();
	TestLocalSwapOpt2();
	TestAntColonyAlgo();
	TestGeneticAlgo();
	TestBruteForceAlgo();
	cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_Unit testing have finished successfully\n";
}

