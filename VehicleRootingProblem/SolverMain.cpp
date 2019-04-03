#include "SolverMain.h"
#include "SolverClarkeWright.h"
#include "SolverDP.h"

ProblemSolution SolverMain::Run(InputData input, double timeLimit, ProblemMode problemMode) {
	if (pow(3, input.TargetsCnt) * input.DronsCnt < 5e8) {
		return SolverDP::Run(input, timeLimit, problemMode);
	} else {
		return SolverClarkeWright::Run(input, timeLimit, problemMode);
	}
}
