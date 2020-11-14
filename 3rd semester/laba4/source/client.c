#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define BUF_SIZE 256
#define MAX_TRIES 5000

const char DLL_PATH[] = "dll\\fileProcess.dll";
const char INPUT_DIR[] = ".\\input\\";
const char OUTPUT_DIR[] = ".\\output\\";

int(*fileProcessing)(CHAR*, char);

void generate_filenames(CHAR name[], CHAR file1[], CHAR file2[]) {
  strcpy(file1, INPUT_DIR);
  strcpy(file2, OUTPUT_DIR);
  
  strcat(file1, name);
  strcat(file2, name);

  strcat(file2, ".sergei");
}

int make_request(HANDLE hRead,
                 HANDLE hWrite,
                 DWORD* cbWritten,
                 DWORD* cbRead,
                 char request[],
                 char buf[]) {
  if (!WriteFile(hWrite, request, strlen(request) + 1, cbWritten, NULL)) {
    printf("a");
    fprintf(stderr, "|ERROR| Error sending signal %lu\n", GetLastError());
    return 2;
  }

  if (ReadFile(hRead, buf, BUF_SIZE, cbRead, NULL)) {
    return 0;
  } else {
    fprintf(stderr, "|ERROR| Error receiving message%lu\n", GetLastError());
    return 3;
  }
}

int main() {
  DWORD cbWritten = 0, cbRead = 0;
  HINSTANCE hLib;

  char targetChar;

  HANDLE hIn, hOut;
  DWORD dIn, dOut;
  HANDLE hRead = GetStdHandle(STD_INPUT_HANDLE),
         hWrite = GetStdHandle(STD_OUTPUT_HANDLE);

  CHAR buf[BUF_SIZE], outbuf[BUF_SIZE];
  CHAR inName[100], outName[100];

  if (make_request(hRead, hWrite, &cbWritten, &cbRead, "n", buf)) {
    return -1;
  } else {
    fprintf(stderr, "CLIENT received name: %s\n", buf);
  }

  generate_filenames((buf), inName, outName);
  memset(buf, '\0', sizeof(buf));

  if (make_request(hRead, hWrite, &cbWritten, &cbRead, "c", buf)) {
    return -1;
  } else {
    fprintf(stderr, "CLIENT received target_char: %s\n", buf);
  }

  targetChar = buf[0];
  memset(buf, '\0', sizeof(buf));

  hIn = CreateFile(inName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
  if (hIn == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "|ERROR| Can't open input file: %lu\n", GetLastError());
    return -4;
  }

  hOut = CreateFile(outName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL, NULL);
  if (hOut == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "|ERROR| Can't open output file: %lu\n", GetLastError());
    return -5;
  }

  hLib = LoadLibrary(DLL_PATH);
  if (hLib == NULL) {
    fprintf(stderr, "|ERROR| Cannot load the library");
    return -6;
  }

  //fileProcessing = (int(*)(CHAR*, char))GetProcAddress(hLib, "fileProcessing");
  fileProcessing = (int (*)(CHAR*, char))GetProcAddress(
      hLib, "fileProcessing");
  if (fileProcessing == NULL) {
    fprintf(stderr, "|ERROR| fileProcessing function not found");
    return -7;
  }

  // PROCESS TEXT
  int exchanges = 0;
  while (ReadFile(hIn, buf, BUF_SIZE, &dIn, NULL) && dIn > 0) {
    //r = (*fileProcessing)(buf, outbuf, r, rCount, BUF_SIZE);
    buf[dIn] = '\0';

    //fprintf(stderr, "File includes: %s\n", buf);
    exchanges = (*fileProcessing)(buf, targetChar);
    //fprintf(stderr, "File processed: %s\n", buf);
    WriteFile(hOut, buf, dIn, &dOut, NULL);
  }

  char output[BUF_SIZE];
  sprintf(output, "p%i", exchanges);
  if (make_request(hRead, hWrite, &cbWritten, &cbRead, output, buf)) {
    return -1;
  } else {
    fprintf(stderr, "Finished with %i changes\n", exchanges);
  }

  CloseHandle(hIn);
  CloseHandle(hOut);
  FreeLibrary(hLib);
  return 0;
}