#pragma once

#include "Tests.h"
#include "DataClasses.h"
#include "SolverMain.h"
#include "Visualization.h"

#include <memory>
#include <msclr\marshal_cppstd.h>
#include <cstring>
#include <string>
#include <algorithm>

namespace VehicleRootingProblem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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

		struct Launch {
			int AlgoIndex;
			int DataSetIndex;
			std::vector<double> ParamsVals;
			ProblemMode Mode;
			ProblemSolution Solution;
			
			Launch() {}
			Launch(int algoIndex, int dataSetIndex, std::vector<double> paramsVals, ProblemMode mode, ProblemSolution solution) :
				AlgoIndex(algoIndex),
				DataSetIndex(dataSetIndex),
				ParamsVals(paramsVals),
				Mode(mode),
				Solution(solution)
			{}
		};

		std::vector<InputData> DataSets;
		std::vector<Algo> Algos = {
			Algo("������ ��������", EAlgorithms::Greedy, {})
			, Algo("���������� ��������", EAlgorithms::AntColony, {
				Param("Alpha", 3, "���������� ������� ��������� ��������� � ������� ����������� ������ �����"),
				Param("Betta", 4, "���������� ������� ������� ��������� � ������� ����������� ������ �����"),
				Param("Rho", 0.4, "����������� ��������� ��������"),
				Param("f", 2, "����������� ��� ���������� ����� ��������� � ������� ���������"),
				Param("g", 3, "����������� ��� �������� ���������� �� ���� � ������� ���������"),
				Param("Imax", 2, "����������� ���������� ��������. ����� �������� = Imax * n"),
				Param("Sigma", 5, "����� ��������� ��������"),
				Param("CandList", 0.5, "���� ������ � ������ ���������� �� ��������� �����"),
				Param("TimeLimit", 0, "����������� �� �������. ���� �� 0, �� Imax �� �����������. ���� 0, �� Imax �� �����������")
			})
			, Algo("������������ ��������", EAlgorithms::Genetic, {
				Param("N", 30, "������ ���������"),
				Param("Alpha", 1500, "������������ ���������� ��������� ���������"),
				Param("Betta", 900, "������������ ���������� �������� ��� ��������� ������� �������"),
				Param("Delta", 0.5, "����������� �������� ������-������� ���� �������"),
				Param("P", 0.5, "����������� �������"),
				Param("TimeLimit", 0, "����������� �� �������. ���� �� 0, �� Alpha � Betta �� �����������")
			})
			, Algo("������ �������", EAlgorithms::BruteForce, {})
		};
		std::vector<Launch> Launches;
		InputData LoadedInputData;
		ProblemSolution CurrentSolution;
		int PrintedLaunchPathsIndex;
	}

	System::String^ ToText(std::string s) {
		return System::String(s.c_str()).ToString();
	}

	System::String^ ToText(int x) {
		return System::Int32(x).ToString();
	}

	System::String^ ToText(double x) {
		return System::Double(x).ToString()->Replace(",", ".");
	}

	/// <summary>
	/// ������ ��� AppForm
	/// </summary>
	public ref class AppForm : public System::Windows::Forms::Form
	{


	public:
		AppForm(void)
		{
			InitializeComponent();
			this->Width = 600;
			groupBoxResults->Visible = false;

			listViewLoadedDataSets->Items->Clear();

			this->buttonSavePathsToFile->Enabled = false;
			buttonUploadFile->Enabled = false;
			buttonRun->Enabled = false;
			buttonRunProcess->Enabled = false;
			
			AppFormVars::PrintedLaunchPathsIndex = -1;

			Graphics = this->pictureBoxRes->CreateGraphics();
		}
	private: System::Windows::Forms::Button^ buttonRunProcess;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::MaskedTextBox^ tbTime;
	private: System::Windows::Forms::GroupBox^ groupBoxEvents;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::NumericUpDown^ numericUpDownYCoord;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::NumericUpDown^ numericUpDownXCoord;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::NumericUpDown^ numericUpDownTargetId;
	private: System::Windows::Forms::Button^ butUpdTW;
	private: System::Windows::Forms::MaskedTextBox^ tbTWClose;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::MaskedTextBox^ tbTWOpen;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ butDeleteTarget;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ butX2;
	private: System::Windows::Forms::Button^ butX3;
	private: System::Windows::Forms::Button^ butX1;
	private: System::Windows::Forms::Button^ butPause;


	public:
		System::Drawing::Graphics^ Graphics;

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~AppForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^ tabControlLeft;
	protected:

	protected:
	private: System::Windows::Forms::TabPage^ tabPage1;
	private: System::Windows::Forms::ListView^ listViewLoadedDataSets;

	private: System::Windows::Forms::ColumnHeader^ colName;
	private: System::Windows::Forms::ColumnHeader^ colDrons;
	private: System::Windows::Forms::ColumnHeader^ colTargets;
	private: System::Windows::Forms::ColumnHeader^ colMaxDist;
	private: System::Windows::Forms::TabPage^ tabPage2;







	private: System::Windows::Forms::GroupBox^ groupBoxLoadedDataSets;







	private: System::Windows::Forms::Button^ buttonRun;






	private: System::Windows::Forms::Button^ buttonFileOpen;
	private: System::Windows::Forms::Label^ labelLoadedFile;

	private: System::Windows::Forms::Button^ buttonUploadFile;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog;
	private: System::Windows::Forms::GroupBox^ groupBoxResults;




	private: System::Windows::Forms::Label^ labelPathsList;
	private: System::Windows::Forms::DataGridView^ dataGridViewPaths;

	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column2;
	private: System::Windows::Forms::Button^ buttonSavePathsToFile;
	private: System::Windows::Forms::PictureBox^ pictureBoxRes;




	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog;
	private: System::Windows::Forms::DataGridView^ dataGridViewSelectedRes;


	private: System::ComponentModel::IContainer^ components;




	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::ListViewItem^ listViewItem1 = (gcnew System::Windows::Forms::ListViewItem(L""));
			this->tabControlLeft = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->buttonRunProcess = (gcnew System::Windows::Forms::Button());
			this->buttonRun = (gcnew System::Windows::Forms::Button());
			this->groupBoxLoadedDataSets = (gcnew System::Windows::Forms::GroupBox());
			this->labelLoadedFile = (gcnew System::Windows::Forms::Label());
			this->buttonUploadFile = (gcnew System::Windows::Forms::Button());
			this->buttonFileOpen = (gcnew System::Windows::Forms::Button());
			this->listViewLoadedDataSets = (gcnew System::Windows::Forms::ListView());
			this->colName = (gcnew System::Windows::Forms::ColumnHeader());
			this->colDrons = (gcnew System::Windows::Forms::ColumnHeader());
			this->colTargets = (gcnew System::Windows::Forms::ColumnHeader());
			this->colMaxDist = (gcnew System::Windows::Forms::ColumnHeader());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->dataGridViewSelectedRes = (gcnew System::Windows::Forms::DataGridView());
			this->pictureBoxRes = (gcnew System::Windows::Forms::PictureBox());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBoxResults = (gcnew System::Windows::Forms::GroupBox());
			this->tbTime = (gcnew System::Windows::Forms::MaskedTextBox());
			this->groupBoxEvents = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->numericUpDownYCoord = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->numericUpDownXCoord = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->numericUpDownTargetId = (gcnew System::Windows::Forms::NumericUpDown());
			this->butUpdTW = (gcnew System::Windows::Forms::Button());
			this->tbTWClose = (gcnew System::Windows::Forms::MaskedTextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->tbTWOpen = (gcnew System::Windows::Forms::MaskedTextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->butDeleteTarget = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->buttonSavePathsToFile = (gcnew System::Windows::Forms::Button());
			this->butX2 = (gcnew System::Windows::Forms::Button());
			this->butX3 = (gcnew System::Windows::Forms::Button());
			this->butX1 = (gcnew System::Windows::Forms::Button());
			this->butPause = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->labelPathsList = (gcnew System::Windows::Forms::Label());
			this->dataGridViewPaths = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->tabControlLeft->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBoxLoadedDataSets->SuspendLayout();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewSelectedRes))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxRes))->BeginInit();
			this->groupBoxResults->SuspendLayout();
			this->groupBoxEvents->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownYCoord))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownXCoord))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownTargetId))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewPaths))->BeginInit();
			this->SuspendLayout();
			// 
			// tabControlLeft
			// 
			this->tabControlLeft->Controls->Add(this->tabPage1);
			this->tabControlLeft->Controls->Add(this->tabPage2);
			this->tabControlLeft->Location = System::Drawing::Point(12, 12);
			this->tabControlLeft->Name = L"tabControlLeft";
			this->tabControlLeft->SelectedIndex = 0;
			this->tabControlLeft->Size = System::Drawing::Size(567, 596);
			this->tabControlLeft->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->buttonRunProcess);
			this->tabPage1->Controls->Add(this->buttonRun);
			this->tabPage1->Controls->Add(this->groupBoxLoadedDataSets);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(559, 570);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"������������";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// buttonRunProcess
			// 
			this->buttonRunProcess->Location = System::Drawing::Point(207, 318);
			this->buttonRunProcess->Margin = System::Windows::Forms::Padding(2);
			this->buttonRunProcess->Name = L"buttonRunProcess";
			this->buttonRunProcess->Size = System::Drawing::Size(145, 39);
			this->buttonRunProcess->TabIndex = 23;
			this->buttonRunProcess->Text = L"��������� ��������";
			this->buttonRunProcess->UseVisualStyleBackColor = true;
			this->buttonRunProcess->Click += gcnew System::EventHandler(this, &AppForm::buttonRunProcess_Click);
			// 
			// buttonRun
			// 
			this->buttonRun->Location = System::Drawing::Point(12, 319);
			this->buttonRun->Margin = System::Windows::Forms::Padding(2);
			this->buttonRun->Name = L"buttonRun";
			this->buttonRun->Size = System::Drawing::Size(145, 39);
			this->buttonRun->TabIndex = 22;
			this->buttonRun->Text = L"��������� ����������";
			this->buttonRun->UseVisualStyleBackColor = true;
			this->buttonRun->Click += gcnew System::EventHandler(this, &AppForm::ButtonRun_Click);
			// 
			// groupBoxLoadedDataSets
			// 
			this->groupBoxLoadedDataSets->Controls->Add(this->labelLoadedFile);
			this->groupBoxLoadedDataSets->Controls->Add(this->buttonUploadFile);
			this->groupBoxLoadedDataSets->Controls->Add(this->buttonFileOpen);
			this->groupBoxLoadedDataSets->Controls->Add(this->listViewLoadedDataSets);
			this->groupBoxLoadedDataSets->Location = System::Drawing::Point(6, 6);
			this->groupBoxLoadedDataSets->Name = L"groupBoxLoadedDataSets";
			this->groupBoxLoadedDataSets->Size = System::Drawing::Size(545, 304);
			this->groupBoxLoadedDataSets->TabIndex = 2;
			this->groupBoxLoadedDataSets->TabStop = false;
			this->groupBoxLoadedDataSets->Text = L"�������� ������� ������";
			// 
			// labelLoadedFile
			// 
			this->labelLoadedFile->AutoSize = true;
			this->labelLoadedFile->Location = System::Drawing::Point(125, 268);
			this->labelLoadedFile->MaximumSize = System::Drawing::Size(95, 13);
			this->labelLoadedFile->Name = L"labelLoadedFile";
			this->labelLoadedFile->Size = System::Drawing::Size(92, 13);
			this->labelLoadedFile->TabIndex = 11;
			this->labelLoadedFile->Text = L"���� �� ������";
			// 
			// buttonUploadFile
			// 
			this->buttonUploadFile->Location = System::Drawing::Point(223, 263);
			this->buttonUploadFile->Name = L"buttonUploadFile";
			this->buttonUploadFile->Size = System::Drawing::Size(161, 23);
			this->buttonUploadFile->TabIndex = 10;
			this->buttonUploadFile->Text = L"���������";
			this->buttonUploadFile->UseVisualStyleBackColor = true;
			this->buttonUploadFile->Click += gcnew System::EventHandler(this, &AppForm::ButtonUploadFile_Click);
			// 
			// buttonFileOpen
			// 
			this->buttonFileOpen->Location = System::Drawing::Point(5, 264);
			this->buttonFileOpen->Margin = System::Windows::Forms::Padding(2);
			this->buttonFileOpen->Name = L"buttonFileOpen";
			this->buttonFileOpen->Size = System::Drawing::Size(115, 22);
			this->buttonFileOpen->TabIndex = 9;
			this->buttonFileOpen->Text = L"�������� ����";
			this->buttonFileOpen->UseVisualStyleBackColor = true;
			this->buttonFileOpen->Click += gcnew System::EventHandler(this, &AppForm::ButtonFileOpen_Click);
			// 
			// listViewLoadedDataSets
			// 
			this->listViewLoadedDataSets->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {
				this->colName,
					this->colDrons, this->colTargets, this->colMaxDist
			});
			this->listViewLoadedDataSets->FullRowSelect = true;
			this->listViewLoadedDataSets->GridLines = true;
			this->listViewLoadedDataSets->HideSelection = false;
			this->listViewLoadedDataSets->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(1) { listViewItem1 });
			this->listViewLoadedDataSets->Location = System::Drawing::Point(6, 19);
			this->listViewLoadedDataSets->MultiSelect = false;
			this->listViewLoadedDataSets->Name = L"listViewLoadedDataSets";
			this->listViewLoadedDataSets->Size = System::Drawing::Size(533, 238);
			this->listViewLoadedDataSets->TabIndex = 0;
			this->listViewLoadedDataSets->UseCompatibleStateImageBehavior = false;
			this->listViewLoadedDataSets->View = System::Windows::Forms::View::Details;
			this->listViewLoadedDataSets->SelectedIndexChanged += gcnew System::EventHandler(this, &AppForm::ListViewLoadedDataSets_SelectedIndexChanged);
			// 
			// colName
			// 
			this->colName->Text = L"�����";
			this->colName->Width = 125;
			// 
			// colDrons
			// 
			this->colDrons->Text = L"���������� ��";
			this->colDrons->Width = 108;
			// 
			// colTargets
			// 
			this->colTargets->Text = L"���������� �����";
			this->colTargets->Width = 109;
			// 
			// colMaxDist
			// 
			this->colMaxDist->Text = L"������������ ��������� �����";
			this->colMaxDist->Width = 184;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->dataGridViewSelectedRes);
			this->tabPage2->Controls->Add(this->pictureBoxRes);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(559, 570);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"������������";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// dataGridViewSelectedRes
			// 
			this->dataGridViewSelectedRes->AllowUserToAddRows = false;
			this->dataGridViewSelectedRes->AllowUserToDeleteRows = false;
			this->dataGridViewSelectedRes->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridViewSelectedRes->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewSelectedRes->Location = System::Drawing::Point(6, 3);
			this->dataGridViewSelectedRes->Name = L"dataGridViewSelectedRes";
			this->dataGridViewSelectedRes->ReadOnly = true;
			this->dataGridViewSelectedRes->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders;
			this->dataGridViewSelectedRes->Size = System::Drawing::Size(547, 82);
			this->dataGridViewSelectedRes->TabIndex = 32;
			// 
			// pictureBoxRes
			// 
			this->pictureBoxRes->Location = System::Drawing::Point(4, 91);
			this->pictureBoxRes->Name = L"pictureBoxRes";
			this->pictureBoxRes->Size = System::Drawing::Size(549, 473);
			this->pictureBoxRes->TabIndex = 0;
			this->pictureBoxRes->TabStop = false;
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog1";
			// 
			// groupBoxResults
			// 
			this->groupBoxResults->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->groupBoxResults->Controls->Add(this->tbTime);
			this->groupBoxResults->Controls->Add(this->groupBoxEvents);
			this->groupBoxResults->Controls->Add(this->buttonSavePathsToFile);
			this->groupBoxResults->Controls->Add(this->butX2);
			this->groupBoxResults->Controls->Add(this->butX3);
			this->groupBoxResults->Controls->Add(this->butX1);
			this->groupBoxResults->Controls->Add(this->butPause);
			this->groupBoxResults->Controls->Add(this->label1);
			this->groupBoxResults->Controls->Add(this->labelPathsList);
			this->groupBoxResults->Controls->Add(this->dataGridViewPaths);
			this->groupBoxResults->Location = System::Drawing::Point(591, 0);
			this->groupBoxResults->MaximumSize = System::Drawing::Size(1200, 718);
			this->groupBoxResults->Name = L"groupBoxResults";
			this->groupBoxResults->Size = System::Drawing::Size(562, 608);
			this->groupBoxResults->TabIndex = 1;
			this->groupBoxResults->TabStop = false;
			this->groupBoxResults->Text = L"����������";
			// 
			// tbTime
			// 
			this->tbTime->Location = System::Drawing::Point(119, 385);
			this->tbTime->Mask = L"00:00";
			this->tbTime->Name = L"tbTime";
			this->tbTime->Size = System::Drawing::Size(38, 20);
			this->tbTime->TabIndex = 40;
			this->tbTime->ValidatingType = System::DateTime::typeid;
			// 
			// groupBoxEvents
			// 
			this->groupBoxEvents->Controls->Add(this->button1);
			this->groupBoxEvents->Controls->Add(this->numericUpDownYCoord);
			this->groupBoxEvents->Controls->Add(this->label7);
			this->groupBoxEvents->Controls->Add(this->numericUpDownXCoord);
			this->groupBoxEvents->Controls->Add(this->label6);
			this->groupBoxEvents->Controls->Add(this->label5);
			this->groupBoxEvents->Controls->Add(this->numericUpDownTargetId);
			this->groupBoxEvents->Controls->Add(this->butUpdTW);
			this->groupBoxEvents->Controls->Add(this->tbTWClose);
			this->groupBoxEvents->Controls->Add(this->label4);
			this->groupBoxEvents->Controls->Add(this->tbTWOpen);
			this->groupBoxEvents->Controls->Add(this->label3);
			this->groupBoxEvents->Controls->Add(this->butDeleteTarget);
			this->groupBoxEvents->Controls->Add(this->label2);
			this->groupBoxEvents->Location = System::Drawing::Point(9, 421);
			this->groupBoxEvents->Name = L"groupBoxEvents";
			this->groupBoxEvents->Size = System::Drawing::Size(515, 179);
			this->groupBoxEvents->TabIndex = 36;
			this->groupBoxEvents->TabStop = false;
			this->groupBoxEvents->Text = L"�������";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(283, 142);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(146, 24);
			this->button1->TabIndex = 51;
			this->button1->Text = L"�������� ����������";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// numericUpDownYCoord
			// 
			this->numericUpDownYCoord->Location = System::Drawing::Point(214, 146);
			this->numericUpDownYCoord->Name = L"numericUpDownYCoord";
			this->numericUpDownYCoord->Size = System::Drawing::Size(56, 20);
			this->numericUpDownYCoord->TabIndex = 50;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(190, 148);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(21, 13);
			this->label7->TabIndex = 49;
			this->label7->Text = L"y =";
			// 
			// numericUpDownXCoord
			// 
			this->numericUpDownXCoord->Location = System::Drawing::Point(123, 146);
			this->numericUpDownXCoord->Name = L"numericUpDownXCoord";
			this->numericUpDownXCoord->Size = System::Drawing::Size(56, 20);
			this->numericUpDownXCoord->TabIndex = 48;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(99, 148);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(21, 13);
			this->label6->TabIndex = 47;
			this->label6->Text = L"x =";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(23, 148);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(72, 13);
			this->label5->TabIndex = 46;
			this->label5->Text = L"����������:";
			// 
			// numericUpDownTargetId
			// 
			this->numericUpDownTargetId->Location = System::Drawing::Point(121, 30);
			this->numericUpDownTargetId->Name = L"numericUpDownTargetId";
			this->numericUpDownTargetId->Size = System::Drawing::Size(56, 20);
			this->numericUpDownTargetId->TabIndex = 45;
			// 
			// butUpdTW
			// 
			this->butUpdTW->Location = System::Drawing::Point(229, 81);
			this->butUpdTW->Name = L"butUpdTW";
			this->butUpdTW->Size = System::Drawing::Size(167, 24);
			this->butUpdTW->TabIndex = 44;
			this->butUpdTW->Text = L"�������� ��������� ����";
			this->butUpdTW->UseVisualStyleBackColor = true;
			// 
			// tbTWClose
			// 
			this->tbTWClose->Location = System::Drawing::Point(185, 84);
			this->tbTWClose->Mask = L"00:00";
			this->tbTWClose->Name = L"tbTWClose";
			this->tbTWClose->Size = System::Drawing::Size(38, 20);
			this->tbTWClose->TabIndex = 43;
			this->tbTWClose->ValidatingType = System::DateTime::typeid;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(167, 87);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(12, 13);
			this->label4->TabIndex = 42;
			this->label4->Text = L"�";
			// 
			// tbTWOpen
			// 
			this->tbTWOpen->Location = System::Drawing::Point(123, 84);
			this->tbTWOpen->Mask = L"00:00";
			this->tbTWOpen->Name = L"tbTWOpen";
			this->tbTWOpen->Size = System::Drawing::Size(38, 20);
			this->tbTWOpen->TabIndex = 41;
			this->tbTWOpen->ValidatingType = System::DateTime::typeid;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(23, 87);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(94, 13);
			this->label3->TabIndex = 39;
			this->label3->Text = L"��������� ����:";
			// 
			// butDeleteTarget
			// 
			this->butDeleteTarget->Location = System::Drawing::Point(188, 28);
			this->butDeleteTarget->Name = L"butDeleteTarget";
			this->butDeleteTarget->Size = System::Drawing::Size(113, 24);
			this->butDeleteTarget->TabIndex = 37;
			this->butDeleteTarget->Text = L"������� ����";
			this->butDeleteTarget->UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(35, 32);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(71, 13);
			this->label2->TabIndex = 0;
			this->label2->Text = L"����� ����:";
			// 
			// buttonSavePathsToFile
			// 
			this->buttonSavePathsToFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->buttonSavePathsToFile->Location = System::Drawing::Point(422, 332);
			this->buttonSavePathsToFile->Name = L"buttonSavePathsToFile";
			this->buttonSavePathsToFile->Size = System::Drawing::Size(134, 21);
			this->buttonSavePathsToFile->TabIndex = 29;
			this->buttonSavePathsToFile->Text = L"��������� ���� � ����";
			this->buttonSavePathsToFile->UseVisualStyleBackColor = true;
			this->buttonSavePathsToFile->Click += gcnew System::EventHandler(this, &AppForm::ButtonSavePathsToFile_Click);
			// 
			// butX2
			// 
			this->butX2->Location = System::Drawing::Point(290, 381);
			this->butX2->Name = L"butX2";
			this->butX2->Size = System::Drawing::Size(46, 26);
			this->butX2->TabIndex = 35;
			this->butX2->Text = L"x5";
			this->butX2->UseVisualStyleBackColor = true;
			// 
			// butX3
			// 
			this->butX3->Location = System::Drawing::Point(340, 381);
			this->butX3->Name = L"butX3";
			this->butX3->Size = System::Drawing::Size(46, 26);
			this->butX3->TabIndex = 34;
			this->butX3->Text = L"x100";
			this->butX3->UseVisualStyleBackColor = true;
			// 
			// butX1
			// 
			this->butX1->Location = System::Drawing::Point(240, 381);
			this->butX1->Name = L"butX1";
			this->butX1->Size = System::Drawing::Size(46, 26);
			this->butX1->TabIndex = 33;
			this->butX1->Text = L"x1";
			this->butX1->UseVisualStyleBackColor = true;
			// 
			// butPause
			// 
			this->butPause->Location = System::Drawing::Point(180, 381);
			this->butPause->Name = L"butPause";
			this->butPause->Size = System::Drawing::Size(56, 26);
			this->butPause->TabIndex = 32;
			this->butPause->Text = L"�����";
			this->butPause->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(23, 388);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(90, 13);
			this->label1->TabIndex = 31;
			this->label1->Text = L"������� �����:";
			// 
			// labelPathsList
			// 
			this->labelPathsList->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelPathsList->AutoSize = true;
			this->labelPathsList->Location = System::Drawing::Point(6, 16);
			this->labelPathsList->Name = L"labelPathsList";
			this->labelPathsList->Size = System::Drawing::Size(75, 13);
			this->labelPathsList->TabIndex = 28;
			this->labelPathsList->Text = L"������ �����";
			// 
			// dataGridViewPaths
			// 
			this->dataGridViewPaths->AllowUserToAddRows = false;
			this->dataGridViewPaths->AllowUserToDeleteRows = false;
			this->dataGridViewPaths->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridViewPaths->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewPaths->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {
				this->Column1,
					this->Column2
			});
			this->dataGridViewPaths->Location = System::Drawing::Point(6, 34);
			this->dataGridViewPaths->Name = L"dataGridViewPaths";
			this->dataGridViewPaths->ReadOnly = true;
			this->dataGridViewPaths->Size = System::Drawing::Size(549, 284);
			this->dataGridViewPaths->TabIndex = 27;
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"����";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column1->Width = 350;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"�����";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			this->Column2->Width = 155;
			// 
			// AppForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1159, 617);
			this->Controls->Add(this->groupBoxResults);
			this->Controls->Add(this->tabControlLeft);
			this->Name = L"AppForm";
			this->Text = L"AppForm";
			this->tabControlLeft->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->groupBoxLoadedDataSets->ResumeLayout(false);
			this->groupBoxLoadedDataSets->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewSelectedRes))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxRes))->EndInit();
			this->groupBoxResults->ResumeLayout(false);
			this->groupBoxResults->PerformLayout();
			this->groupBoxEvents->ResumeLayout(false);
			this->groupBoxEvents->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownYCoord))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownXCoord))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDownTargetId))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewPaths))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	void UpdateListViewLoaded() {
		for (int i = 0; i < this->listViewLoadedDataSets->Items->Count; ++i) {
			this->listViewLoadedDataSets->Items[i]->BackColor = Color::White;
		}
		if (listViewLoadedDataSets->SelectedIndices->Count == 1)
			this->listViewLoadedDataSets->SelectedItems[0]->BackColor = Color::LightBlue;
	}

	void UpdateRunButton() {
		if (listViewLoadedDataSets->SelectedIndices->Count == 1) {
			this->buttonRun->Enabled = true;
		}
		else {
			this->buttonRun->Enabled = false;
		}
	}

	void UpdateDataGridViewPaths(ProblemSolution& solution) {
		while (this->dataGridViewPaths->Rows->Count < (int)solution.Paths.size())
			this->dataGridViewPaths->Rows->Add();
		while (this->dataGridViewPaths->Rows->Count > (int)solution.Paths.size())
			this->dataGridViewPaths->Rows->RemoveAt(this->dataGridViewPaths->Rows->Count - 1);

		int lineNum = 0;
		for (auto& path : solution.Paths) {
			double pathDistance = 0;
			if (path.size() > 0)
				pathDistance = solution.Input.Distance(0, path[0]) + solution.Input.Distance(0, path.back()); // from depot and to depot
			std::string pathStr = "";
			char buffer[(1 << 5)];
			for (size_t i = 0; i < path.size(); ++i) {
				int x = path[i];
				if (i + 1 < path.size())
					pathDistance += solution.Input.Distance(x, path[i + 1]);
				_itoa_s(x, buffer, 10);
				pathStr += ((std::string)buffer);
				pathStr += " ";
			}
			this->dataGridViewPaths->Rows[lineNum]->Cells[0]->Value = System::String(pathStr.c_str()).ToString();
			this->dataGridViewPaths->Rows[lineNum]->Cells[1]->Value = System::Double(pathDistance);
			this->dataGridViewPaths->Update();
			++lineNum;
		}

		this->dataGridViewPaths->ClearSelection();
	}

	// rowInd is clicked row in dataGridViewResults
	void UpdateVisualization(int launchInd) {
		this->dataGridViewSelectedRes->Rows->Clear();
		this->dataGridViewSelectedRes->Columns->Clear();

		int colInd = 0;
		for (auto col : { "����� ��������", "���������� TC",
			"���������� �����", "����������� ���������", "����� ����", "���� �����" }) {
			this->dataGridViewSelectedRes->Columns->Add(ToText(colInd++), ToText((std::string)col));
		}
		for (auto param : AppFormVars::Algos[AppFormVars::Launches[launchInd].AlgoIndex].Params) {
			this->dataGridViewSelectedRes->Columns->Add(ToText(colInd++), ToText((std::string)param.Name));
		}

		this->dataGridViewSelectedRes->Rows->Add();
		this->dataGridViewSelectedRes->Rows[0]->Cells[0]->Value = 
			ToText(AppFormVars::Launches[launchInd].DataSetIndex + 1);
		this->dataGridViewSelectedRes->Rows[0]->Cells[1]->Value = 
			ToText(AppFormVars::DataSets[AppFormVars::Launches[launchInd].DataSetIndex].DronsCnt);
		this->dataGridViewSelectedRes->Rows[0]->Cells[2]->Value = 
			ToText(AppFormVars::DataSets[AppFormVars::Launches[launchInd].DataSetIndex].TargetsCnt);
		this->dataGridViewSelectedRes->Rows[0]->Cells[3]->Value = 
			ToText(AppFormVars::DataSets[AppFormVars::Launches[launchInd].DataSetIndex].MaxDist);
		this->dataGridViewSelectedRes->Rows[0]->Cells[4]->Value = 
			ToText(AppFormVars::Launches[launchInd].Solution.SumOfPathLengths);
		this->dataGridViewSelectedRes->Rows[0]->Cells[5]->Value = 
			ToText(AppFormVars::Launches[launchInd].Solution.MaxPathLength);
		int curCol = 6;
		for (auto& param : AppFormVars::Launches[launchInd].ParamsVals) {
			this->dataGridViewSelectedRes->Rows[0]->Cells[curCol++]->Value = ToText(param);
		}
		
		for (int i = 0; i < this->dataGridViewSelectedRes->Columns->Count; i++) {
			this->dataGridViewSelectedRes->Columns[i]->AutoSizeMode = DataGridViewAutoSizeColumnMode::DisplayedCells;
			int widthCol = this->dataGridViewSelectedRes->Columns[i]->Width - 15;
			this->dataGridViewSelectedRes->Columns[i]->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
			this->dataGridViewSelectedRes->Columns[i]->Width = widthCol;
		}

		
		this->tabControlLeft->SelectedTab = this->tabControlLeft->TabPages[1];
		this->tabControlLeft->Refresh();

		DrawPaths(Graphics, AppFormVars::Launches[launchInd].Solution,
			this->pictureBoxRes->Height, this->pictureBoxRes->Width, 80);
	}


	private: System::Void ButtonRun_Click(System::Object^ sender, System::EventArgs^ e) {
		int dataSetInd = this->listViewLoadedDataSets->SelectedIndices[0];
		ProblemMode mode = ProblemMode::MINSUM;

		int algoInd = 0;
		if (AppFormVars::DataSets[dataSetInd].TargetsCnt <= 9) {
			algoInd = 3;
		}
		else if (AppFormVars::DataSets[dataSetInd].TargetsCnt > 90) {
			algoInd = 1;
		}
		else {
			algoInd = 2;
		}

		std::vector<double> args;
		for (int i = 0; i < (int)AppFormVars::Algos[algoInd].Params.size(); ++i) {
			args.push_back(AppFormVars::Algos[algoInd].Params[i].Recommend);
		}

		AppFormVars::CurrentSolution = SolverMain::Run(AppFormVars::DataSets[dataSetInd], mode, AppFormVars::Algos[algoInd].Ealgo, args);

		if (!AppFormVars::CurrentSolution.SolutionExists) {
			MessageBox::Show("�� ������� ����� �������");
		}
		else {
			AppFormVars::Launches.push_back(AppFormVars::Launch(algoInd, dataSetInd, args, mode, AppFormVars::CurrentSolution));
			UpdateDataGridViewPaths(AppFormVars::Launches.back().Solution);
			AppFormVars::PrintedLaunchPathsIndex = (int)AppFormVars::Launches.size() - 1;
			this->buttonSavePathsToFile->Enabled = true;
			this->buttonRunProcess->Enabled = true;
		}
	}

	private: System::Void ButtonFileOpen_Click(System::Object^ sender, System::EventArgs^ e) {
		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
			return;
		}
		std::string fileName = msclr::interop::marshal_as<std::string>(this->openFileDialog->FileName);
		this->labelLoadedFile->Text = System::String(fileName.c_str()).ToString();

		AppFormVars::LoadedInputData = InputData(fileName);
		buttonUploadFile->Enabled = true;
	}

	private: System::Void ButtonUploadFile_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!AppFormVars::LoadedInputData.DronsCnt && !AppFormVars::LoadedInputData.TargetsCnt) {
			MessageBox::Show("�������� ������ �����. ��������� ������:\n" +
				"���������� �� ��������������� ��������������\n ������ ��� ���������\n������ ��������� ����");
		}
		else {
			AppFormVars::DataSets.push_back(AppFormVars::LoadedInputData);
			std::string fn = AppFormVars::DataSets.back().FileName;
			std::size_t found = fn.find_last_of("/\\");
			fn = fn.substr(found + 1);
			auto newitem = gcnew ListViewItem(ToText(fn));
			newitem->SubItems->Add(ToText(AppFormVars::DataSets.back().DronsCnt));
			newitem->SubItems->Add(ToText(AppFormVars::DataSets.back().TargetsCnt));
			newitem->SubItems->Add(ToText(AppFormVars::DataSets.back().MaxDist));
			listViewLoadedDataSets->Items->Add(newitem);
		}
		AppFormVars::LoadedInputData = InputData();
		this->labelLoadedFile->Text = "���� �� ������";
		buttonUploadFile->Enabled = false;
		UpdateListViewLoaded();
	}

	private: void tb_KeyPress(System::Object^ sender, KeyPressEventArgs^ e) {
		auto s = (sender)->ToString();
		std::string tmps = msclr::interop::marshal_as<std::string>(s);
		int ind = tmps.find("Text");
		tmps = tmps.substr(ind, (int)tmps.size() - ind);
		if (tmps.find(".") != -1 && e->KeyChar == '.') {
			e->Handled = true;
		}
		else 
			if (!(Char::IsDigit(e->KeyChar)) && (e->KeyChar != '.')) {
			if (e->KeyChar != (char)Keys::Back) {
				e->Handled = true;
			}
		}
	}

	private: System::Void ListViewLoadedDataSets_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		UpdateRunButton();
		UpdateListViewLoaded();
	}

	private: System::Void ComboBoxResAlgo_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void ButtonSavePathsToFile_Click(System::Object^ sender, System::EventArgs^ e) {
		if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
			return;
		}
		std::string fileName = msclr::interop::marshal_as<std::string>(this->saveFileDialog->FileName);
		std::ofstream fout(fileName.c_str(), std::ios::out);
		std::string outputString;
		for (int i = 0; i < this->dataGridViewPaths->Rows->Count; ++i) {
			std::string curPath = msclr::interop::marshal_as<std::string>(this->dataGridViewPaths->Rows[i]->Cells[0]->Value->ToString());
			outputString += curPath;
			outputString += "\n";
		}

		fout << outputString;
		fout.close();
	}

	private: System::Void buttonRunProcess_Click(System::Object^ sender, System::EventArgs^ e) {
		int launchInd = (int)AppFormVars::Launches.size() - 1;
		UpdateVisualization(launchInd);
		UpdateDataGridViewPaths(AppFormVars::Launches[launchInd].Solution);
		this->Width = 1175;
		this->groupBoxResults->Visible = true;
	}
};
}
