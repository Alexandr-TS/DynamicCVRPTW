#include "AppForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void Main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	RunTests();

	VehicleRootingProblem::AppForm form;
	Application::Run(% form);
}
