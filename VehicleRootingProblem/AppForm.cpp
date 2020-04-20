#include "AppForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	AllocConsole();
	//FILE* stream;
	//freopen_s(&stream, "CONOUT$", "w", stdout);
	//RunTests();

	VehicleRootingProblem::AppForm form;
	Application::Run(% form);
	return 0;
}
