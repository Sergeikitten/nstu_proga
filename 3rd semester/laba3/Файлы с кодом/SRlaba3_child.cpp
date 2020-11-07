// srDLL1Client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <windows.h> 
#include <stdio.h> 
#define BUF_SIZE 256


HINSTANCE hLib;
int(*fileProcessing)(CHAR*, char);


int main(int argc, LPTSTR argv[]) {
	HANDLE hIn, hOut;
	DWORD nIn, nOut;
	CHAR buf[BUF_SIZE];
	CHAR FileOut[BUF_SIZE];
	int exchanges = 0, MAX_CHANGES;
	char TARGET_CHAR;
	/*if (argc != 3) {
		printf("Error: ");
		exit(-5);
	}*/


	hLib = LoadLibrary("childDLL.dll");
	if (hLib == NULL) {
		printf("|ERROR| Cannot load the library\n");
		exit(-1);
	}

	fileProcessing = (int(*)(CHAR*, char))GetProcAddress(hLib, "fileProcessing");
	if (fileProcessing == NULL) {
		printf("|ERROR| Function not found\n");
		exit(-2);
	}
	//(*TestHello)();



	if (argc != 3) {
		printf("|ERROR| Please, use: *PROGRAM_NAME* input_file max_changes\n\n");
		return -1;
	}

	//string name = argv[1]; 
	char fileIn[100] = { 0 };
	char fileOut[100] = { 0 };

	// Get the name of output file 
	strcpy_s(fileIn, argv[1]);
	strcpy_s(fileOut, argv[1]);
	strcat_s(fileOut, ".sergei");


	// Try to open input file (READ MODE)
	hIn = CreateFile(fileIn, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hIn == INVALID_HANDLE_VALUE) {
		printf("|ERROR| Cannot open input file. Error: %x\n\n", GetLastError());
		return -1;
	}

	// Get maximum count of changes
	TARGET_CHAR = argv[2][0];
	//MAX_CHANGES = atoi(argv[2]);


	// [HAFTO CHANGE] Get output file name 
	//if (strchr(argv[1], '.') == NULL) { // If input file hasn't file extension
	//	strcpy(FileOut, argv[1]);
	//} else {
	//	strncpy(FileOut, argv[1], strchr(argv[1], '.') - argv[1]);
	//}
	//strcat_s(FileOut, ".out");



	// Try to open output file (WRITE MODE)
	hOut = CreateFile(fileOut, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		printf("|ERROR| Cannot open output file. Error: %x\n\n", GetLastError());
		return -1;
	}


	// File reading-writing 
	while (ReadFile(hIn, buf, BUF_SIZE, &nIn, NULL) && nIn > 0) {
		// Sometime there is an error with fantom characters
		buf[nIn] = '\0';
		//printf("========= Recognized ========= \n%s\n\n", buf);
		exchanges = (*fileProcessing)(buf, TARGET_CHAR);
		//printf("========= Processed  ========= \n%s\n\n", buf);
		WriteFile(hOut, buf, nIn, &nOut, NULL);
		if (nIn != nOut) {
			printf("|ERROR| Writing error: %x\n\n", GetLastError());
			return -1;
		}
	}

	FreeLibrary(hLib);
	//printf(" ========= EXCHANGES: %d |DONE| \n\n", exchanges);
	//printf("Number of exhanges: %d \n", exchanges);
	CloseHandle(hIn);
	CloseHandle(hOut);
	return exchanges;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
