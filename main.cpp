#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

// Enumerarea tipurilor de tokeni
enum TokenType
{
    KEYWORD,
    IDENTIFIER,
    SYMBOL,
    INTEGER_CONSTANT,
    FLOAT_CONSTANT,
    STRING,
    ERROR,
    PREPROCESSOR,
    OPERATOR,
    COMMENT
};

// Structura pentru reprezentarea unui token
struct Token
{
    TokenType type;
    std::string value;
    int line;
    int column;
};

std::map<int, std::string> tokenTypeMap = {
    {KEYWORD, "Keyword"},
    {IDENTIFIER, "Identifier"},
    {SYMBOL, "Symbol"},
    {INTEGER_CONSTANT, "Integer Constant"},
    {FLOAT_CONSTANT, "Float Constant"},
    {STRING, "String"},
    {ERROR, "Error"},
    {PREPROCESSOR, "Preprocessor"},
    {OPERATOR, "Operator"},
    {COMMENT, "Comment"}
    // ... alte mapări ...
};
// Clasa pentru analizorul lexical
class LexicalAnalyzer
{
private:
    std::ifstream sourceFile;
    std::ofstream outputFile;
    std::vector<std::string> stringTable;
    std::unordered_map<std::string, TokenType> keywordTable;
    int currentLine;
    int currentColumn;

    void initializeKeywordTable()
    {
        // Inițializare tabelă cuvinte cheie
        keywordTable = {
            {"auto", KEYWORD},
            {"break", KEYWORD},
            {"case", KEYWORD},
            {"char", KEYWORD},
            {"const", KEYWORD},
            {"continue", KEYWORD},
            {"default", KEYWORD},
            {"do", KEYWORD},
            {"double", KEYWORD},
            {"else", KEYWORD},
            {"enum", KEYWORD},
            {"extern", KEYWORD},
            {"float", KEYWORD},
            {"for", KEYWORD},
            {"goto", KEYWORD},
            {"if", KEYWORD},
            {"int", KEYWORD},
            {"long", KEYWORD},
            {"register", KEYWORD},
            {"return", KEYWORD},
            {"short", KEYWORD},
            {"signed", KEYWORD},
            {"sizeof", KEYWORD},
            {"static", KEYWORD},
            {"struct", KEYWORD},
            {"switch", KEYWORD},
            {"typedef", KEYWORD},
            {"union", KEYWORD},
            {"unsigned", KEYWORD},
            {"void", KEYWORD},
            {"volatile", KEYWORD},
            {"while", KEYWORD}};
    }
    bool isKeyword(const std::string &word)
    {
        return keywordTable.find(word) != keywordTable.end();
    }
    bool isIdentifierChar(char ch)
    {
        return isalnum(ch) || ch == '_';
    }

    Token getIdentOrKeywordToken(std::string &buffer)
    {
        Token token;
        token.line = currentLine;
        token.column = currentColumn - buffer.length();
        if (isKeyword(buffer))
        {
            token.type = KEYWORD;
        }
        else
        {
            token.type = IDENTIFIER;
        }
        token.value = buffer;
        return token;
    }

    bool isDigit(char ch)
    {
        return isdigit(ch);
    }

    Token getConstantToken(std::string &buffer)
    {
        Token token;
        if (buffer.find('.') != std::string::npos)
        {
            // Dacă buffer-ul conține un punct, este o constantă float
            token.type = FLOAT_CONSTANT;
        }
        else
        {
            // Altfel, este o constantă întreagă
            token.type = INTEGER_CONSTANT;
        }
        token.value = buffer;
        token.line = currentLine;
        token.column = currentColumn - buffer.length();
        return token;
    }
    bool isOperatorChar(char ch)
    {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' ||
               ch == '==' || ch == '!=' || ch == '<' || ch == '>' || ch == '<=' ||
               ch == '>=' || ch == '&&' || ch == '||' || ch == '!' || ch == '=' ||
               ch == '+=' || ch == '-=' || ch == '&' || ch == '|' || ch == '^' ||
               ch == '~' || ch == '<<' || ch == '>>';
    }

    Token getOperatorToken(std::string &buffer)
    {
        Token token;
        token.type = OPERATOR;
        token.value = buffer;
        token.line = currentLine;
        token.column = currentColumn - buffer.length();
        return token;
    }
    bool isSymbolChar(char ch)
    {
        return ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
               ch == '[' || ch == ']' || ch == ';' || ch == ',' || ch == ':';
    }

    Token getSymbolToken(char ch)
    {
        Token token;
        token.type = SYMBOL;
        token.value = std::string(1, ch);
        token.line = currentLine;
        token.column = currentColumn;
        return token;
    }
    bool isStartOfComment(char ch1, char ch2)
    {
        return (ch1 == '/' && (ch2 == '/' || ch2 == '*'));
    }

    Token getCommentToken()
    {
        Token token;
        token.type = COMMENT;
        char ch;
        std::string buffer;
        sourceFile.get(ch); // Obținerea celui de-al doilea caracter al comentariului

        if (ch == '/')
        {
            // Comentariu pe o singură linie
            std::getline(sourceFile, buffer); // Citirea restului liniei
            buffer = "//" + buffer;           // Adăugarea '//'
        }
        else if (ch == '*')
        {
            // Comentariu pe mai multe linii
            buffer = "/*"; // Inițializarea buffer-ului cu '/*'
            while (sourceFile.get(ch))
            {
                buffer += ch;
                if (ch == '*' && sourceFile.peek() == '/')
                {
                    buffer += '/';    // Adăugarea '/'
                    sourceFile.get(); // Consumarea caracterului '/'
                    break;
                }
            }
        }

        token.value = buffer;
        token.line = currentLine;
        token.column = currentColumn - buffer.length();
        return token;
    }
    bool isStartOfPreprocessor(char ch)
    {
        return ch == '#';
    }

    Token getPreprocessorToken(std::string &buffer)
    {
        Token token;
        token.type = PREPROCESSOR;
        token.value = buffer;
        token.line = currentLine;
        token.column = currentColumn - buffer.length();
        return token;
    }
    bool isWhitespace(char ch)
    {
        return isspace(ch);
    }
    Token getErrorToken(std::string &error)
    {
        Token token;
        token.type = ERROR;
        token.value = error;
        token.line = currentLine;
        token.column = currentColumn;
        return token;
    }
    Token getStringToken(std::string &buffer)
    {
        Token token;
        token.type = STRING;
        token.value = buffer;
        token.line = currentLine;
        token.column = currentColumn - buffer.length();
        return token;
    }

public:
    LexicalAnalyzer(const std::string &inputFilename, const std::string &outputFilename) : currentLine(1), currentColumn(0)
    {
        sourceFile.open(inputFilename);
        outputFile.open(outputFilename);
        initializeKeywordTable();
    }

    ~LexicalAnalyzer()
    {
        sourceFile.close();
    }

    Token getToken()
    {
        char ch;
        if (!(sourceFile.get(ch)))
        {
            // Sfârșitul fișierului
            return Token{ERROR, "EOF", currentLine, currentColumn};
        }

        std::string buffer; // Buffer pentru a stoca caracterele tokenului curent
        buffer += ch;

        if (ch == '"')
        {                // Dacă este un caracter de început al stringului
            buffer = ch; // Inițializați buffer cu caracterul de început al stringului
            while (sourceFile.get(ch) && ch != '"')
            { // Continuați până când găsiți următorul caracter de ghilime
                buffer += ch;
            }
            if (ch == '"')
            {                                  // Verificați dacă bucla s-a terminat din cauza unui caracter de ghilime închis
                buffer += ch;                  // Adăugați caracterul de ghilime închis la buffer
                return getStringToken(buffer); // Presupunând că aveți o metodă pentru a obține un token de string din buffer
            }
            else
            {
                // Dacă bucla s-a terminat din cauza sfârșitului fișierului sau a unei alte condiții,
                // ar putea fi o eroare (de exemplu, un string neînchis)
                std::string error = "Lexical error: Unclosed string";
                return getErrorToken(error);
            }
        }
        // Verifică cuvintele cheie și identificatorii
        if (isalpha(ch) || ch == '_')
        {
            while (sourceFile.get(ch) && isIdentifierChar(ch))
            {
                buffer += ch;
            }
            sourceFile.unget(); // Pune ultimul caracter înapoi, deoarece nu face parte din token
            return getIdentOrKeywordToken(buffer);
        }

        // Verifică constantele
        if (isdigit(ch))
        {
            buffer = ch; // Inițializați buffer cu primul caracter numeric
            while (sourceFile.get(ch) && (isdigit(ch) || ch == '.'))
            {
                buffer += ch; // Adăugați restul numerelor și eventual punctul
            }
            // Reveniți la ultimul caracter citit, care nu este parte a numărului
            sourceFile.putback(ch);
            return getConstantToken(buffer);
        }

        // Verifică comentariile
        if (isStartOfComment(ch, sourceFile.peek()))
        {
            // ... logica pentru a consuma tot comentariul ...
            return getCommentToken();
        }
        // Verifică operatorii
        if (isOperatorChar(ch))
        {
            // ... logică similară ca mai sus, sau logica specifică pentru operatori ...
            return getOperatorToken(buffer);
        }

        // Verifică simbolurile
        if (isSymbolChar(ch))
        {
            return getSymbolToken(ch);
        }

        // Verifică directivele de preprocesare
        if (isStartOfPreprocessor(ch))
        {
            if (isStartOfPreprocessor(ch))
            {
                while (sourceFile.get(ch) && ch != '\n')
                {
                    buffer += ch;
                }
                // Eliminați orice caractere de sfârșit de linie de la sfârșitul bufferului
                if (buffer.back() == '\r')
                {
                    buffer.pop_back();
                }
                return getPreprocessorToken(buffer);
            }
        }

        // Verifică spațiile albe
        if (isWhitespace(ch))
        {
            // ... logica pentru a consuma toate spațiile albe consecutive ...
            // Nu returnăm un token pentru spații albe, ci pur și simplu facem recursiv pentru următorul token
            return getToken();
        }

        std::string error = "Lexical error: Unknown token";
        return getErrorToken(error);
    }

    void writeTokenToFile(const Token &token)
    {
        outputFile << "Token: " << token.value
                   << ", Type: " << tokenTypeMap[token.type]
                   << "\n"; // Trecere la linie nouă pentru fiecare token
    }
    // ... alte metode necesare ...
};

int main()
{
    LexicalAnalyzer lexer("input.txt", "output.txt");
    Token token;
    do
    {
        token = lexer.getToken();
        lexer.writeTokenToFile(token);
        // ... procesare token dacă este necesar ...
    } while (token.type != ERROR);

    return 0;
}