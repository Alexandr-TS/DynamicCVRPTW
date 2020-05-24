#include "SolverBruteForce.h"
#include "SolverGenetic.h"

#include <numeric>

using namespace std;

ProblemSolution SolverBruteForce::Run(InputData input, vector<double>args) {
	vector<int> permutation(input.TargetsCnt);
	iota(permutation.begin(), permutation.end(), 1);

	auto best_solution = ProblemSolution();
	best_solution.SumOfPathLengths = INF;

	// Check if any feasible solution exists
	for (int i = 1; i <= input.TargetsCnt; ++i) {
		if (input.Distance(0, i) * 2 > input.MaxDist || 
			input.TimeWindows[i].second < input.Distance(0, i)) {
			return best_solution;
		}
	}

	do {
		auto paths = SplitPaths(permutation, input);
		auto solution = ProblemSolution(input, paths);
		if (solution.SolutionExists && solution.SumOfPathLengths < best_solution.SumOfPathLengths) {
			best_solution = solution;
		}
	} while (next_permutation(permutation.begin(), permutation.end()));
	return best_solution;
}
