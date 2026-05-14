#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

#define CLR_RESET   "\033[0m"
#define CLR_BOLD    "\033[1m"
#define CLR_DIM     "\033[2m"
#define CLR_ITALIC  "\033[3m"
#define CLR_ULINE   "\033[4m"

#define CLR_RED     "\033[91m"
#define CLR_GREEN   "\033[92m"
#define CLR_YELLOW  "\033[93m"
#define CLR_BLUE    "\033[94m"
#define CLR_MAGENTA "\033[95m"
#define CLR_CYAN    "\033[96m"
#define CLR_WHITE   "\033[97m"
#define CLR_GRAY    "\033[90m"

#define CLR_BG_BLUE    "\033[44m"
#define CLR_BG_GREEN   "\033[42m"
#define CLR_BG_RED     "\033[41m"
#define CLR_BG_YELLOW  "\033[43m"
#define CLR_BG_CYAN    "\033[46m"
#define CLR_BG_MAGENTA "\033[45m"
#define CLR_BG_WHITE   "\033[47m"

void enableColors();
void clearScreen();
void pauseScreen();

void printBanner();
void printMenuHeader(const char* title);
void printMenuOption(int number, const char* label);
void printMenuFooter();
void printSeparator(char character = '-', int length = 55);
void printDoubleSeparator(int length = 55);

void printSuccess(const char* message);
void printError(const char* message);
void printWarning(const char* message);
void printInfo(const char* message);

void getCurrentTimestamp(char* buffer, int maxLen);

void strCopyN(char* destination, const char* source, int maxLen);
bool strEquals(const char* first, const char* second);
bool strEqualsNoCase(const char* first, const char* second);
int displayWidth(const char* text);

bool readInt(int& out);
bool isBlankString(const char* str);
void printRailwayArt();
void printSubMenuDeco();

#endif
