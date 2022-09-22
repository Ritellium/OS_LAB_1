#include <Windows.h>
#include <iostream>
#include "../Employee.h"
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <charconv>

#pragma warning (disable:4996)

/*
Написать консольную программу Main и две утилиты (служебные программы)
Creator и Reporter, которые выполняют следующие действия.
Утилита Creator создает бинарный файл, имя которого и количество записей в котором
получает через командную строку.

remember, no STD...
*/

int main(int argc, char* argv[])
{
	// Input for Creator

	int emount = 0;
	printf("input number of elements: ");
	scanf_s("%d", &emount);
	char* file_name = new char;
	printf("input name of binary file (it will be created): ");
	scanf_s("%s", file_name, 31);
	
	// Creator work

	char CreatorAllocation[40] = "Creator.exe";
	
	char* parameters = new char;
	parameters = strcpy(parameters, file_name); //"{file_name}"
	parameters = strcat(parameters, " "); //"{file_name} "

	std::string s = std::to_string(emount);
	const char* nchar = s.c_str();
	parameters = strcat(parameters, nchar); // "{file_name} {emount}"

	STARTUPINFO Creator_StartInf;
	PROCESS_INFORMATION Creator_PrInf;

	ZeroMemory(&Creator_StartInf, sizeof(STARTUPINFO));
	Creator_StartInf.cb = sizeof(STARTUPINFO);

	wchar_t destination[50];
	strcat(CreatorAllocation, " ");
	strcat(CreatorAllocation, parameters);
	mbstowcs(destination, CreatorAllocation, strlen(CreatorAllocation) + 1);

	// создаем Creator
	if (!CreateProcess(nullptr, destination, nullptr, nullptr, FALSE,
		CREATE_NEW_CONSOLE, nullptr, nullptr, &Creator_StartInf, &Creator_PrInf))
	{
		_cputs("The Creator is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	_cputs("The Creator is runned.\n");
	// ждем завершения Creator
	WaitForSingleObject(Creator_PrInf.hProcess, INFINITE);
	// закрываем дескрипторы процесса Creator
	CloseHandle(Creator_PrInf.hThread);
	CloseHandle(Creator_PrInf.hProcess);

	_cputs("The Creator is Finished.\n");

	// Вывод бинарника на консоль

	errno_t error;
	FILE* file;

	error = fopen_s(&file, file_name, "rb");
	if (error != 0)
	{
		printf("Error opening file");
		return 0;
	}
	fseek(file, 0, SEEK_SET);

	employee inputter;

	fread(&emount, sizeof(int), 1, file);

	for (size_t i = 0; i < emount; i++)
	{
		inputter.input_file(file);
		inputter.output(); 
		printf("\n");
	}

	// Конец вывода

	// Input for Reporter

	char* report_name = new char;
	printf("input name of report file (it will be created): ");
	scanf_s("%s", report_name, 31);

	double PayForHour = 0;
	printf("input pay for hour: ");
	scanf_s("%lf", &PayForHour);

	// Reporter work

	char ReporterAllocation[40] = "Reporter.exe";

	char* rep_parameters = new char;
	strcpy(rep_parameters, file_name);	//"{file_name}"
	strcat(rep_parameters, " ");		//"{file_name} "
	strcat(rep_parameters, report_name);	//"{file_name} {report_name}"
	strcat(rep_parameters, " ");			//"{file_name} {report_name} "

	std::string s_rep = std::to_string(PayForHour);
	const char* p_char = s_rep.c_str();
	rep_parameters = strcat(rep_parameters, p_char); //"{file_name} {report_name} {PayForHour}"

	STARTUPINFO Reporter_StartInf;
	PROCESS_INFORMATION Reporter_PrInf;

	ZeroMemory(&Reporter_StartInf, sizeof(STARTUPINFO));
	Reporter_StartInf.cb = sizeof(STARTUPINFO);

	wchar_t destination_rep[50];
	strcat(ReporterAllocation, " ");
	strcat(ReporterAllocation, rep_parameters);
	mbstowcs(destination_rep, ReporterAllocation, strlen(ReporterAllocation) + 1);

	// создаем Reporter
	if (!CreateProcess(NULL, destination_rep, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &Reporter_StartInf, &Reporter_PrInf))
	{
		_cputs("The Reporter is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	_cputs("The Reporter is runned.\n");
	// ждем завершения Reporte
	WaitForSingleObject(Reporter_PrInf.hProcess, INFINITE);
	// закрываем дескрипторы процесса Reporter
	CloseHandle(Reporter_PrInf.hThread);
	CloseHandle(Reporter_PrInf.hProcess);

	_cputs("The Reporter is Finished.\n");

	FILE* report_in;

	error = fopen_s(&report_in, report_name, "r");
	if (error != 0)
	{
		printf("Error opening file");
	}

	employee inputter_rep;

	for (size_t i = 0; i < emount; i++)
	{
		inputter_rep.input_file_txt(report_in);
		double salary = inputter_rep.hours * PayForHour;
		
		inputter_rep.output();
		printf("%f", salary);
		printf("\n");
	}

	return 0;
}