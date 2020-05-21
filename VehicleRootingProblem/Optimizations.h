#pragma once

#include "DataClasses.h"
#include "Utils.h"

// For first paths build
bool GlobalSwapOptimization(MatrixInt& paths, InputData& input);
bool GlobalInsertOptimization(MatrixInt& paths, InputData& input);
bool LocalSwapOptimization(std::vector<int>& path, InputData& input);

// For paths build during the process
bool OptStringCross(MatrixInt& paths, InputData& input, double cur_time);
bool OptStringExchange(MatrixInt& paths, InputData& input, double cur_time, size_t max_segment_length = 3);
bool OptSingleStringRelocation(std::vector<int>& path, InputData& input, double cur_time);
bool OptSingleStringExchange(std::vector<int>& path, InputData& input, double cur_time);

bool MultiOptimization(MatrixInt& paths, InputData& input, double cur_time, ETargetPathsChange target_paths_change);
