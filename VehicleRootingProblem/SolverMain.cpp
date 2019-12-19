#include "SolverMain.h"
#include "SolverBruteForce.h"
#include "SolverAntColony.h"
#include "SolverGenetic.h"
#include "SolverGreedy.h"

using namespace std;

ProblemSolution SolverMain::Run(InputData input, ProblemMode problemMode, 
	EAlgorithms algorithm, std::vector<double> args) {
	auto solution = ProblemSolution();
	double start_time = clock();
	if (algorithm == EAlgorithms::Greedy) {
		solution = SolverGreedy::Run(input, problemMode, args);
	}
	else if (algorithm == EAlgorithms::AntColony) {
		solution = SolverAntColony::Run(input, args);
	}
	else if (algorithm == EAlgorithms::Genetic) {
		solution = SolverGenetic::Run(input, args);
	}
	if (algorithm == EAlgorithms::BruteForce) {
		solution = SolverBruteForce::Run(input, args);
	}

	cout << "\n\n__________________________________________________\n";
	cout << "TOTAL WORK TIME: " << fixed << setprecision(2) << (clock() - start_time) / CLOCKS_PER_SEC << " " << endl;
	if (solution.SolutionExists) {
		cout << "SOLUTION SUM OF LENGTHS: " << fixed << setprecision(2) << solution.SumOfPathLengths << endl;
	}
	else {
		cout << "Solution not found" << endl;
	}
	cout << "__________________________________________________\n\n";
	return solution;
}

// Useful to run all the algos with selected input 
ProblemSolution RunAllAlgos(InputData input, ProblemMode problemMode, 
	EAlgorithms algorithm, std::vector<double> args) {
	vector<pair<double, double>> answer;

	auto solution = ProblemSolution();
	
	std::vector<std::vector<double>> argss = {
		{}, {3, 4, 0.4, 2, 3, 2, 5, 0.5, 0}, {30, 1500, 900, 0.5, 0.5, 0}, {} 
	};

	double start_time = clock();
	solution = SolverGreedy::Run(input, problemMode, argss[0]);
	if (solution.SolutionExists)
		answer.push_back({ (clock() - start_time) / CLOCKS_PER_SEC, solution.SumOfPathLengths });
	else 
		answer.push_back({ (clock() - start_time) / CLOCKS_PER_SEC, -1.0 });

	start_time = clock();
	solution = SolverAntColony::Run(input, argss[1]);
	if (solution.SolutionExists)
		answer.push_back({ (clock() - start_time) / CLOCKS_PER_SEC, solution.SumOfPathLengths });
	else 
		answer.push_back({ (clock() - start_time) / CLOCKS_PER_SEC, -1.0 });

	start_time = clock();
	solution = SolverGenetic::Run(input, argss[2]);
	if (solution.SolutionExists)
		answer.push_back({ (clock() - start_time) / CLOCKS_PER_SEC, solution.SumOfPathLengths });
	else 
		answer.push_back({ (clock() - start_time) / CLOCKS_PER_SEC, -1.0 });
	
	if (input.TargetsCnt <= 10) {
		start_time = clock();
		solution = SolverBruteForce::Run(input, argss[3]);
		if (solution.SolutionExists)
			answer.push_back({ (clock() - start_time) / CLOCKS_PER_SEC, solution.SumOfPathLengths });
		else
			answer.push_back({ (clock() - start_time) / CLOCKS_PER_SEC, -1.0 });
	}

	cout << "\n\n\n__________________________________________________";
	cout << "TOTAL WORK TIME: " << endl;
	for (auto x : answer) {
		cout << fixed << setprecision(2) << x.first << " " << x.second << " ";
	}
	cout << "__________________________________________________\n\n";
	return solution;
}
