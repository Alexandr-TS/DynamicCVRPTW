#include "Optimizations.h"

// Bruteforces all paths and all elements in different paths and tries to swap them.
ProblemSolution SwapOptimization(ProblemSolution solution, ProblemMode problemMode) {
	for (size_t path1 = 0; path1 < solution.Paths.size(); ++path1) {
		for (size_t path2 = path1 + 1; path2 < solution.Paths.size(); ++path2) {
			for (size_t ind1 = 0; ind1 < solution.Paths[path1].size(); ++ind1) {
				for (size_t ind2 = 0; ind2 < solution.Paths[path2].size(); ++ind2) {
					int prev1 = (ind1 == 0 ? 0 : solution.Paths[path1][ind1 - 1]);
					int next1 = (ind1 + 1 == solution.Paths[path1].size() ? 0 : solution.Paths[path1][ind1 + 1]);
					int cur1 = solution.Paths[path1][ind1];
					int prev2 = (ind2 == 0 ? 0 : solution.Paths[path2][ind2 - 1]);
					int next2 = (ind2 + 1 == solution.Paths[path2].size() ? 0 : solution.Paths[path2][ind2 + 1]);
					int cur2 = solution.Paths[path2][ind2];
					if (problemMode == ProblemMode::MINSUM && false) { // has a bug: can break good solution, by making path longer than maxDist
						if (solution.Input.Distance(prev1, cur1) + solution.Input.Distance(cur1, next1) +
							solution.Input.Distance(prev2, cur2) + solution.Input.Distance(cur2, next2) >
							solution.Input.Distance(prev1, cur2) + solution.Input.Distance(cur2, next1) +
							solution.Input.Distance(prev2, cur1) + solution.Input.Distance(cur1, next2)) {
							std::swap(solution.Paths[path1][ind1], solution.Paths[path2][ind2]);
						}
					} else {
						if (solution.Input.Distance(prev1, cur1) + solution.Input.Distance(cur1, next1) >
							solution.Input.Distance(prev1, cur2) + solution.Input.Distance(cur2, next1) && 
							solution.Input.Distance(prev2, cur2) + solution.Input.Distance(cur2, next2) >
							solution.Input.Distance(prev2, cur1) + solution.Input.Distance(cur1, next2)) {
							std::swap(solution.Paths[path1][ind1], solution.Paths[path2][ind2]);
						}
					}
				}
			}
		}
	}
	auto nsolution = ProblemSolution(solution.Input, solution.Paths);
	return nsolution;
}

// 3-edges update
ProblemSolution Edges3Optimization(ProblemSolution solution, ProblemMode problemMode) {
	std::vector<std::vector<int> > swaps = {
		{0, 1, 2, 3, 5, 4},
		{0, 1, 3, 2, 4, 5},
		{0, 1, 3, 2, 5, 4},
		{0, 1, 4, 5, 2, 3},
		{0, 1, 4, 5, 3, 2},
		{0, 1, 5, 4, 2, 3},
		{0, 1, 5, 4, 3, 2},
	};

	for (std::vector<int> path: solution.Paths) {
		if (!path.size()) continue;
		path.push_back(0); // depot
		rotate(path.begin(), path.begin() + (int)path.size() - 1, path.end()); // depot to 0 index
		int n = path.size();
		for (int i1 = 0; i1 + 1 < n; ++i1)
			for (int i2 = i1 + 2; i2 + 1 < n; ++i2)
				for (int i3 = i2 + 2; i3 + 1 < n; ++i3) {
					std::vector<int> indices = {i1 + 1, i2, i2 + 1, i3, i3 + 1, i1};
					double startDist = solution.Input.Distance(path[indices[1]], path[indices[2]]) +
									 solution.Input.Distance(path[indices[3]], path[indices[4]]) +
									 solution.Input.Distance(path[indices[5]], path[indices[0]]);
					int bestSwapsInd = -1;
					double bestDist = INF;
					for (size_t i = 0; i < swaps.size(); ++i) {
						auto permutation = swaps[i];
						for (int i = 0; i < 6; ++i)
							permutation[i] = indices[permutation[i]];
						double curDist = solution.Input.Distance(path[permutation[1]], path[permutation[2]]) +
							solution.Input.Distance(path[permutation[3]], path[permutation[4]]) +
							solution.Input.Distance(path[permutation[5]], path[permutation[0]]);
						if (curDist < bestDist && curDist < startDist) {
							bestDist = curDist;
							bestSwapsInd = i;
						}
					}
					if (bestSwapsInd != -1) {
						std::vector<int> newPath;
						std::vector<std::vector<int> > subPaths(3);
						for (int i = i1 + 1; i <= i2; ++i)
							subPaths[0].push_back(path[i]);
						for (int i = i2 + 1; i <= i3; ++i)
							subPaths[1].push_back(path[i]);
						for (int i = i3 + 1; i != i1 + 1; i = (i + 1) % n)
							subPaths[2].push_back(path[i]);
						if (swaps[bestSwapsInd][2] > 3)
							std::swap(subPaths[1], subPaths[2]);
						if (swaps[bestSwapsInd][2] > swaps[bestSwapsInd][3])
							reverse(subPaths[1].begin(), subPaths[1].end());
						if (swaps[bestSwapsInd][4] > swaps[bestSwapsInd][5])
							reverse(subPaths[2].begin(), subPaths[2].end());
						for (int i = 0; i < 3; ++i)
							newPath.insert(newPath.end(), subPaths[i].begin(), subPaths[i].end());
						int posDepot = (find(newPath.begin(), newPath.end(), 0) - newPath.begin());
						rotate(newPath.begin(), newPath.begin() + posDepot, newPath.end());
						path = newPath;
					}
				}
		assert(path[0] == 0);
		rotate(path.begin(), path.begin() + 1, path.end());
		assert(path.back() == 0);
		path.pop_back();
		assert(find(path.begin(), path.end(), 0) == path.end());
	}
	solution = ProblemSolution(solution.Input, solution.Paths);
	return solution;
}

ProblemSolution ReverseOptimization(ProblemSolution solution, ProblemMode problemMode) {
	while (true) {
		bool improved = false;
		for (auto& path : solution.Paths) {
			path.push_back(0);
			path.push_back(0);
			rotate(path.begin(), path.begin() + path.size() - 1, path.end());
			// we have path: 0 4 7 1 3 0

			// try to reverse [i, j]
			for (size_t i = 1; i + 1 < path.size(); ++i) {
				for (size_t j = i + 1; j + 1 < path.size(); ++j) {
					double curDist = solution.Input.Distance(path[i - 1], path[i]) +
						solution.Input.Distance(path[j], path[j + 1]);
					double anotherDist = solution.Input.Distance(path[i - 1], path[j]) +
						solution.Input.Distance(path[i], path[j + 1]);
					if (anotherDist < curDist) {
						reverse(path.begin() + i, path.begin() + j + 1);
						improved = true;
					}
				}
			}
			rotate(path.begin(), path.begin() + 1, path.end());
			assert(path.back() == 0);
			path.pop_back();
			assert(path.back() == 0);
			path.pop_back();
		}
		if (!improved)
			break;
	}
	return ProblemSolution(solution.Input, solution.Paths);
}