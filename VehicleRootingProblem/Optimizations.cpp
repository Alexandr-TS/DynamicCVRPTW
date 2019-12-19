#include "Optimizations.h"

using namespace std;

/* 
a1-a2-a3 
b1-b2-b3
changing to
a1-b2-a3
b1-a2-b3
------------
path1[i1] is a2, path2[i2] is b2
*/
bool IsValidPrecheckForGlobalOpt(InputData& input, vector<int>& path1, vector<int>& path2, int i1, int i2) {
	double dist = input.Distance(path1[i1], path2[i2]);
	// not enough time to get from a1 to b2
	if (input.TimeWindows[path1[i1 - 1]].first + dist > input.TimeWindows[path2[i2]].second) {
		return false;
	}
	// not enough time to get from b1 to a2
	if (input.TimeWindows[path2[i2 - 1]].first + dist > input.TimeWindows[path1[i1]].second) {
		return false;
	}
	// not enought time to get from a2 to b3
	if (input.TimeWindows[path1[i1]].first + dist > input.TimeWindows[path2[i2 + 1]].second) {
		return false;
	}
	// not enought time to get from b2 to a3
	if (input.TimeWindows[path2[i2]].first + dist > input.TimeWindows[path1[i1 + 1]].second) {
		return false;
	}
	return true;
}

// path[i - 1], path[i], path[i + 1] ===> path[i - 1], v, path[i + 1]
// INF if path is invalid
double CalcNewLenForGlobalOpt(InputData& input, vector<int>& path, int i = 0, int v = 0) {
	double cur_time = 0;
	double cur_len = 0;
	int path_i = path[i];
	path[i] = v;
	for (int j = 1; j < (int)path.size(); ++j) {
		double next_dist = input.Distance(path[j - 1], path[j]);
		if (input.TimeWindows[path[j]].second < cur_time + next_dist) {
			path[i] = path_i;
			return INF;
		}
		cur_len += next_dist;
		cur_time = max(input.TimeWindows[path[j]].first, cur_time + next_dist);
	}

	path[i] = path_i;
	return cur_len;
}

void AppendZerosToPaths(MatrixInt& paths) {
	for (auto& path : paths) {
		path.push_back(0);
	}
}

void PopZerosFromPaths(MatrixInt& paths) {
	for (auto& path : paths) {
		if (path.empty()) {
			continue;
		}
		assert(path.back() == 0);
		path.pop_back();
	}
}

// each path has format: {0, 3, 4, 1, 0}
bool GlobalSwapOptimization(MatrixInt& paths, InputData& input) {
	bool have_0 = (paths[0].back() == 0);
	if (!have_0) {
		AppendZerosToPaths(paths);
	}

	bool improved = false;

	for (size_t path_1 = 0; path_1 < paths.size(); ++path_1) {
		double len_1 = CalcNewLenForGlobalOpt(input, paths[path_1]);
		for (size_t path_2 = path_1 + 1; path_2 < paths.size(); ++path_2) {
			double len_2 = CalcNewLenForGlobalOpt(input, paths[path_2]);
			for (size_t i1 = 1; i1 + 1 < paths[path_1].size(); i1++) {
				for (size_t i2 = 1; i2 + 1 < paths[path_2].size(); i2++) {
					if (!IsValidPrecheckForGlobalOpt(input, paths[path_1], paths[path_2], i1, i2)) {
						continue;
					}
					double new_len_1 = CalcNewLenForGlobalOpt(input, paths[path_1], i1, paths[path_2][i2]);
					double new_len_2 = CalcNewLenForGlobalOpt(input, paths[path_2], i2, paths[path_1][i1]);
					if (new_len_1 + new_len_2 + EPS < len_1 + len_2 && new_len_1 < INF - 1 && new_len_2 < INF - 1) {
						swap(paths[path_1][i1], paths[path_2][i2]);
						len_1 = new_len_1;
						len_2 = new_len_2;
						improved = true;
					}
				}
			}
		}
	}

	if (!have_0) {
		PopZerosFromPaths(paths);
	}
	return improved;
}

bool GlobalInsertOptimization(MatrixInt& paths, InputData& input) {
	bool have_0 = (paths[0].back() == 0);
	if (!have_0) {
		AppendZerosToPaths(paths);
	}

	bool improved = false;
	bool iter_improved = false;

	for (size_t path_1 = 0; path_1 < paths.size(); ++path_1) {
		double len_1 = CalcNewLenForGlobalOpt(input, paths[path_1], 0, 0);
		auto cur_path = paths[path_1];
		cur_path.insert(cur_path.begin(), { -1 });
		iter_improved = false;
		for (size_t i1 = 1; !iter_improved && i1 + 1 < cur_path.size(); i1++) {
			swap(cur_path[i1 - 1], cur_path[i1]);
			for (size_t path_2 = 0; !iter_improved && path_2 < paths.size(); ++path_2) {
				if (path_2 == path_1) {
					continue;
				}
				double len_2 = CalcNewLenForGlobalOpt(input, paths[path_2], 0, 0);
				for (size_t i2 = 1; !iter_improved && i2 + 1 < paths[path_2].size(); i2++) {
					cur_path[i1] = paths[path_2][i2];
					double new_len_1 = CalcNewLenForGlobalOpt(input, cur_path);
					// this makes two same adjacent vertices => dist = 0
					double new_len_2 = CalcNewLenForGlobalOpt(input, paths[path_2], i2, paths[path_2][i2 - 1]);
					if (new_len_1 + new_len_2 + EPS < len_1 + len_2 && new_len_1 < INF - 1 && new_len_2 < INF - 1) {
						paths[path_1] = cur_path;
						paths[path_2].erase(paths[path_2].begin() + i2, paths[path_2].begin() + i2 + 1);
						improved = iter_improved = true;
					}
				}

			}
		}
	}

	if (!have_0) {
		PopZerosFromPaths(paths);
	}
	return improved;
}

bool LocalSwapOptimization(std::vector<int>& path, InputData& input) {
	bool have_0 = (path.back() == 0);
	if (!have_0) {
		path.push_back(0);
	}

	bool improved = false;

	double len_1 = CalcNewLenForGlobalOpt(input, path);
	for (size_t i1 = 1; i1 + 1 < path.size(); ++i1) {
		for (size_t i2 = i1 + 1; i2 + 1 < path.size(); ++i2) {
			swap(path[i1], path[i2]);
			double new_len = CalcNewLenForGlobalOpt(input, path);
			if (new_len + EPS < len_1) {
				improved = true;
				len_1 = new_len;
			}
			else {
				swap(path[i1], path[i2]);
			}
		}
	}
	if (!have_0) {
		path.pop_back();
	}
	return improved;
}

