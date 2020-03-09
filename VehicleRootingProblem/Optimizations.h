#pragma once

#include "DataClasses.h"
#include "Utils.h"

// For first paths build
bool GlobalSwapOptimization(MatrixInt& paths, InputData& input);
bool GlobalInsertOptimization(MatrixInt& paths, InputData& input);
bool LocalSwapOptimization(std::vector<int>& path, InputData& input);

// For paths build during the process
// TODO 
bool OptStringCross(MatrixInt& paths, InputData& input);
// Currently implemented partly - mix of GlobalSwapOpt and GlobalInsertOpt
bool OptStringExchange(MatrixInt& paths, InputData& input);
bool OptSingleStringRelocation(std::vector<int>& path, InputData& input);
bool OptSingleStringExchange(std::vector<int>& path, InputData& input);
