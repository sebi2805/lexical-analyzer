#include "LexicalAnalyzer.h"
#include <iostream>

std::map<int, std::string> tokenTypeMap = {
    {KEYWORD, "Keyword"},
    {IDENTIFIER, "Identifier"},
    {SYMBOL, "Symbol"},
    {INTEGER_CONSTANT, "Integer Constant"},
    {FLOAT_CONSTANT, "Float Constant"},
    {STRING, "String"},
    {ERROR, "Error"},
    {PREPROCESSOR, "Preprocessor"},
    {OPERATOR, "Operator"}};

std::unordered_map<std::string, TokenType> keywordTable = {
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

bool LexicalAnalyzer::isKeyword(const std::string &word)
{
    return keywordTable.find(word) != keywordTable.end();
}

bool LexicalAnalyzer::isIdentifierChar(char ch)
{
    return isalnum(ch) || ch == '_';
}

Token LexicalAnalyzer::getIdentOrKeywordToken(std::string &buffer)
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
    // pune referinta la string unic OBS: nu se copiaza stringul

    token.value = buffer;
    return token;
}

bool LexicalAnalyzer::isDigit(char ch)
{
    return isdigit(ch);
}

Token LexicalAnalyzer::getConstantToken(std::string &buffer)
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

// Verific daca caracterul este un operator
bool LexicalAnalyzer::isOperatorChar(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' ||
           ch == '!' || ch == '<' || ch == '>' || ch == '&' || ch == '|' ||
           ch == '=' || ch == '^' || ch == '~';
}

// Verific daca buffer-ul este un operator valid
bool LexicalAnalyzer::isValidOperator(const std::string &op)
{
    return op == "==" || op == "!=" || op == "<=" || op == ">=" ||
           op == "&&" || op == "||" || op == "!" || op == "=" ||
           op == "+=" || op == "-=" || op == "&" || op == "|" ||
           op == "^" || op == "~" || op == "<<" || op == ">>" ||
           op == "++" || op == "--";
}

Token LexicalAnalyzer::getOperatorToken(std::string &buffer)
{
    Token token;
    token.type = OPERATOR;
    token.value = buffer;
    token.line = currentLine;
    token.column = currentColumn - buffer.length();
    return token;
}
bool LexicalAnalyzer::isSymbolChar(char ch)
{
    return ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
           ch == '[' || ch == ']' || ch == ';' || ch == ',' || ch == ':';
}

Token LexicalAnalyzer::getSymbolToken(char ch)
{
    Token token;
    token.type = SYMBOL;
    token.value = std::string(1, ch);
    token.line = currentLine;
    token.column = currentColumn;
    return token;
}
bool LexicalAnalyzer::isStartOfComment(char ch1, char ch2)
{
    return (ch1 == '/' && (ch2 == '/' || ch2 == '*'));
}

void LexicalAnalyzer::getCommentToken(const std::string &initialBuffer)
{

    std::string buffer = initialBuffer;
    char ch;

    if (initialBuffer == "//")
    {
        //  Single-line comment
        std::getline(sourceFile, buffer);
        buffer = "//" + buffer;
    }
    else if (initialBuffer == "/*")
    {
        // Multi-line comment
        while (sourceFile.get(ch))
        {
            buffer += ch;
            if (ch == '*' && sourceFile.peek() == '/')
            {
                buffer += '/';
                sourceFile.get();
                break;
            }
        }
    }
    return;
}
bool LexicalAnalyzer::isStartOfPreprocessor(char ch)
{
    return ch == '#';
}

Token LexicalAnalyzer::getPreprocessorToken(std::string &buffer)
{
    Token token;
    token.type = PREPROCESSOR;
    token.value = buffer;
    token.line = currentLine;
    token.column = currentColumn - buffer.length();
    return token;
}

bool LexicalAnalyzer::isWhitespace(char ch)
{
    return ch == ' ' || ch == '\n' || ch == '\r';
}

Token LexicalAnalyzer::getErrorToken(std::string &error)
{
    Token token;
    token.type = ERROR;
    token.value = error;
    token.line = currentLine;
    token.column = currentColumn;
    return token;
}
Token LexicalAnalyzer::getStringToken(std::string &buffer)
{
    Token token;
    token.type = STRING;
    token.value = buffer;
    token.line = currentLine;
    token.column = currentColumn - buffer.length();
    return token;
}

void LexicalAnalyzer::writeTokenToFile(const Token &token)
{
    outputFile << "Token: " << token.value
               << " Type: " << tokenTypeMap[token.type]
               << "\n"; // Trecere la linie nouă pentru fiecare token
}

Token LexicalAnalyzer::getToken()
{
    char ch;

    switch (currentState)
    {
    case START:
    {
        if (!sourceFile.get(ch))
        {
            // End of file
            return Token{ERROR, "EOF", currentLine, currentColumn};
        }

        buffer += ch;

        if (isWhitespace(ch))
        {
            buffer = "";
            return getToken(); // Apeluri recursive pentru a scăpa de spații
        }
        else if (isStartOfPreprocessor(ch))
        {
            currentState = READING_PREPROCESSOR;
        }
        else if (ch == '"')
        {
            currentState = READING_STRING;
        }
        else if (ch == '/')
        {
            currentState = READING_COMMENT_OR_OP; // Stare intermediară pentru a distinge între comentarii și operatori
        }
        else if (isalpha(ch) || ch == '_')
        {
            currentState = READING_KEYWORD_ID; // Poate fi keyword sau identificator
        }
        else if (isDigit(ch))
        {
            currentState = READING_INT; //  Poate fi constantă întreagă sau float, dar asumam ca e integer pana la .
        }
        else if (isOperatorChar(ch))
        {
            currentState = READING_OPERATOR; //  Poate fi operator multi
        }
        else if (ch == '\'')
        {
            currentState = READING_CHAR;
        }
        else if (isSymbolChar(ch))
        {
            // currentState = READING_SYMBOL;
            // nu schimb starea ptc tot ce as face ar fi
            // sa stau in starea aia pana citesc un altfel de caracter
            // simbolurile sunt single
            Token token = getSymbolToken(ch);
            buffer = "";
            return token;
        }
        else
        {
            std::string err = "Unrecognized character";
            return getErrorToken(err);
        }
    }
    break;

    case READING_PREPROCESSOR:
    {
        while (sourceFile.get(ch) && ch != '\n' && ch != '\r')
        {
            buffer += ch;
        }
        sourceFile.unget();
        Token token = getPreprocessorToken(buffer);
        buffer = "";
        currentState = START;
        return token;
        break;
    }

    case READING_COMMENT_OR_OP:
    {
        sourceFile.get(ch);
        currentState = START;
        if (ch == '/')
        {
            getCommentToken("//");
            buffer = "";
            return getToken();
        }
        else if (ch == '*')
        {
            getCommentToken("/*");
            buffer = "";
            return getToken();
        }
        else
        {
            std::string slash = "/";
            Token token = getOperatorToken(slash);
            buffer = "";
            return token;
        }
    }
    break;
    case READING_STRING:
    {
        bool escapeNextChar = false;

        while (sourceFile.get(ch))
        {
            if (escapeNextChar)
            {
                buffer += ch;
                escapeNextChar = false;
            }
            else if (ch == '\\')
            {
                buffer += ch;
                escapeNextChar = true;
            }
            else if (ch == '"')
            {
                buffer += ch;
                currentState = START;
                Token token = getStringToken(buffer);
                buffer = "";
                return token;
            }
            else
            {
                buffer += ch;
            }
        }

        buffer = "";
        std::string err = "Unclosed string";
        return getErrorToken(err);
    }
    break;
    case READING_CHAR:
    {
        sourceFile.get(ch);
        buffer += ch;
        if (sourceFile.get(ch) && ch == '\'')
        {
            currentState = START;
            Token token;
            token.type = CHAR;
            token.value = buffer;
            buffer = "";
            return token;
        }
        else
        {
            std::string err = "Unclosed char";
            return getErrorToken(err);
        }
    }
    break;
    case READING_KEYWORD_ID:
    {

        while (sourceFile.get(ch) && isIdentifierChar(ch))
        {
            buffer += ch;
        }

        sourceFile.unget();
        currentState = START;
        Token token = getIdentOrKeywordToken(buffer);
        buffer = "";
        return token;
        break;
    }
    case READING_INT:
    {

        while (sourceFile.get(ch) && isDigit(ch))
        {
            buffer += ch;
        }

        if (ch == '.')
        {
            currentState = READING_FLOAT;
            buffer += ch;
        }
        else
        {
            currentState = START;
            sourceFile.unget();
            Token token = getConstantToken(buffer);
            buffer = "";
            return token;
        }
    }
    break;

    case READING_FLOAT:
    {
        while (sourceFile.get(ch) && isDigit(ch))
        {
            buffer += ch;
        }

        sourceFile.unget();
        currentState = START;
        Token token = getConstantToken(buffer);
        buffer = "";
        return token;
    }
    break;
    case READING_SYMBOL:
    {
        sourceFile.get(ch);
        if (!isSymbolChar(ch))
        {
            sourceFile.unget();
            currentState = START;
            buffer = "";
            return getToken();
        }
        else
        {
            Token token = getSymbolToken(ch);
            return token;
        }
    }

    break;
    case READING_OPERATOR:
    {
        currentState = START;
        char nextCh;
        if (sourceFile.get(nextCh))
        {
            std::string potentialOp = buffer + nextCh;
            if (isOperatorChar(nextCh) && isValidOperator(potentialOp))
            {
                buffer += nextCh;
            }
            else
            {
                sourceFile.unget();
            }
        }
        Token token = getOperatorToken(buffer);
        buffer = "";
        return token;
    }
    break;
    default:
        std::string error = "Unrecognized state";
        buffer = "";
        return getErrorToken(error);
    }
    return Token{NONE, "NONE", currentLine, currentColumn};
}
