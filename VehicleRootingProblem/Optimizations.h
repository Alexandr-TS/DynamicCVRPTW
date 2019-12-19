#pragma once

#include "DataClasses.h"
#include "Utils.h"

bool GlobalSwapOptimization(MatrixInt& paths, InputData& input);
bool GlobalInsertOptimization(MatrixInt& paths, InputData& input);
bool LocalSwapOptimization(std::vector<int>& path, InputData& input);