#pragma once

#include "Optimizations.h"
#include "SolverClarkeWright.h"
#include "SolverDP.h"
#include "SolverGreedy.h"
#include "SolverGreedyClusterized.h"
#include "SolverMain.h"
#include "Utils.h"
#include "Visualization.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <memory>
#include <msclr\marshal_cppstd.h>

namespace VehicleRootingProblem {

    bool localDebug = false;

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MyForm : public System::Windows::Forms::Form {
    public:
        MyForm(void) {
            InitializeComponent();
            if (localDebug) {
                this->Width = 1800;
                this->Height = 740;
                this->textBoxResult->Width = 300;
                this->pictureBox1->Visible = true;
                this->textBox1->Visible = true;
                this->progressBar1->Visible = true;
                this->button1->Visible = true;
            } else {

            }
            AllocConsole();
            freopen("CONOUT$", "w", stdout);
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~MyForm() {
            if (components) {
                delete components;
            }
            if (formInputData) {
                delete formInputData;
            }
        }
    private: System::Windows::Forms::Button^  button1;
    private: System::Windows::Forms::ProgressBar^  progressBar1;
    private: System::Windows::Forms::TextBox^  textBox1;
    private: System::Windows::Forms::PictureBox^  pictureBox1;
    private: System::Windows::Forms::TextBox^  textBoxMatrix;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::Button^  buttonRun;
    private: System::Windows::Forms::RadioButton^  radioButtonMinMaxLen;
    private: System::Windows::Forms::RadioButton^  radioButtonMinSum;
    private: System::Windows::Forms::TextBox^  textBoxMaxDist;
    private: System::Windows::Forms::TextBox^  textBoxCntTargets;
    private: System::Windows::Forms::TextBox^  textBoxCntDrons;
    private: System::Windows::Forms::Label^  label7;
    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  labelFileName;
    private: System::Windows::Forms::Button^  buttonFileOpen;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
    private: System::Windows::Forms::GroupBox^  groupBox2;
    private: System::Windows::Forms::Button^  buttonSave;
    private: System::Windows::Forms::TextBox^  textBoxResult;
    private: System::Windows::Forms::TextBox^  textBoxResMaxLen;

    private: System::Windows::Forms::Label^  label8;
    private: System::Windows::Forms::TextBox^  textBoxResSumLen;

    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
    private: System::Windows::Forms::Label^  label9;

    protected:

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void) {
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
            this->textBoxMatrix = (gcnew System::Windows::Forms::TextBox());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->buttonRun = (gcnew System::Windows::Forms::Button());
            this->radioButtonMinMaxLen = (gcnew System::Windows::Forms::RadioButton());
            this->radioButtonMinSum = (gcnew System::Windows::Forms::RadioButton());
            this->textBoxMaxDist = (gcnew System::Windows::Forms::TextBox());
            this->textBoxCntTargets = (gcnew System::Windows::Forms::TextBox());
            this->textBoxCntDrons = (gcnew System::Windows::Forms::TextBox());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->labelFileName = (gcnew System::Windows::Forms::Label());
            this->buttonFileOpen = (gcnew System::Windows::Forms::Button());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
            this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->buttonSave = (gcnew System::Windows::Forms::Button());
            this->textBoxResult = (gcnew System::Windows::Forms::TextBox());
            this->textBoxResMaxLen = (gcnew System::Windows::Forms::TextBox());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->textBoxResSumLen = (gcnew System::Windows::Forms::TextBox());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
            this->groupBox1->SuspendLayout();
            this->groupBox2->SuspendLayout();
            this->SuspendLayout();
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(697, 675);
            this->button1->Margin = System::Windows::Forms::Padding(4);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(169, 84);
            this->button1->TabIndex = 0;
            this->button1->Text = L"Run";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Visible = false;
            this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
            // 
            // progressBar1
            // 
            this->progressBar1->Location = System::Drawing::Point(320, 672);
            this->progressBar1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
            this->progressBar1->Name = L"progressBar1";
            this->progressBar1->Size = System::Drawing::Size(360, 52);
            this->progressBar1->TabIndex = 2;
            this->progressBar1->Visible = false;
            // 
            // textBox1
            // 
            this->textBox1->Location = System::Drawing::Point(873, 675);
            this->textBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(159, 22);
            this->textBox1->TabIndex = 3;
            this->textBox1->Visible = false;
            // 
            // pictureBox1
            // 
            this->pictureBox1->Location = System::Drawing::Point(15, 21);
            this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
            this->pictureBox1->Name = L"pictureBox1";
            this->pictureBox1->Size = System::Drawing::Size(683, 610);
            this->pictureBox1->TabIndex = 4;
            this->pictureBox1->TabStop = false;
            this->pictureBox1->Visible = false;
            // 
            // textBoxMatrix
            // 
            this->textBoxMatrix->Location = System::Drawing::Point(11, 139);
            this->textBoxMatrix->MaxLength = 527670;
            this->textBoxMatrix->Multiline = true;
            this->textBoxMatrix->Name = L"textBoxMatrix";
            this->textBoxMatrix->ScrollBars = System::Windows::Forms::ScrollBars::Both;
            this->textBoxMatrix->Size = System::Drawing::Size(647, 398);
            this->textBoxMatrix->TabIndex = 5;
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->buttonRun);
            this->groupBox1->Controls->Add(this->radioButtonMinMaxLen);
            this->groupBox1->Controls->Add(this->radioButtonMinSum);
            this->groupBox1->Controls->Add(this->textBoxMaxDist);
            this->groupBox1->Controls->Add(this->textBoxCntTargets);
            this->groupBox1->Controls->Add(this->textBoxCntDrons);
            this->groupBox1->Controls->Add(this->label7);
            this->groupBox1->Controls->Add(this->label6);
            this->groupBox1->Controls->Add(this->label5);
            this->groupBox1->Controls->Add(this->label4);
            this->groupBox1->Controls->Add(this->labelFileName);
            this->groupBox1->Controls->Add(this->buttonFileOpen);
            this->groupBox1->Controls->Add(this->label2);
            this->groupBox1->Controls->Add(this->label1);
            this->groupBox1->Controls->Add(this->textBoxMatrix);
            this->groupBox1->Location = System::Drawing::Point(12, 13);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(664, 655);
            this->groupBox1->TabIndex = 6;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"Входные данные";
            // 
            // buttonRun
            // 
            this->buttonRun->Location = System::Drawing::Point(342, 583);
            this->buttonRun->Name = L"buttonRun";
            this->buttonRun->Size = System::Drawing::Size(193, 48);
            this->buttonRun->TabIndex = 19;
            this->buttonRun->Text = L"Запустить";
            this->buttonRun->UseVisualStyleBackColor = true;
            this->buttonRun->Click += gcnew System::EventHandler(this, &MyForm::buttonRun_Click);
            // 
            // radioButtonMinMaxLen
            // 
            this->radioButtonMinMaxLen->AutoSize = true;
            this->radioButtonMinMaxLen->Location = System::Drawing::Point(19, 610);
            this->radioButtonMinMaxLen->Name = L"radioButtonMinMaxLen";
            this->radioButtonMinMaxLen->Size = System::Drawing::Size(185, 21);
            this->radioButtonMinMaxLen->TabIndex = 18;
            this->radioButtonMinMaxLen->Text = L"Минимизировать время";
            this->radioButtonMinMaxLen->UseVisualStyleBackColor = true;
            // 
            // radioButtonMinSum
            // 
            this->radioButtonMinSum->AutoSize = true;
            this->radioButtonMinSum->Checked = true;
            this->radioButtonMinSum->Location = System::Drawing::Point(19, 583);
            this->radioButtonMinSum->Name = L"radioButtonMinSum";
            this->radioButtonMinSum->Size = System::Drawing::Size(261, 21);
            this->radioButtonMinSum->TabIndex = 17;
            this->radioButtonMinSum->TabStop = true;
            this->radioButtonMinSum->Text = L"Минимизировать суммарную длину";
            this->radioButtonMinSum->UseVisualStyleBackColor = true;
            // 
            // textBoxMaxDist
            // 
            this->textBoxMaxDist->Location = System::Drawing::Point(375, 81);
            this->textBoxMaxDist->Name = L"textBoxMaxDist";
            this->textBoxMaxDist->Size = System::Drawing::Size(160, 22);
            this->textBoxMaxDist->TabIndex = 16;
            // 
            // textBoxCntTargets
            // 
            this->textBoxCntTargets->Location = System::Drawing::Point(375, 51);
            this->textBoxCntTargets->Name = L"textBoxCntTargets";
            this->textBoxCntTargets->Size = System::Drawing::Size(160, 22);
            this->textBoxCntTargets->TabIndex = 15;
            // 
            // textBoxCntDrons
            // 
            this->textBoxCntDrons->Location = System::Drawing::Point(375, 21);
            this->textBoxCntDrons->Name = L"textBoxCntDrons";
            this->textBoxCntDrons->Size = System::Drawing::Size(160, 22);
            this->textBoxCntDrons->TabIndex = 14;
            // 
            // label7
            // 
            this->label7->AutoSize = true;
            this->label7->Location = System::Drawing::Point(135, 84);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(232, 17);
            this->label7->TabIndex = 13;
            this->label7->Text = L"Максимальная дальность полёта:";
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Location = System::Drawing::Point(135, 54);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(134, 17);
            this->label6->TabIndex = 12;
            this->label6->Text = L"Количество целей:";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(135, 24);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(141, 17);
            this->label5->TabIndex = 11;
            this->label5->Text = L"Количество дронов:";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(135, 114);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(151, 17);
            this->label4->TabIndex = 10;
            this->label4->Text = L"Матрица расстояний:";
            // 
            // labelFileName
            // 
            this->labelFileName->AutoSize = true;
            this->labelFileName->Location = System::Drawing::Point(305, 548);
            this->labelFileName->Name = L"labelFileName";
            this->labelFileName->Size = System::Drawing::Size(118, 17);
            this->labelFileName->TabIndex = 9;
            this->labelFileName->Text = L"Файл не выбран";
            // 
            // buttonFileOpen
            // 
            this->buttonFileOpen->Location = System::Drawing::Point(160, 543);
            this->buttonFileOpen->Name = L"buttonFileOpen";
            this->buttonFileOpen->Size = System::Drawing::Size(139, 27);
            this->buttonFileOpen->TabIndex = 8;
            this->buttonFileOpen->Text = L"Выберите файл";
            this->buttonFileOpen->UseVisualStyleBackColor = true;
            this->buttonFileOpen->Click += gcnew System::EventHandler(this, &MyForm::buttonFileOpen_Click);
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(8, 548);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(146, 17);
            this->label2->TabIndex = 7;
            this->label2->Text = L"Или загрузите файл:";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(8, 24);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(121, 17);
            this->label1->TabIndex = 6;
            this->label1->Text = L"Введите данные:";
            // 
            // openFileDialog1
            // 
            this->openFileDialog1->FileName = L"openFileDialog1";
            // 
            // groupBox2
            // 
            this->groupBox2->Controls->Add(this->pictureBox1);
            this->groupBox2->Controls->Add(this->label9);
            this->groupBox2->Controls->Add(this->buttonSave);
            this->groupBox2->Controls->Add(this->textBoxResult);
            this->groupBox2->Controls->Add(this->textBoxResMaxLen);
            this->groupBox2->Controls->Add(this->label8);
            this->groupBox2->Controls->Add(this->textBoxResSumLen);
            this->groupBox2->Controls->Add(this->label3);
            this->groupBox2->Location = System::Drawing::Point(682, 13);
            this->groupBox2->Name = L"groupBox2";
            this->groupBox2->Size = System::Drawing::Size(721, 655);
            this->groupBox2->TabIndex = 7;
            this->groupBox2->TabStop = false;
            this->groupBox2->Text = L"Результат";
            // 
            // label9
            // 
            this->label9->AutoSize = true;
            this->label9->Location = System::Drawing::Point(12, 93);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(101, 17);
            this->label9->TabIndex = 24;
            this->label9->Text = L"Список путей:";
            // 
            // buttonSave
            // 
            this->buttonSave->Location = System::Drawing::Point(15, 553);
            this->buttonSave->Name = L"buttonSave";
            this->buttonSave->Size = System::Drawing::Size(147, 27);
            this->buttonSave->TabIndex = 20;
            this->buttonSave->Text = L"Сохранить в файл";
            this->buttonSave->UseVisualStyleBackColor = true;
            this->buttonSave->Click += gcnew System::EventHandler(this, &MyForm::buttonSave_Click);
            // 
            // textBoxResult
            // 
            this->textBoxResult->Location = System::Drawing::Point(11, 139);
            this->textBoxResult->Multiline = true;
            this->textBoxResult->Name = L"textBoxResult";
            this->textBoxResult->ScrollBars = System::Windows::Forms::ScrollBars::Both;
            this->textBoxResult->Size = System::Drawing::Size(704, 398);
            this->textBoxResult->TabIndex = 20;
            // 
            // textBoxResMaxLen
            // 
            this->textBoxResMaxLen->Location = System::Drawing::Point(177, 59);
            this->textBoxResMaxLen->Name = L"textBoxResMaxLen";
            this->textBoxResMaxLen->Size = System::Drawing::Size(160, 22);
            this->textBoxResMaxLen->TabIndex = 23;
            // 
            // label8
            // 
            this->label8->AutoSize = true;
            this->label8->Location = System::Drawing::Point(12, 32);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(130, 17);
            this->label8->TabIndex = 20;
            this->label8->Text = L"Суммарная длина:";
            // 
            // textBoxResSumLen
            // 
            this->textBoxResSumLen->Location = System::Drawing::Point(177, 29);
            this->textBoxResSumLen->Name = L"textBoxResSumLen";
            this->textBoxResSumLen->Size = System::Drawing::Size(160, 22);
            this->textBoxResSumLen->TabIndex = 22;
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(12, 62);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(154, 17);
            this->label3->TabIndex = 21;
            this->label3->Text = L"Наидлиннейший путь:";
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1409, 673);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->groupBox2);
            this->Controls->Add(this->textBox1);
            this->Controls->Add(this->progressBar1);
            this->Controls->Add(this->button1);
            this->Margin = System::Windows::Forms::Padding(4);
            this->Name = L"MyForm";
            this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
            this->Text = L"Маршрутизация транспорта";
            this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            this->groupBox2->ResumeLayout(false);
            this->groupBox2->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
        //auto input = InputData("input.txt", false);

        std::ofstream fout("output.txt", std::ios::out);
        auto startTime = clock();

        InputDataGenerator generator;
        int tests = 1;
        this->progressBar1->Maximum = tests;
        this->progressBar1->Value = 0;
        this->progressBar1->Step = 1;
        auto probMode = ProblemMode::MINMAXLEN;
        if (this->radioButtonMinSum->Checked) {
            probMode = ProblemMode::MINSUM;
        }
		std::vector<ProblemSolution> solutions;
        for (int i = 0; i < tests; ++i) {
            int t = 100;

            auto input = generator.GenInputData(1, t / 5, 1, t, 100, 3000, -100, 100);

            auto sol1 = SolverGreedy::Run(input, 1000, probMode);
            auto sol2 = SolverGreedyClusterized::Run(input, 1000, probMode);

            
            solutions.push_back(sol1);
            solutions.push_back(sol2);
            solutions.push_back(SolverClarkeWright::Run(input, 1000, probMode));
            solutions.push_back(ReverseOptimization(solutions.back(), probMode));

            fout << "drons: " << input.DronsCnt << ", targets: " << input.TargetsCnt << std::endl;
            for (auto x : solutions)
                if (!x.SolutionExists) {
                    fout << "No \n";
                } else {
                    if (probMode == ProblemMode::MINSUM)
                        fout << x.SumOfPathLengths << " " << x.Paths.size() << "\n";
                    else
                        fout << x.MaxPathLength << " " << x.Paths.size() << "\n";
                }
            fout << "maxdist: " << input.MaxDist << std::endl << std::endl << std::endl;
            this->progressBar1->PerformStep();
        }

        double timeL = ((double)clock() - startTime) / CLOCKS_PER_SEC;

        this->textBox1->Text = System::Double(timeL).ToString();
        auto graphics = this->pictureBox1->CreateGraphics();
        DrawPaths(graphics, solutions.back(), this->pictureBox1->Height, this->pictureBox1->Width);
    }

    private: System::Void buttonSave_Click(System::Object^  sender, System::EventArgs^  e) {
        if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
            return;
        }
        std::string fileName = msclr::interop::marshal_as<std::string>(this->saveFileDialog1->FileName);
        std::ofstream fout(fileName.c_str(), std::ios::out);
        std::string outputString = msclr::interop::marshal_as<std::string>(this->textBoxResult->Text);
        for (int i = 0; i + 1 < (int)outputString.size(); ++i)
            while (i + 1 < (int)outputString.size() && !isdigit(outputString[i]) && outputString[i] != '.' &&
                !isdigit(outputString[i + 1]) && outputString[i + 1] != '.' && outputString[i + 1] != '\n')
                outputString.erase(outputString.begin() + i + 1, outputString.begin() + i + 2);
        fout << outputString;
        fout.close();
    }

    private: System::Void buttonFileOpen_Click(System::Object^  sender, System::EventArgs^  e) {
        this->labelFileName->Text = "Файл не выбран";
        delete formInputData;
        formInputData = nullptr;
        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
            return;
        }
        std::string fileName = msclr::interop::marshal_as<std::string>(this->openFileDialog1->FileName);
        this->labelFileName->Text = System::String(fileName.c_str()).ToString();
        if (formInputData)
            delete formInputData;
        formInputData = new InputData(fileName);
    }

    private: InputData* formInputData = nullptr;
    private: ProblemMode formProblemMode = ProblemMode::MINMAXLEN;

    private: System::Void buttonRun_Click(System::Object^  sender, System::EventArgs^  e) {
        if (this->radioButtonMinMaxLen->Checked) {
            formProblemMode = ProblemMode::MINMAXLEN;
        } else {
            formProblemMode = ProblemMode::MINSUM;
        }

        bool fromFile = false;

        if (this->textBoxCntDrons->Text->Length == 0 || this->textBoxCntTargets->Text->Length == 0
            || this->textBoxMaxDist->Text->Length == 0) {
            fromFile = true;
        }

        if (!fromFile) {
            int cntDrons = atoi(msclr::interop::marshal_as<std::string>(this->textBoxCntDrons->Text).c_str());
            int cntTargets = atoi(msclr::interop::marshal_as<std::string>(this->textBoxCntTargets->Text).c_str());
            double maxDist = atof(msclr::interop::marshal_as<std::string>(this->textBoxMaxDist->Text).c_str());
            MatrixDouble matrix;
            std::string matrixStr = msclr::interop::marshal_as<std::string>(this->textBoxMatrix->Text) + " ";
            std::vector<double> matrix1;
            std::string cur;
            for (int i = 0; i < (int)matrixStr.size(); ++i) {
                if (!isdigit(matrixStr[i]) && matrixStr[i] != '.') {
                    if (cur.size()) {
                        matrix1.push_back(atof(cur.c_str()));
                        cur = "";
                    }
                } else {
                    cur += matrixStr[i];
                }
            }

            int sz = matrix1.size();

            if (sz != (cntTargets + 1) * (cntTargets + 1)) {
                auto sizeStr = System::Int32(cntTargets + 1).ToString();
                MessageBox::Show("Матрица должна иметь размер " + sizeStr + "x" + sizeStr);
                return;
            }

            matrix.resize(cntTargets + 1);

            for (int i = 0; i < (int)matrix1.size(); ++i) {
                int lineNum = i / (cntTargets + 1);
                assert(lineNum < (int)matrix.size());
                matrix[lineNum].push_back(matrix1[i]);
            }
            if (formInputData)
                delete formInputData;
            formInputData = new InputData(cntDrons, cntTargets, maxDist, matrix);
        }

        if (!formInputData) {
            MessageBox::Show("Загрузите файл или введите все данные в форму");
            return;
        }

        auto solution = SolverMain::Run(*formInputData, 5, formProblemMode);
        this->textBoxResMaxLen->Text = System::Double(solution.MaxPathLength).ToString();
        this->textBoxResSumLen->Text = System::Double(solution.SumOfPathLengths).ToString();
        this->textBoxResult->Clear();
        if (!solution.SolutionExists) {
            this->textBoxResult->Text = System::String("Решение не найдено").ToString();
            return;
        }

        for (auto& path : solution.Paths) {
            for (auto x : path) {
                this->textBoxResult->Text += System::Double(x).ToString() + System::String(" ").ToString();
            }
            this->textBoxResult->Text += Environment::NewLine;
        }
    }
    private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
    }
};
}
