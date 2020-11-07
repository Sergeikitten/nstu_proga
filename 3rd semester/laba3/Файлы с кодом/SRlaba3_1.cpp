// SRlaba3_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

	char child[30] = "child.exe";

	STARTUPINFO si[255];
	PROCESS_INFORMATION pi[255];
	char command[100];
		
	DWORD finish, result, total = 0;
	LPDWORD res = &result;

	HANDLE ht[255];

	if (argc < 3) {
		printf("|ERROR| There must be at least two arguments\n");
		printf("Please type: fileName fileName ... targetChar\n");
		return -1;
	}

	int i = 0;
	for (i = 0; i < (argc - 2); i++) {
		strcpy_s(command, child);
		strcat_s(command, " ");
		strcat_s(command, argv[i + 1]);
		strcat_s(command, " ");
		strcat_s(command, argv[argc - 1]);
		printf("The command is: %s\n", command);

		ZeroMemory(&si[i], sizeof(si[i]));
		si[i].cb = sizeof(si);
		ZeroMemory(&pi[i], sizeof(pi[i]));


		if (CreateProcess(NULL, command, NULL, NULL, TRUE, NULL, NULL, NULL, &si[i], &pi[i])) {
			printf("Process %lu started for file: %s\n", pi[i].dwProcessId, argv[i + 1]);
			ht[i] = pi[i].hProcess;
			//Sleep(500);
		}
		else {
			printf("|ERROR| CreateProcess failed: %lu\n", GetLastError());
			return -2;
		}
	}
	
	Sleep(1000);
	printf("\n");
	for (i = 0; i < (argc - 2); i++) {
		finish = WaitForSingleObject(pi[i].hProcess, INFINITE);

		if (finish == WAIT_OBJECT_0) {
			GetExitCodeProcess(pi[i].hProcess, res);
			printf("Process %lu finished his work and made %d changes\n", pi[i].dwProcessId, result);
			//printf("Process %lu made %d changes\n", pi[i].dwProcessId, result);
			total += result;
		}
		else {
			printf("|ERROR| Process %lu failed his job\n", pi[i].dwProcessId);
			CloseHandle(pi[i].hProcess);
			CloseHandle(pi[i].hThread);
			TerminateProcess(pi[i].hProcess, 0);
		}
	}

	printf("\n");
	printf("%d files processed with total changes %d |DONE|\n", argc - 2, total);

	return 0;
}
//---------------------------------------------------------------------------

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
