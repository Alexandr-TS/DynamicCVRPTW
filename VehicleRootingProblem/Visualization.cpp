#include "Visualization.h"
#include <string>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace std;

void TranslatePoint(pair<double, double>& pt, double minX, 
	int sz2, int sz1, double minY, double newMaxY, double newMinY) {
	pt = make_pair((pt.first - minX) * sz2 / sz1 + 15, 7 + (pt.second - minY) * sz2 / sz1);
	pt.second = newMaxY - (pt.second - newMinY);
}

void DrawPaths(System::Drawing::Graphics^ graphics, ProblemSolution solution, int height, int width, 
	int cur_time, bool update_only_progress) {
	int sumPoints = 0;
	for (auto path : solution.Paths) {
		sumPoints += static_cast<int>(path.size());
	}
	if (!sumPoints) {
		return;
	}
	double INF = 1e9;
	double minX = INF, maxX = -INF, minY = INF, maxY = -INF;
	for (auto pt : solution.Input.Points) {
		minX = min(minX, pt.first);
		maxX = max(maxX, pt.first);
		minY = min(minY, pt.second);
		maxY = max(maxY, pt.second);
	}
	int sz2 = min(height - 30, width - 30);
	int sz1 = static_cast<int>(max(maxY - minY, maxX - minX));
	sz1 = max(sz1, 1);

	auto points = solution.Input.Points;
	for (auto& pt : points) {
		pt = make_pair((pt.first - minX) * sz2 / sz1 + 15, 7 + (pt.second - minY) * sz2 / sz1);
	}

	double newMinY = INF, newMaxY = -INF;
	for (auto pt : points) {
		newMinY = min(newMinY, pt.second);
		newMaxY = max(newMaxY, pt.second);
	}

	for (auto& pt : points) {
		pt.second = newMaxY - (pt.second - newMinY);
	}

	if (!update_only_progress) {
		graphics->Clear(Color::White);
	}

	size_t path_idx = 0;
	for (auto path : solution.Paths) {
		if (path.empty()) {
			++path_idx;
			continue;
		}
		path.push_back(0);
		path.push_back(0);
		rotate(path.begin(), path.begin() + path.size() - 1, path.end());
		auto pen = (gcnew Pen(Color::FromArgb(120, 10, 10, 255), 1));

		if (!update_only_progress) {
			int last_i = static_cast<int>(path.size()) - 2;
			if (solution.BrokenVehicleTimeById.count(static_cast<int>(path_idx))) {
				last_i -= 2;
			}
			for (int i = 0; i <= last_i; ++i) {
				graphics->DrawLine(pen, static_cast<int>(points[path[i]].first), static_cast<int>(points[path[i]].second),
					static_cast<int>(points[path[i + 1]].first), static_cast<int>(points[path[i + 1]].second));
			}
			if (solution.BrokenVehicleTimeById.count(static_cast<int>(path_idx))) {
				auto cur_pt = GetVehicleCoords(solution, static_cast<int>(path_idx), solution.BrokenVehicleTimeById[static_cast<int>(path_idx)]);
				TranslatePoint(cur_pt, minX, sz2, sz1, minY, newMaxY, newMinY);
				int last_visited_id = static_cast<int>(path.size()) - 3;
				graphics->DrawLine(pen, static_cast<int>(points[path[last_visited_id]].first), static_cast<int>(points[path[last_visited_id]].second),
					static_cast<int>(cur_pt.first), static_cast<int>(cur_pt.second));
			}
		}

		pen = (gcnew Pen(Color::FromArgb(180, 10, 10, 255), 2));
		auto arrival_times = solution.ArrivalTimes[path_idx];
		arrival_times.push_back(arrival_times.back() + solution.Input.Distance(path[path.size() - 2], 0));
		double real_time = cur_time;
		if (solution.BrokenVehicleTimeById.count(static_cast<int>(path_idx))) {
			real_time = solution.BrokenVehicleTimeById[static_cast<int>(path_idx)];
		}
		for (size_t i = 1; i < path.size(); ++i) {
			if (arrival_times[i - 1] <= real_time) {
				graphics->DrawLine(pen, static_cast<int>(points[path[i - 1]].first), static_cast<int>(points[path[i - 1]].second),
					static_cast<int>(points[path[i]].first), static_cast<int>(points[path[i]].second));
			}
			else {
				auto cur_pt = GetVehicleCoords(solution, static_cast<int>(path_idx), real_time);
				TranslatePoint(cur_pt, minX, sz2, sz1, minY, newMaxY, newMinY);
				graphics->DrawLine(pen, static_cast<int>(points[path[i - 1]].first), static_cast<int>(points[path[i - 1]].second),
					static_cast<int>(cur_pt.first), static_cast<int>(cur_pt.second));
				break;
			}
		}

		++path_idx;
	}


	if (!update_only_progress) {
		auto pen = (gcnew Pen(Color::FromArgb(0, 255, 0), 3));
		for (auto& path : solution.Paths) {
			for (size_t i = 0; i < path.size(); ++i) {
				auto pt = points[path[i]];
				// dummy copies of depot
				if (abs(pt.first - points[0].first) < EPS && abs(pt.second - points[0].second) < EPS) {
					continue;
				}
				graphics->DrawEllipse(pen, Rectangle(static_cast<int>(pt.first) - 2, static_cast<int>(pt.second) - 2, 4, 4));

				string text_str = to_string(path[i]);
				auto text = System::String(text_str.c_str()).ToString();

				auto drawFont = gcnew Font("Arial", 12);
				auto drawBrush = gcnew SolidBrush(Color::Black);
				float x = static_cast<float>(pt.first - 1.0);
				float y = static_cast<float>(pt.second + 5.0);
				auto drawFormat = gcnew StringFormat();
				graphics->DrawString(text, drawFont, drawBrush, x, y, drawFormat);
			}
		}

		pen = (gcnew Pen(Color::FromArgb(255, 0, 0), 3));
		for (auto [vehicle_id, break_time] : solution.BrokenVehicleTimeById) {
			auto cur_pt = GetVehicleCoords(solution, vehicle_id, break_time);
			TranslatePoint(cur_pt, minX, sz2, sz1, minY, newMaxY, newMinY);
			graphics->DrawEllipse(pen, Rectangle(static_cast<int>(cur_pt.first) - 2, static_cast<int>(cur_pt.second) - 2, 4, 4));
		}

		pen = (gcnew Pen(Color::FromArgb(0, 0, 0), 3));
		graphics->DrawEllipse(pen, Rectangle(static_cast<int>(points[0].first) - 2, static_cast<int>(points[0].second) - 2, 4, 4));
	}
}