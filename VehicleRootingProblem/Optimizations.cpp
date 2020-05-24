#include "Optimizations.h"

using namespace std;

// each path has format: {0, 1, 2, 3, 0}
size_t FirstPossibleToMoveElement(vector<int>& path, InputData& input, double cur_time) {
	if (cur_time < EPS) {
		return 1;
	}
	double tmp_time = 0;
	for (size_t i = 1; i < path.size(); ++i) {
		tmp_time = max(input.TimeWindows[path[i]].first, 
			tmp_time + input.Distance(path[i - 1], path[i]));
		if (tmp_time + EPS >= cur_time) {
			return i + 1;
		}
	}
	return path.size();
}

/* 
a1-a2-a3 
b1-b2-b3
changing to
a1-b2-a3
b1-a2-b3
------------
path1[i1] is a2, path2[i2] is b2
*/
bool IsValidPrecheckForGlobalOpt(InputData& input, vector<int>& path1, 
	vector<int>& path2, size_t i1, size_t i2) {
	double dist = input.Distance(path1[i1], path2[i2]);
	assert(i1 > 0u);
	assert(i2 > 0u);
	assert(i1 + 1 < path1.size());
	assert(i2 + 1 < path2.size());
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
double CalcNewLenForGlobalOpt(InputData& input, vector<int>& path, unsigned i = 0, int v = 0) {
	double cur_time = 0;
	double cur_len = 0;
	int path_i = path[i];
	path[i] = v;
	for (size_t j = 1; j < path.size(); ++j) {
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
			for (size_t i1 = 1; i1 + 1 < paths[path_1].size(); ++i1) {
				for (size_t i2 = 1; i2 + 1 < paths[path_2].size(); ++i2) {
					if (!IsValidPrecheckForGlobalOpt(input, paths[path_1], paths[path_2], i1, i2)) {
						continue;
					}
					double new_len_1 = CalcNewLenForGlobalOpt(input, paths[path_1], 
						static_cast<unsigned>(i1), paths[path_2][i2]);
					double new_len_2 = CalcNewLenForGlobalOpt(input, paths[path_2], 
						static_cast<unsigned>(i2), paths[path_1][i1]);
					if (new_len_1 + new_len_2 + EPS < len_1 + len_2 && 
						new_len_1 < INF - 1 && new_len_2 < INF - 1) {
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
		for (size_t i1 = 1; !iter_improved && i1 + 1 < cur_path.size(); ++i1) {
			swap(cur_path[i1 - 1], cur_path[i1]);
			for (size_t path_2 = 0; !iter_improved && path_2 < paths.size(); ++path_2) {
				if (path_2 == path_1) {
					continue;
				}
				double len_2 = CalcNewLenForGlobalOpt(input, paths[path_2], 0, 0);
				for (size_t i2 = 1; !iter_improved && i2 + 1 < paths[path_2].size(); ++i2) {
					cur_path[i1] = paths[path_2][i2];
					double new_len_1 = CalcNewLenForGlobalOpt(input, cur_path);
					// this makes two same adjacent vertices => dist = 0
					double new_len_2 = CalcNewLenForGlobalOpt(input, paths[path_2], 
						static_cast<unsigned>(i2), paths[path_2][i2 - 1]);
					if (new_len_1 + new_len_2 + EPS < len_1 + len_2 && 
						new_len_1 < INF - 1 && new_len_2 < INF - 1) {
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

pair<bool, bool> PreparePathWithZeros(vector<int>& path) {
	bool have_0_start = (path[0] == 0);
	if (!have_0_start) {
		path.insert(path.begin(), 0);
	}
	bool have_0_end = (path.back() == 0);
	if (!have_0_end) {
		path.push_back(0);
	}
	return { have_0_start, have_0_end };
}

void UndoPreparePathWithZeroz(vector<int>& path, pair<bool, bool> info) {
	if (!info.first) {
		path.erase(path.begin());
	}
	if (!info.second) {
		path.pop_back();
	}
}

bool LocalSwapOptimization(vector<int>& path, InputData& input) {
	auto zeros_info = PreparePathWithZeros(path);
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

	UndoPreparePathWithZeroz(path, zeros_info);
	return improved;
}

bool OptSingleStringExchange(vector<int>& path, InputData& input, double cur_time) {
	if (path.empty()) {
		return false;
	}
	auto zeros_info = PreparePathWithZeros(path);
	auto first_possible_to_move = FirstPossibleToMoveElement(path, input, cur_time);
	bool improved = false;

	double len_1 = CalcNewLenForGlobalOpt(input, path);
	for (size_t i1 = first_possible_to_move; i1 + 1 < path.size(); ++i1) {
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

	UndoPreparePathWithZeroz(path, zeros_info);
	return improved;
}

bool OptSingleStringRelocation(vector<int>& path, InputData& input, double cur_time) {
	if (path.empty()) {
		return false;
	}
	auto zeros_info = PreparePathWithZeros(path);

	auto first_possible_to_move = FirstPossibleToMoveElement(path, input, cur_time);

	bool improved = false;

	double len_1 = CalcNewLenForGlobalOpt(input, path);

	for (size_t l = first_possible_to_move; l + 1 < path.size(); ++l) {
		for (size_t r = l; r + 1 < path.size(); ++r) {
			for (size_t i = first_possible_to_move; i + 1 < path.size(); ++i) {
				if (i + 1 >= l && i <= r) {
					i = r;
					continue;
				}
				vector<int> cur_path{ path.begin(), path.end() };
				if (i < l) {
					cur_path.erase(cur_path.begin() + l, cur_path.begin() + r + 1);
					cur_path.insert(cur_path.begin() + i, path.begin() + l, path.begin() + r + 1);
				}
				else {
					cur_path.insert(cur_path.begin() + i, path.begin() + l, path.begin() + r + 1);
					cur_path.erase(cur_path.begin() + l, cur_path.begin() + r + 1);
				}

				double new_len = CalcNewLenForGlobalOpt(input, cur_path);
				if (new_len + EPS < len_1) {
					improved = true;
					len_1 = new_len;
					path = cur_path;
				}
			}
		}
	}

	UndoPreparePathWithZeroz(path, zeros_info);
	return improved;
}

bool OptStringExchange(MatrixInt& paths, InputData& input, double cur_time, size_t max_segment_length, map<int, double> broken_paths) {
	pair<bool, bool> zeros_info;
	for (auto& path : paths) {
		if (!path.empty()) {
			zeros_info = PreparePathWithZeros(path);
		}
	}

	bool improved = false;
	for (size_t path_id_1 = 0; path_id_1 < paths.size(); ++path_id_1) {
		if (paths[path_id_1].empty() || broken_paths.count(static_cast<int>(path_id_1))) {
			continue;
		}
		auto first_pos_1 = max((size_t)1, FirstPossibleToMoveElement(paths[path_id_1], input, cur_time));
		double path_1_len = CalcNewLenForGlobalOpt(input, paths[path_id_1]);
		for (size_t path_id_2 = path_id_1 + 1; path_id_2 < paths.size(); ++path_id_2) {
			if (paths[path_id_2].empty() || broken_paths.count(static_cast<int>(path_id_2))) {
				continue;
			}
			auto first_pos_2 = max((size_t)1, FirstPossibleToMoveElement(paths[path_id_2], input, cur_time));
			double path_2_len = CalcNewLenForGlobalOpt(input, paths[path_id_2]);

			bool improved_local = false;

			for (size_t l_1 = first_pos_1; l_1 < paths[path_id_1].size(); ++l_1) {
				if (improved_local) break;
				for (size_t r_1 = l_1; r_1 < 
					min(l_1 + max_segment_length + 1, paths[path_id_1].size()); ++r_1) {
					if (improved_local) break;

					auto path_1_copy(paths[path_id_1]);
					vector<int>path_1_mid{ paths[path_id_1].begin() + l_1, 
						paths[path_id_1].begin() + r_1 };
					path_1_copy.erase(path_1_copy.begin() + l_1, path_1_copy.begin() + r_1);

					for (size_t l_2 = first_pos_2; l_2 < paths[path_id_2].size(); ++l_2) {
						if (improved_local) break;
						for (size_t r_2 = l_2; r_2 < 
							min(l_2 + max_segment_length + 1, paths[path_id_2].size()); ++r_2) {
							if (improved_local) break;
							auto path_2_copy(paths[path_id_2]);
							path_2_copy.erase(path_2_copy.begin() + l_2, path_2_copy.begin() + r_2);
							path_2_copy.insert(path_2_copy.begin() + l_2,
								path_1_mid.begin(), path_1_mid.end());
							path_1_copy.insert(path_1_copy.begin() + l_1, paths[path_id_2].begin() + l_2, 
								paths[path_id_2].begin() + r_2);

							double new_path_1_len = CalcNewLenForGlobalOpt(input, path_1_copy);
							double new_path_2_len = CalcNewLenForGlobalOpt(input, path_2_copy);
							if (new_path_1_len + new_path_2_len < INF && 
								new_path_1_len + new_path_2_len + EPS < path_1_len + path_2_len) {
								paths[path_id_1] = path_1_copy;
								paths[path_id_2] = path_2_copy;
								path_1_len = new_path_1_len;
								path_2_len = new_path_2_len;
								improved_local = true;
							}
							path_1_copy.erase(path_1_copy.begin() + l_1, 
								path_1_copy.begin() + l_1 + (r_2 - l_2));
						}
					}
				}
			}
			improved |= improved_local;
		}
	}

	for (auto& path : paths) {
		if (!path.empty()) {
			UndoPreparePathWithZeroz(path, zeros_info);
		}
	}
	return improved;
}

bool OptStringCross(MatrixInt& paths, InputData& input, double cur_time, map<int, double> broken_paths) {
	pair<bool, bool> zeros_info;
	for (auto& path : paths) {
		if (!path.empty()) {
			zeros_info = PreparePathWithZeros(path);
		}
	}

	bool improved = false;
	for (size_t path_id_1 = 0; path_id_1 < paths.size(); ++path_id_1) {
		if (paths[path_id_1].empty() || broken_paths.count(static_cast<int>(path_id_1))) {
			continue;
		}
		auto first_pos_1 = max((size_t)1, 
			FirstPossibleToMoveElement(paths[path_id_1], input, cur_time));
		double path_1_len = CalcNewLenForGlobalOpt(input, paths[path_id_1]);
		for (size_t path_id_2 = path_id_1 + 1; path_id_2 < paths.size(); ++path_id_2) {
			if (paths[path_id_2].empty() || broken_paths.count(static_cast<int>(path_id_2))) {
				continue;
			}
			auto first_pos_2 = max((size_t)1, 
				FirstPossibleToMoveElement(paths[path_id_2], input, cur_time));
			double path_2_len = CalcNewLenForGlobalOpt(input, paths[path_id_2]);

			for (size_t i_1 = first_pos_1; i_1 + 1 < paths[path_id_1].size(); ++i_1) {
				for (size_t i_2 = first_pos_2; i_2 + 1 < paths[path_id_2].size(); ++i_2) {
					vector<int> path_1_copy{ paths[path_id_1].begin(), 
						paths[path_id_1].begin() + i_1 };
					copy(paths[path_id_2].begin() + i_2, paths[path_id_2].end(), 
						back_inserter(path_1_copy));
					vector<int> path_2_copy{ paths[path_id_2].begin(), 
						paths[path_id_2].begin() + i_2 };
					copy(paths[path_id_1].begin() + i_1, paths[path_id_1].end(), 
						back_inserter(path_2_copy));
					double new_path_1_len = CalcNewLenForGlobalOpt(input, path_1_copy);
					double new_path_2_len = CalcNewLenForGlobalOpt(input, path_2_copy);
					if (new_path_1_len + new_path_2_len < INF && 
						new_path_1_len + new_path_2_len + EPS < path_1_len + path_2_len) {
						paths[path_id_1] = path_1_copy;
						paths[path_id_2] = path_2_copy;
						path_1_len = new_path_1_len;
						path_2_len = new_path_2_len;
						improved = true;
					}
				}
			}
		}
	}

	for (auto& path : paths) {
		if (!path.empty()) {
			UndoPreparePathWithZeroz(path, zeros_info);
		}
	}
	return improved;
}


bool MultiOptimization(ProblemSolution& problem_solution, double cur_time, ETargetPathsChange target_paths_change) {
	auto& paths = problem_solution.Paths;
	auto& input = problem_solution.Input;

	bool improved = false;
	bool flag = true;
	double start_time = clock();
	while (flag) {
		if (clock() - start_time > 2.7 * CLOCKS_PER_SEC) {
			break;
		}
		flag = false;
		for (size_t i = 0; i < paths.size(); ++i) {
			if (problem_solution.BrokenVehicleTimeById.count(static_cast<int>(i))) {
				continue;
			}
			flag |= OptSingleStringExchange(paths[i], input, cur_time);
			flag |= OptSingleStringRelocation(paths[i], input, cur_time);
		}
		if (target_paths_change == ETargetPathsChange::ENABLE) {
			flag |= OptStringCross(paths, input, cur_time, problem_solution.BrokenVehicleTimeById);
			flag |= OptStringExchange(paths, input, cur_time, 10, problem_solution.BrokenVehicleTimeById);
		}
		improved |= flag;
	}
	return improved;
}
