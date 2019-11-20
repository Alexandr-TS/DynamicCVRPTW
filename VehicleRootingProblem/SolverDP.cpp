//#include "SolverDP.h"
//
//ProblemSolution SolverDP::Run(InputData input, ProblemMode problemMode, std::vector<double>args) {
//	double startTime = (double)clock() / CLOCKS_PER_SEC;
//
//	input.DronsCnt = std::min(input.DronsCnt, input.TargetsCnt);
//
//	// DP for 1 dron
//	std::vector<std::vector<double> > dp1(1 << (input.TargetsCnt),
//		std::vector<double>(input.TargetsCnt, INF));
//	std::vector<std::vector<int> > prevDp1(1 << (input.TargetsCnt),
//		std::vector<int>(input.TargetsCnt, -1));
//
//	for (int i = 0; i < input.TargetsCnt; ++i)
//		dp1[(1 << i)][i] = input.Distance(0, i + 1);
//	for (int mask = 0; mask < (1 << (input.TargetsCnt)); ++mask) {
//		for (int curV = 0; curV < input.TargetsCnt; ++curV) {
//			if (dp1[mask][curV] == INF) continue;
//			for (int nextV = 0; nextV < input.TargetsCnt; ++nextV) {
//				if (!((1 << nextV) & mask)) {
//					auto newValue = dp1[mask][curV] + input.Distance(curV + 1, nextV + 1);
//					if (newValue < dp1[mask | (1 << nextV)][nextV]) {
//						dp1[mask | (1 << nextV)][nextV] = newValue;
//						prevDp1[mask | (1 << nextV)][nextV] = curV;
//					}
//				}
//			}
//		}
//	}
//
//	// finishing in depot
//	std::vector<int> optLastV((1 << (input.TargetsCnt)), -1);
//	std::vector<double> optMaskDist((1 << (input.TargetsCnt)), INF);
//	optMaskDist[0] = 0;
//
//	for (int mask = 0; mask < (1 << (input.TargetsCnt)); ++mask) {
//		for (int curV = 0; curV < input.TargetsCnt; ++curV)
//			dp1[mask][curV] += input.Distance(curV + 1, 0);
//		for (int curV = 0; curV < input.TargetsCnt; ++curV)
//			if (dp1[mask][curV] < optMaskDist[mask]) {
//				optLastV[mask] = curV;
//				optMaskDist[mask] = dp1[mask][curV];
//			}
//	}
//
//	// DP for many drons
//	std::vector<std::vector<double> > dp(input.DronsCnt + 1,
//		std::vector<double>((1 << (input.TargetsCnt)), INF));
//	dp[0][0] = 0;
//	std::vector<std::vector<int> > prevDp(input.DronsCnt + 1,
//		std::vector<int>((1 << (input.TargetsCnt)), -1));
//
//	MatrixInt paths;
//
//	for (int cntDrons = 1; cntDrons <= input.DronsCnt; ++cntDrons) {
//		for (int mask = 0; mask < (1 << (input.TargetsCnt)); ++mask) {
//			for (int subMask = mask; ; subMask = (subMask - 1) & mask) {
//				if (optMaskDist[subMask] - EPS > input.MaxDist)
//					continue;
//				if (problemMode == ProblemMode::MINSUM) {
//					if (dp[cntDrons][mask] > dp[cntDrons - 1][mask ^ subMask] + optMaskDist[subMask]) {
//						dp[cntDrons][mask] = dp[cntDrons - 1][mask ^ subMask] + optMaskDist[subMask];
//						prevDp[cntDrons][mask] = subMask;
//					}
//				}
//				else {
//					if (dp[cntDrons][mask] > std::max(dp[cntDrons - 1][mask ^ subMask], optMaskDist[subMask])) {
//						dp[cntDrons][mask] = std::max(dp[cntDrons - 1][mask ^ subMask], optMaskDist[subMask]);
//						prevDp[cntDrons][mask] = subMask;
//					}
//				}
//				if (!subMask)
//					break;
//			}
//		}
//	}
//
//	// Making answer
//	int curMask = (1 << (input.TargetsCnt)) - 1;
//	int curDronsCnt = input.DronsCnt;
//	if (prevDp[curDronsCnt][curMask] != -1) {
//		while (curMask) {
//			if (curDronsCnt == 0) break;
//			int subMask = prevDp[curDronsCnt][curMask];
//			assert(subMask >= 0);
//			std::vector<int> path;
//			int curV = optLastV[subMask];
//			int curSubMask = subMask;
//			while (curV != -1) {
//				path.push_back(curV + 1);
//				auto tmpMask = curSubMask ^ (1 << curV);
//				curV = prevDp1[curSubMask][curV];
//				curSubMask = tmpMask;
//			}
//
//			reverse(path.begin(), path.end());
//			paths.push_back(path);
//
//			curMask ^= subMask;
//			curDronsCnt--;
//		}
//	}
//
//	return ProblemSolution(input, paths);
//}
