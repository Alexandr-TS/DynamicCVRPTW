#include "SolverBruteForce.h"
#include "SolverGenetic.h"

using namespace std;

ProblemSolution SolverBruteForce::Run(InputData input, vector<double>args) {
	vector<int> permutation;
	for (int i = 1; i <= input.TargetsCnt; ++i) {
		permutation.push_back(i);
	}

	auto bestSolution = ProblemSolution();
	bestSolution.SumOfPathLengths = INF;

	// Check if any feasible solution exists
	for (int i = 1; i <= input.TargetsCnt; ++i) {
		if (input.Distance(0, i) * 2 > input.MaxDist || 
			input.TimeWindows[i].second < input.Distance(0, i)) {
			return bestSolution;
		}
	}

	do {
		auto paths = SplitPaths(permutation, input);
		auto solution = ProblemSolution(input, paths);
		if (solution.SolutionExists && solution.SumOfPathLengths < bestSolution.SumOfPathLengths) {
			bestSolution = solution;
		}
	} while (next_permutation(permutation.begin(), permutation.end()));
	return bestSolution;
}
