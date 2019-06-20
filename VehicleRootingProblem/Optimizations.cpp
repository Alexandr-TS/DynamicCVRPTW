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

bool LocalReverseOptimization(std::vector<int>& path, InputData& input, bool pathHasFirst0) {
	path.push_back(0);
	if (!pathHasFirst0) {
		path.push_back(0);
		rotate(path.begin(), path.begin() + path.size() - 1, path.end());
	}
	// we have path: 0 4 7 1 3 0

	bool improvedSolution = false;

	while (true) {
		bool improved = false;

		// try to reverse [i, j]
		for (size_t i = 1; i + 1 < path.size(); ++i) {
			for (size_t j = i + 1; j + 1 < path.size(); ++j) {
				double curDist = input.Distance(path[i - 1], path[i]) +
					input.Distance(path[j], path[j + 1]);
				double anotherDist = input.Distance(path[i - 1], path[j]) +
					input.Distance(path[i], path[j + 1]);
				if (anotherDist < curDist) {
					reverse(path.begin() + i, path.begin() + j + 1);
					improved = true;
					improvedSolution = true;
				}
			}
		}
		
		if (!improved)
			break;
	}
	if (!pathHasFirst0) {
		rotate(path.begin(), path.begin() + 1, path.end());
		assert(path.back() == 0);
		path.pop_back();
	}
	assert(path.back() == 0);
	path.pop_back();

	if (!pathHasFirst0) {
		assert(find(path.begin(), path.end(), 0) == path.end());
	}

	return improvedSolution;
}

// Recalculates prefix and sufix sums of distances of path
void recalcPS(std::vector<double>& pref, std::vector<double>& suf, std::vector<int>& path, InputData& input) {
	if (!path.size()) {
		pref.resize(0);
		suf.resize(0);
		return;
	}
	pref.resize(path.size());
	suf.resize(path.size());
	pref[0] = 0;
	for (int i = 1; i < (int)pref.size(); i++) {
		pref[i] = pref[i - 1] + input.Distance(path[i - 1], path[i]);
	}
	suf[(int)path.size() - 1] = 0;
	for (int i = (int)path.size() - 2; i >= 0; i--) {
		suf[i] = suf[i + 1] + input.Distance(path[i], path[i + 1]);
	}
}

void MakeStringCross(std::vector<std::vector<int>>& paths, std::vector<std::vector<double>>& pref, 
	std::vector<std::vector<double>>& suf, int i1, int i2, int p1, int p2, InputData& input) {
	auto tmp = paths[p1];
	paths[p1].resize(i1 - i2 + (int)paths[p2].size());
	for (int j = i2 + 1; j < (int)paths[p2].size(); ++j)
		paths[p1][j - i2 + i1] = paths[p2][j];
	paths[p2].resize(i2 - i1 + (int)tmp.size());
	for (int j = i1 + 1; j < (int)tmp.size(); ++j)
		paths[p2][j - i1 + i2] = tmp[j];
	recalcPS(pref[p1], suf[p1], paths[p1], input);
	recalcPS(pref[p2], suf[p2], paths[p2], input);
}

// Paths have to be in format {{0, 1, 2, 5}, {0, 6, 4}} or {{1, 2, 5}, {6, 4}}
bool StringCrossOptimization(std::vector<std::vector<int>>& paths, InputData& input, 
	ProblemMode problemMode, bool pathsHaveFirst0) {
	for (auto& path: paths)
		path.emplace_back(0);
	if (!pathsHaveFirst0) {
		for (auto& path : paths)
			path.insert(path.begin(), { 0 });
	}

	std::vector<std::vector<double>> pref(paths.size());
	std::vector<std::vector<double>> suf(paths.size());

	for (int p = 0; p < (int)paths.size(); ++p) {
		recalcPS(pref[p], suf[p], paths[p], input);
	}

	bool improvedSolution = false;

	for (int p1 = 0; p1 < (int)paths.size(); ++p1) {
		for (int p2 = p1 + 1; p2 < (int)paths.size(); ++p2) {
			for (int i1 = 1; i1 < (int)paths[p1].size() - 2; ++i1) {
				for (int i2 = 1; i2 < (int)paths[p2].size() - 2; ++i2) {
					//assert(paths[p1].size() == pref[p1].size() && paths[p1].size() == suf[p1].size());
					//assert(paths[p2].size() == pref[p2].size() && paths[p2].size() == suf[p2].size());
					double pref1 = pref[p1][i1];
					double suf1 = suf[p1][i1 + 1];
					double pref2 = pref[p2][i2];
					double suf2 = suf[p2][i2 + 1];
					double edge1 = input.Distance(paths[p1][i1], paths[p1][i1 + 1]);
					double edge2 = input.Distance(paths[p2][i2], paths[p2][i2 + 1]);
					double crossEdge1 = input.Distance(paths[p1][i1], paths[p2][i2 + 1]);
					double crossEdge2 = input.Distance(paths[p2][i2], paths[p1][i1 + 1]);
					if (problemMode == ProblemMode::MINMAXLEN) {
						double maxPrevDist = std::max(pref1 + edge1 + suf1, pref2 + edge2 + suf2);
						double maxNewDist = std::max(pref1 + crossEdge1 + suf2, pref2 + crossEdge2 + suf1);
						if (maxNewDist < maxPrevDist) {
							MakeStringCross(paths, pref, suf, i1, i2, p1, p2, input);
							improvedSolution = true;
						}
					}
					else if (problemMode == ProblemMode::MINSUM) {
						if (edge1 + edge2 > crossEdge1 + crossEdge2) {
							if (pref1 + crossEdge1 + suf2 <= input.MaxDist && pref2 + crossEdge2 + suf1 <= input.MaxDist) {
								MakeStringCross(paths, pref, suf, i1, i2, p1, p2, input);
								improvedSolution = true;
							}
						}
					}
				}
			}
		}
	}

	if (!pathsHaveFirst0) {
		for (auto& path : paths) {
			assert(path[0] == 0);
			path.erase(path.begin());
		}
	}

	for (auto& path : paths) {
		assert(path.back() == 0);
		path.pop_back();
	}

	if (!pathsHaveFirst0) {
		for (auto& path : paths) {
			assert(find(path.begin(), path.end(), 0) == path.end());
		}
	}

	return improvedSolution;
}