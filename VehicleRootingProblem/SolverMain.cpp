#include "SolverMain.h"
#include "SolverClarkeWright.h"
#include "SolverDP.h"

ProblemSolution SolverMain::Run(InputData input, double timeLimit, ProblemMode problemMode, EAlgorithms algorithm) {
	if (algorithm == EAlgorithms::DP) {
		return SolverDP::Run(input, timeLimit, problemMode);
	} else {
		return SolverClarkeWright::Run(input, timeLimit, problemMode);
	}
}
