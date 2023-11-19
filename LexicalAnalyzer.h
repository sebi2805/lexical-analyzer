#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "utils.h"
#include <fstream>
#include <vector>

class LexicalAnalyzer
{
private:
          std::ifstream sourceFile;
          std::ofstream outputFile;
          std::vector<std::string> stringTable;
          std::string buffer;
          LexerState currentState = START;
          int currentLine;
          int currentColumn;
          std::vector<std::string> uniqueTokenValues;

          bool isKeyword(const std::string &word);
          bool isIdentifierChar(char ch);
          Token getIdentOrKeywordToken(std::string &buffer);
          bool isDigitCh(char ch);
          Token getConstantToken(std::string &buffer);
          bool isOperatorChar(char ch);
          Token getOperatorToken(std::string &buffer);
          bool isSymbolChar(char ch);
          Token getSymbolToken(char ch);
          bool isStartOfComment(char ch1, char ch2);
          void getCommentToken(const std::string &initialBuffer);
          bool isStartOfPreprocessor(char ch);
          Token getPreprocessorToken(std::string &buffer);
          bool isWhitespace(char ch);
          Token getErrorToken(std::string &error);
          Token getStringToken(std::string &buffer);
          bool isValidOperator(const std::string &op);
          int getTokenValueIndex(const std::string &tokenValue);
          Token createErrorToken(const std::string &errorMessage);

public:
          LexicalAnalyzer(const std::string &inputFilename, const std::string &outputFilename) : currentLine(1), currentColumn(0)
          {
                    buffer = "";
                    sourceFile.open(inputFilename);
                    outputFile.open(outputFilename);
          };

          Token getToken();
          void writeTokenToFile(const Token &token);
          ~LexicalAnalyzer()
          {
                    sourceFile.close();
          }
};

#endif