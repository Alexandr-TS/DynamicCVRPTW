#include "Research.h"
#include "DataClasses.h"
#include "SolverMain.h"
#include "EventsHandler.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using namespace Research;
using namespace std;

constexpr int TESTS_PER_FILE = 10;

ofstream outlog("C:\\Users\\Admin\\OneDrive\\UNIVER\\11trimestr\\DynamicCVRPTW\\VehicleRootingProblem\\research_logs.txt");
mt19937 gen;
const vector<OptsConfig> configs { 
	{false, true, true, true}, 
	{true, false, true, true}, 
	{true, true, false, true}, 
	{true, true, true, false}, 

	{true, false, false, false}, 
	{false, true, false, false}, 
	{false, false, true, false}, 
	{false, false, false, true}, 

	{true, true, true, true}, 
	{false, false, false, false},
	{false, false, true, true}
};


namespace AppFormVars {
	struct Param {
		std::string Name;
		double Recommend;
		std::string Comment;

		Param() {}
		Param(std::string name, double recommend, std::string comment) :
			Name(name),
			Recommend(recommend),
			Comment(comment)
		{}
	};

	struct Algo {
		std::string Name;
		EAlgorithms Ealgo;
		std::vector<Param> Params;

		Algo() {}
		Algo(std::string name, EAlgorithms ealgo, std::vector<Param> params) :
			Name(name),
			Ealgo(ealgo),
			Params(params)
		{}
	};

	std::vector<Algo> Algos = {
		Algo("Муравьиный алгоритм", EAlgorithms::AntColony, {
			Param("Alpha", 3, "Показатель степени стойкости феромонов в формуле вероятности выбора ребра"),
			Param("Betta", 4, "Показатель степени функции видимости в формуле вероятности выбора ребра"),
			Param("Rho", 0.4, "Коэффициент стойкости феромнов"),
			Param("f", 2, "Коэффициент при расстоянии между вершинами в функции видимости"),
			Param("g", 3, "Коэффициент при разности расстояний до депо в функции видимости"),
			Param("Imax", 2, "Коэффициент количества итераций. Число итераций = Imax * n"),
			Param("Sigma", 5, "Число элитарных муравьев"),
			Param("CandList", 0.5, "Доля вершин в списке кандидатов на следующее ребро"),
			Param("TimeLimit", 0, "Ограничение по времени. Если не 0, то Imax не учитывается. Если 0, то Imax не учитывается")
		})
		, Algo("Генетический алгоритм", EAlgorithms::Genetic, {
			Param("N", 30, "Размер популяции"),
			Param("Alpha", 1500, "Максимальное количество изменений популяции"),
			Param("Betta", 900, "Максимальное количество итераций без улучшения лучшего решения"),
			Param("Delta", 0.5, "Минимальная разность фитнес-функций двух решений"),
			Param("P", 0.5, "Вероятность мутации"),
			Param("TimeLimit", 0, "Ограничение по времени. Если не 0, то Alpha и Betta не учитываются")
		})
		, Algo("Полный перебор", EAlgorithms::BruteForce, {})
	};
}


vector<InputData> MakeInputDatas(const string& directory_name) {
	filesystem::directory_iterator dir_iterator;
	try {
		dir_iterator = filesystem::directory_iterator(directory_name);
	}
	catch (const filesystem::filesystem_error&) {
		cout << "Error: No such directory: " << directory_name << ". ";
		cout << "Please check the directory path" << endl;
		return {};
	}

	vector<InputData> result;
	for (const auto& file : dir_iterator) {
		result.emplace_back(InputData(file.path().string()));
		outlog << "finished reading: " << file.path().string() << endl;
	}
	return result;
}

vector<ProblemSolution> BuildSolutions(const vector<InputData>& input_files) {
	using namespace AppFormVars;

	vector<ProblemSolution> result;
	int test_set_idx = 1;
	for (const auto& input: input_files) {
		ProblemMode mode = ProblemMode::MINSUM;
		int algoInd = 1;
		if (input.TargetsCnt <= 9) {
			algoInd = 2;
		}
		else if (input.TargetsCnt > 90) {
			algoInd = 0;
		}
		vector<double> args;
		for (int i = 0; i < static_cast<int>(Algos[algoInd].Params.size()); ++i) {
			args.push_back(Algos[algoInd].Params[i].Recommend);
		}
		
		result.push_back(SolverMain::Run(input, mode, Algos[algoInd].Ealgo, args));
		outlog << "finished building test #" << test_set_idx++ << " out of " << input_files.size() << endl;
	}
	return result;
}

void PrintResearchResults(vector<vector<double>>& results, ofstream& outlog) {
	for (auto& row : results) {
		for (auto el : row) {
			outlog << el << "\t";
		}
		outlog << "\n";
	}
	outlog << "\n";

	vector<double> sum_error(configs.size(), 0);
	vector<int> cnt_inf(configs.size(), 0);

	for (auto& row : results) {
		auto min_val = *min_element(row.begin(), row.end());
		for (int i = 0; i < static_cast<int>(configs.size()); ++i) {
			auto el = row[i];
			if (el >= INF - 1) {
				outlog << fixed << setprecision(2) << "INF\t";
				cnt_inf[i]++;
			}
			else {
				double error = (el - min_val) / min_val;
				sum_error[i] += error;
				outlog << fixed << setprecision(3) << 100. * error << "%\t";
			}
		}
		outlog << "\n";
	}
	outlog << "\n";

	for (auto el : cnt_inf) {
		outlog << el << " ";
		cout << el << " ";
	}
	cout << endl;
	outlog << "\n";
	for (int i = 0; i < static_cast<int>(sum_error.size()); ++i) {
		outlog << 100. * sum_error[i] / (results.size() - cnt_inf[i]) << "%\t";
	}
	outlog << "\n";
}

void ResearchVehicleBreakdown(const vector<ProblemSolution>& solutions, ofstream& outlog) {
	vector<vector<double>> results;
	for (const auto& solution : solutions) {
		assert(solution.SolutionExists);
		for (int test_ = 0; test_ < TESTS_PER_FILE; ++test_) {
			int vehicle_id = gen() % solution.Paths.size();
			double cur_time = static_cast<double>(gen() % (9 * 60) + 9 * 60);
			vector<double> results_row;
			for (size_t i = 0; i < configs.size(); ++i) {
				auto copy_solution = solution;
				try {
					EventsHandler::UpdateOnVehicleBreakdown(
						copy_solution, vehicle_id, cur_time, ETargetPathsChange::ENABLE, configs[i]);
				}
				catch (...) {
					copy_solution.SolutionExists = false;
				}
				if (!copy_solution.SolutionExists) {
					results_row.push_back(INF);
				}
				else {
					results_row.push_back(copy_solution.SumOfPathLengths);
				}
			}
			results.push_back(results_row);
		}
	}
	PrintResearchResults(results, outlog);
}

void ResearchTargetDelete(const vector<ProblemSolution>& solutions, ofstream& outlog) {
	vector<vector<double>> results;
	for (const auto& solution : solutions) {
		assert(solution.SolutionExists);
		for (int test_ = 0; test_ < TESTS_PER_FILE; ++test_) {

			int target_id = gen() % solution.Input.TargetsCnt + 1;
			double cur_time = static_cast<double>(gen() % (9 * 60) + 9 * 60);
			while (solution.Input.TimeWindows[target_id].first - 90 < cur_time) {
				target_id = gen() % solution.Input.TargetsCnt + 1;
				cur_time = static_cast<double>(gen() % (10 * 60) + 8 * 60);
			}

			vector<double> results_row;
			for (size_t i = 0; i < configs.size(); ++i) {
				auto copy_solution = solution;
				try {
					EventsHandler::UpdateOnRemoveTarget(
						copy_solution, target_id, cur_time, ETargetPathsChange::ENABLE, configs[i]);
				}
				catch (...) {
					copy_solution.SolutionExists = false;
				}
				if (!copy_solution.SolutionExists) {
					results_row.push_back(INF);
				}
				else {
					results_row.push_back(copy_solution.SumOfPathLengths);
				}
			}
			results.push_back(results_row);
		}
	}
	PrintResearchResults(results, outlog);
}

void ResearchTWUpdate(const vector<ProblemSolution>& solutions, ofstream& outlog) {
	vector<vector<double>> results;
	for (const auto& solution : solutions) {
		assert(solution.SolutionExists);
		for (int test_ = 0; test_ < TESTS_PER_FILE; ++test_) {

			int target_id = gen() % solution.Input.TargetsCnt + 1;
			double cur_time = static_cast<double>(gen() % (9 * 60) + 9 * 60);
			while (solution.Input.TimeWindows[target_id].first - 90 < cur_time) {
				target_id = gen() % solution.Input.TargetsCnt + 1;
				cur_time = static_cast<double>(gen() % (10 * 60) + 8 * 60);
			}

			double new_start = max(cur_time, static_cast<double>((gen() % 7 + 11) * 60));
			double new_end = max(cur_time + 60, static_cast<double>((gen() % 9 + 8) * 60));
			if (new_start > new_end) {
				swap(new_start, new_end);
			}
			if (new_end - new_start < 1) {
				new_end += 60;
			}

			vector<double> results_row;
			for (size_t i = 0; i < configs.size(); ++i) {
				auto copy_solution = solution;
				try {
					EventsHandler::UpdateOnTimeWindowUpdate(
						copy_solution, target_id, cur_time, new_start, new_end, ETargetPathsChange::ENABLE, configs[i]);
				}
				catch (...) {
					copy_solution.SolutionExists = false;
				}
				if (!copy_solution.SolutionExists) {
					results_row.push_back(INF);
				}
				else {
					results_row.push_back(copy_solution.SumOfPathLengths);
				}
			}
			results.push_back(results_row);
		}
	}
	PrintResearchResults(results, outlog);
}


void ResearchDistanceUpdate(const vector<ProblemSolution>& solutions, ofstream& outlog) {
	vector<vector<double>> results;
	for (const auto& solution : solutions) {
		assert(solution.SolutionExists);
		for (int test_ = 0; test_ < TESTS_PER_FILE; ++test_) {

			int target_id = gen() % solution.Input.TargetsCnt + 1;
			double cur_time = static_cast<double>(gen() % (9 * 60) + 9 * 60);
			while (solution.Input.TimeWindows[target_id].first - 90 < cur_time) {
				target_id = gen() % solution.Input.TargetsCnt + 1;
				cur_time = static_cast<double>(gen() % (9 * 60) + 9 * 60);
			}

			vector<DistanceToChange> dist_updates;

			int size_km = 20;
			pair<double, double> new_coords{ gen() % (1000 * size_km), gen() % (1000 * size_km) };
			const int basic_speed = 60;
			const int add_minutes = 3;
			exponential_distribution<> e_d_gen(1.5);
			for (size_t j = 0; j < solution.Input.Distances.size(); ++j) {
				double dist_m = hypot(
					solution.Input.Points[target_id].first - solution.Input.Points[j].first,
					solution.Input.Points[target_id].second - solution.Input.Points[j].second
				);
				double dist_minutes = dist_m / 1000. * 60.0 / (basic_speed / (1.0 + e_d_gen(gen)));
				if (target_id != j) {
					dist_minutes += add_minutes;
				}
				dist_updates.push_back({ target_id, static_cast<int>(j), dist_minutes });
				dist_updates.push_back({ static_cast<int>(j), target_id, dist_minutes });
			}

			vector<double> results_row;
			for (size_t i = 0; i < configs.size(); ++i) {
				auto copy_solution = solution;
				try {
					EventsHandler::UpdateOnDistMatrixUpdate(copy_solution, cur_time, dist_updates, ETargetPathsChange::ENABLE, configs[i]);
				}
				catch (...) {
					copy_solution.SolutionExists = false;
				}
				if (!copy_solution.SolutionExists) {
					results_row.push_back(INF);
				}
				else {
					results_row.push_back(copy_solution.SumOfPathLengths);
				}
			}
			results.push_back(results_row);
		}
	}
	PrintResearchResults(results, outlog);
}


void Research::RunResearch() {
	double start_time = clock();

	auto input_files = MakeInputDatas("C:\\Users\\Admin\\OneDrive\\UNIVER\\11trimestr\\DynamicCVRPTW\\InputGenerator\\tests");
	auto solutions = BuildSolutions(input_files);
	outlog << "Solutions build\nTime passed: " << (clock() - start_time) / CLOCKS_PER_SEC << endl;

	ofstream fout1("C:\\Users\\Admin\\OneDrive\\UNIVER\\11trimestr\\DynamicCVRPTW\\VehicleRootingProblem\\vehicle_breakdown_logs.txt");
	ResearchVehicleBreakdown(solutions, fout1);
	fout1.close();
	outlog << "Done research on Vehicle Breakdown" << endl;
	outlog << "Time passed: " << (clock() - start_time) / CLOCKS_PER_SEC << endl;

	ofstream fout2("C:\\Users\\Admin\\OneDrive\\UNIVER\\11trimestr\\DynamicCVRPTW\\VehicleRootingProblem\\target_delete_logs.txt");
	ResearchTargetDelete(solutions, fout2);
	fout2.close();
	outlog << "Done research on Targets Delete" << endl;
	outlog << "Time passed: " << (clock() - start_time) / CLOCKS_PER_SEC << endl;

	ofstream fout3("C:\\Users\\Admin\\OneDrive\\UNIVER\\11trimestr\\DynamicCVRPTW\\VehicleRootingProblem\\tw_update_logs.txt");
	ResearchTWUpdate(solutions, fout3);
	fout3.close();
	outlog << "Done research on TimeWindows Update" << endl;
	outlog << "Time passed: " << (clock() - start_time) / CLOCKS_PER_SEC << endl;

	ofstream fout4("C:\\Users\\Admin\\OneDrive\\UNIVER\\11trimestr\\DynamicCVRPTW\\VehicleRootingProblem\\dist_upd_logs.txt");
	ResearchDistanceUpdate(solutions, fout4);
	fout4.close();
	outlog << "Done research on Distances Update" << endl;
	outlog << "Time passed: " << (clock() - start_time) / CLOCKS_PER_SEC << endl;
}
