#include <iostream>
#include "LexicalAnalyzer.h"
int main()
{
    LexicalAnalyzer lexer("input.txt", "output.txt");
    Token token;
    do
    {
        token = lexer.getToken();
        if (token.type != NONE)
        {
            lexer.writeTokenToFile(token);
        }

        // ... procesare token dacă este necesar ...
    } while (token.type != ERROR && token.type != END_OF_FILE);

    return 0;
}