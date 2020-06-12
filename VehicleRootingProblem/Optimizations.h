#pragma once

#include "DataClasses.h"
#include "Utils.h"

struct OptsConfig {
	OptsConfig() = default;
	OptsConfig(double multiopt_tl)
		: multiopt_tl(multiopt_tl)
	{
	}

	bool use_string_cross = true;
	bool use_string_exchange = true;
	bool use_single_string_relocation = true;
	bool use_single_string_exchange = true;
	double multiopt_tl = 2.7;
};

// For first paths build
bool GlobalSwapOptimization(MatrixInt& paths, InputData& input);
bool GlobalInsertOptimization(MatrixInt& paths, InputData& input);
bool LocalSwapOptimization(std::vector<int>& path, InputData& input);

// For paths build during the process
bool OptStringCross(MatrixInt& paths, InputData& input, double cur_time, std::map<int, double> broken_paths = {});
bool OptStringExchange(MatrixInt& paths, InputData& input, double cur_time, size_t max_segment_length = 3, std::map<int, double> broken_paths = {});
bool OptSingleStringRelocation(std::vector<int>& path, InputData& input, double cur_time);
bool OptSingleStringExchange(std::vector<int>& path, InputData& input, double cur_time);

bool MultiOptimization(ProblemSolution& problem_solution, double cur_time, ETargetPathsChange target_paths_change, OptsConfig opts_config);
