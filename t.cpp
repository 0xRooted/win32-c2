#include <iostream>
#include <windows.h>
using namespace std ;

char *currpath()
{
    char *path = new char[MAX_PATH];
    memset(path, 0, MAX_PATH);
    GetModuleFileNameA(GetModuleHandleA(NULL), path, MAX_PATH);
    return path;
}

int main()
{
    char *path = currpath();
    std::cout << path << "\n";
    free(path);
    return EXIT_SUCCESS;
}