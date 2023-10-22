#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <unordered_set>
#include <algorithm>
// Definirea tipurilor de tokeni
enum TokenType
{
          IDENTIFIER,
          INTEGER,
          FLOAT,
          OPERATOR,
          ASSIGNMENT_OP,
          LOGICAL_OP,
          COMPARISON_OP,
          DELIMITER,
          PUNCTUATION,
          SPECIAL_CHAR,
          KEYWORD,
          STRING,
          CHARACTER,
          COMMENT,
          END,
          ERROR
};
bool isDelimiter(char ch)
{
          std::string delimiters = " \t\n;(){}[]";
          return delimiters.find(ch) != std::string::npos;
}

bool isSpecialCharacter(char ch)
{
          std::string specialCharacters = "<>=!+-*/%&|^~";
          return specialCharacters.find(ch) != std::string::npos;
}
// Structura pentru un token
struct Token
{
          TokenType type;
          int valueIndex;
};

// Scheletul clasei Lexer
class Lexer
{
private:
          std::map<int, std::map<char, int>> dfa;   // DFA sub forma unei tabele de tranzitie: starea_curentă, (caracter, stare_următoare)
          std::vector<std::string> valueTable;      // Tabelul de valori pentru tokeni
          std::unordered_set<std::string> keywords; // Tabelul de cuvinte cheie
          std::ifstream sourceFile;                 // Fisierul sursa care va fi analizat
          int currentState;                         // Starea curenta in DFA

public:
          Lexer(const std::string &filePath) : currentState(0)
          {
                    sourceFile.open(filePath);
                    if (sourceFile.fail())
                    {
                              std::cerr << "Failbit set" << std::endl;
                    }
                    if (sourceFile.bad())
                    {
                              std::cerr << "Badbit set" << std::endl;
                    }
                    if (sourceFile.eof())
                    {
                              std::cerr << "EOFbit set" << std::endl;
                    }
                    if (!sourceFile.is_open())
                    {
                              std::cerr << "Nu s-a putut deschide fisierul sursa." << std::endl;
                              // Tratare eroare
                    }

                    dfa[0][' '] = 3;
                    dfa[0]['\t'] = 3;
                    dfa[0]['\n'] = 3;
                    dfa[0]['\r'] = 3;

                    // Inițializare pentru starea 4 (semne de punctuație)
                    dfa[0]['.'] = 4;
                    dfa[0][','] = 4;
                    dfa[0][';'] = 4;
                    // ... (adăugați alte semne de punctuație aici)

                    // Inițializare pentru starea 5 (caractere speciale)
                    dfa[0]['!'] = 5;
                    dfa[0]['@'] = 5;
                    dfa[0]['#'] = 5;

                    for (char c = 'a'; c <= 'z'; ++c)
                    {
                              dfa[0][c] = 1;
                    }
                    for (char c = 'A'; c <= 'Z'; ++c)
                    {
                              dfa[0][c] = 1;
                    }
                    for (char c = '0'; c <= '9'; ++c)
                    {
                              dfa[0][c] = 2;
                    }

                    // Inițializare pentru starea 1 (identificatori)
                    for (char c = 'a'; c <= 'z'; ++c)
                    {
                              dfa[1][c] = 1;
                    }
                    for (char c = 'A'; c <= 'Z'; ++c)
                    {
                              dfa[1][c] = 1;
                    }
                    for (char c = '0'; c <= '9'; ++c)
                    {
                              dfa[1][c] = 1;
                    }

                    // Inițializare pentru starea 2 (constante întregi)
                    for (char c = '0'; c <= '9'; ++c)
                    {
                              dfa[2][c] = 2;
                    }
          }
          std::vector<std::string> getValueTable() const
          {
                    return valueTable;
          }
          Token generateToken(int state, const std::string &lexeme);
          bool isFinalState(int state);
          Token getToken(); // Aceasta functie va fi implementata ulterior
};

// Metoda getToken va fi definita ulterior
Token Lexer::getToken()
{
          std::string lexeme;
          int finalState = -1;
          currentState = 0;

          char ch;
          while (sourceFile.get(ch))
          {
                    if (dfa[currentState].find(ch) != dfa[currentState].end())
                    {
                              currentState = dfa[currentState][ch];
                              lexeme += ch;

                              if (isFinalState(currentState))
                              {
                                        finalState = currentState;
                              }
                    }
                    else
                    {
                              if (isDelimiter(ch) || isSpecialCharacter(ch))
                              {
                                        if (finalState != -1)
                                        {
                                                  Token token = generateToken(finalState, lexeme);
                                                  lexeme.clear();
                                                  finalState = -1;
                                                  sourceFile.putback(ch); // Pune înapoi ultimul caracter citit
                                                  return token;
                                        }
                                        // Adaugă logica pentru delimitatori și caractere speciale aici
                              }
                              else
                              {
                                        if (finalState != -1)
                                        {
                                                  Token token = generateToken(finalState, lexeme);
                                                  lexeme.clear();
                                                  finalState = -1;
                                                  sourceFile.putback(ch); // Pune înapoi ultimul caracter citit
                                                  return token;
                                        }
                                        else
                                        {
                                                  Token errorToken;
                                                  errorToken.type = ERROR;
                                                  errorToken.valueIndex = -1;
                                                  return errorToken;
                                        }
                              }
                    }
          }

          Token endToken;
          endToken.type = END;
          endToken.valueIndex = -1;
          return endToken;
}

bool Lexer::isFinalState(int state)
{
          return state == 1 || state == 2 || state == 3 || state == 4;
}

Token Lexer::generateToken(int state, const std::string &lexeme)
{
          Token token;
          if (state == 1)
          {
                    token.type = IDENTIFIER;
          }
          else if (state == 2)
          {
                    token.type = INTEGER;
          }
          else if (state == 3)
          {
                    token.type = PUNCTUATION; // Adăugați acest tip în enumerația TokenType
          }
          else if (state == 4)
          {
                    token.type = SPECIAL_CHAR; // Adăugați acest tip în enumerația TokenType
          }

          // Căutăm lexemul în tabela de valori
          auto it = std::find(valueTable.begin(), valueTable.end(), lexeme);

          if (it != valueTable.end())
          {
                    // Lexemul există deja în tabelă; setăm valueIndex cu poziția sa
                    token.valueIndex = std::distance(valueTable.begin(), it);
          }
          else
          {
                    // Adăugăm lexemul nou în tabela de valori și setăm valueIndex
                    valueTable.push_back(lexeme);
                    token.valueIndex = valueTable.size() - 1;
          }

          return token;
}
const std::vector<std::string> TokenTypeNames = {
    "IDENTIFIER", "INTEGER", "FLOAT", "OPERATOR", "ASSIGNMENT_OP", "LOGICAL_OP",
    "COMPARISON_OP", "DELIMITER", "KEYWORD", "STRING", "CHARACTER", "COMMENT", "END", "ERROR"};

// Modificări în funcția main
int main()
{
          Lexer lexer("./program_sursa.txt"); // înlocuiește cu calea către fișierul tău
          Token token;
          std::ofstream outputFile("output.txt");

          do
          {
                    token = lexer.getToken();
                    if (token.type == ERROR)
                    {
                              outputFile << "A apărut o eroare." << std::endl;
                    }
                    else
                    {
                              const std::vector<std::string> &valueTable = lexer.getValueTable();
                              if (token.valueIndex >= 0 && token.valueIndex < valueTable.size())
                              {
                                        outputFile << "Tipul tokenului: " << TokenTypeNames[token.type]
                                                   << ", Lexem: " << valueTable[token.valueIndex] << std::endl;
                              }
                    }
          } while (token.type != END);

          outputFile.close(); // Închide fișierul de output
          return 0;
}
