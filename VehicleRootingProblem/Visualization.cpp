#include "Visualization.h"
#include <string>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

void DrawPaths(System::Drawing::Graphics^ graphics, ProblemSolution solution, int height, int width, int cur_time) {
	int sumPoints = 0;
	for (auto path : solution.Paths) {
		sumPoints += path.size();
	}
	if (!sumPoints) {
		return;
	}
	double INF = 1e9;
	double minX = INF, maxX = -INF, minY = INF, maxY = -INF;
	for (auto pt : solution.Input.Points) {
		minX = std::min(minX, pt.first);
		maxX = std::max(maxX, pt.first);
		minY = std::min(minY, pt.second);
		maxY = std::max(maxY, pt.second);
	}
	int sz2 = std::min(height - 30, width - 30);
	int sz1 = (int)std::max(maxY - minY, maxX - minX);
	sz1 = std::max(sz1, 1);

	auto points = solution.Input.Points;
	for (auto& pt : points) {
		pt = std::make_pair((pt.first - minX) * sz2 / sz1 + 15, 7 + (pt.second - minY) * sz2 / sz1);
	}

	double newMinY = INF, newMaxY = -INF;
	for (auto pt : points) {
		newMinY = std::min(newMinY, pt.second);
		newMaxY = std::max(newMaxY, pt.second);
	}

	for (auto& pt : points) {
		pt.second = newMaxY - (pt.second - newMinY);
	}

	graphics->Clear(Color::White);

	for (auto path : solution.Paths) {
		path.push_back(0);
		path.push_back(0);
		rotate(path.begin(), path.begin() + path.size() - 1, path.end());
		// blue
		auto pen = (gcnew Pen(Color::FromArgb(120, 10, 10, 255), 1));
		for (int i = 0; i + 1 < (int)path.size(); ++i) {
			graphics->DrawLine(pen, (int)points[path[i]].first, (int)points[path[i]].second,
									(int)points[path[i + 1]].first, (int)points[path[i + 1]].second);
		}

		double left_time = cur_time;
		// red
		pen = (gcnew Pen(Color::FromArgb(180, 10, 10, 255), 2));
		for (int i = 0; i + 1 < (int)path.size(); ++i) {
			double len = solution.Input.Distance(path[i], path[i + 1]);
			if (len <= left_time) {
				left_time -= len;
				graphics->DrawLine(pen, (int)points[path[i]].first, (int)points[path[i]].second,
									(int)points[path[i + 1]].first, (int)points[path[i + 1]].second);
			}
			else {
				auto second_point = points[path[i]];
				auto move_vector = std::make_pair(points[path[i + 1]].first - points[path[i]].first, 
										     points[path[i + 1]].second - points[path[i]].second);
				second_point.first += move_vector.first * left_time / len;
				second_point.second += move_vector.second * left_time / len;
				graphics->DrawLine(pen, (int)points[path[i]].first, (int)points[path[i]].second,
					(int)second_point.first, (int)second_point.second);
				break;
			}
		}
	}

	auto pen = (gcnew Pen(Color::FromArgb(0, 255, 0), 3));
	for (auto& path : solution.Paths) {
		for (size_t i = 0; i < path.size(); ++i) {
			auto pt = points[path[i]];
			graphics->DrawEllipse(pen, Rectangle((int)pt.first - 2, (int)pt.second - 2, 4, 4));

			std::string text_str = std::to_string(path[i]);
			auto text = System::String(text_str.c_str()).ToString();

			auto drawFont = gcnew Font("Arial", 12);
			auto drawBrush = gcnew SolidBrush(Color::Black);
			float x = pt.first - 1;
			float y = pt.second + 5;
			auto drawFormat = gcnew StringFormat();
			graphics->DrawString(text, drawFont, drawBrush, x, y, drawFormat);
		}
	}
	pen = (gcnew Pen(Color::FromArgb(0, 0, 0), 3));
	
	graphics->DrawEllipse(pen, Rectangle((int)points[0].first - 2, (int)points[0].second - 2, 4, 4));
}