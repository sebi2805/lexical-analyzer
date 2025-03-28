#include <iostream>
#include <string>
#include "LexicalAnalyzer.h"
int main(int argc, char *argv[])
{
    std::string inputFileName = "input.txt";
    std::string outputFileName = "output.txt";

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "-i" && i + 1 < argc)
        {
            inputFileName = argv[++i];
        }
        else if (arg == "-o" && i + 1 < argc)
        {
            outputFileName = argv[++i];
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    LexicalAnalyzer lexer(inputFileName, outputFileName);
    Token token;
    do
    {
        token = lexer.getToken();
        if (token.type != NONE)
            lexer.writeTokenToFile(token);

        // ... procesare token dacă este necesar ...
    } while (token.type != ERROR && token.type != END_OF_FILE);

    return 0;
}