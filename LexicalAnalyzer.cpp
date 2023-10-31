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
    {OPERATOR, "Operator"},
    {COMMENT, "Comment"}};

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
// Checks if a single character can be part of an operator
bool LexicalAnalyzer::isOperatorChar(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' ||
           ch == '!' || ch == '<' || ch == '>' || ch == '&' || ch == '|' ||
           ch == '=' || ch == '^' || ch == '~';
}

// Checks if a string forms a valid operator
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

Token LexicalAnalyzer::getCommentToken(const std::string &initialBuffer)
{
    Token token;
    token.type = COMMENT;
    std::string buffer = initialBuffer;
    char ch;

    if (initialBuffer == "//")
    {
        // Single-line comment
        std::getline(sourceFile, buffer); // Read the rest of the line
        buffer = "//" + buffer;           // Add '//' back to the beginning
    }
    else if (initialBuffer == "/*")
    {
        // Multi-line comment
        while (sourceFile.get(ch))
        {
            buffer += ch;
            if (ch == '*' && sourceFile.peek() == '/')
            {
                buffer += '/';    // Add '/'
                sourceFile.get(); // Consume the '/'
                break;
            }
        }
    }

    token.value = buffer;
    token.line = currentLine;
    token.column = currentColumn - buffer.length();
    return token;
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

// Token LexicalAnalyzer::getToken()
// {
//           char ch;
//           if (!(sourceFile.get(ch)))
//           {
//                     // Sfârșitul fișierului
//                     return Token{ERROR, "EOF", currentLine, currentColumn};
//           }

//           std::string buffer; // Buffer pentru a stoca caracterele tokenului curent
//           buffer += ch;

//           if (ch == '"')
//           {                      // Dacă este un caracter de început al stringului
//                     buffer = ch; // Inițializați buffer cu caracterul de început al stringului
//                     while (sourceFile.get(ch) && ch != '"')
//                     { // Continuați până când găsiți următorul caracter de ghilime
//                               buffer += ch;
//                     }
//                     if (ch == '"')
//                     {                                        // Verificați dacă bucla s-a terminat din cauza unui caracter de ghilime închis
//                               buffer += ch;                  // Adăugați caracterul de ghilime închis la buffer
//                               return getStringToken(buffer); // Presupunând că aveți o metodă pentru a obține un token de string din buffer
//                     }
//                     else
//                     {
//                               // Dacă bucla s-a terminat din cauza sfârșitului fișierului sau a unei alte condiții,
//                               // ar putea fi o eroare (de exemplu, un string neînchis)
//                               std::string error = "Lexical error: Unclosed string";
//                               return getErrorToken(error);
//                     }
//           }
//           // Verifică cuvintele cheie și identificatorii
//           if (isalpha(ch) || ch == '_')
//           {
//                     while (sourceFile.get(ch) && isIdentifierChar(ch))
//                     {
//                               buffer += ch;
//                     }
//                     sourceFile.unget(); // Pune ultimul caracter înapoi, deoarece nu face parte din token
//                     return getIdentOrKeywordToken(buffer);
//           }

//           // Verifică constantele
//           if (isdigit(ch))
//           {
//                     buffer = ch; // Inițializați buffer cu primul caracter numeric
//                     while (sourceFile.get(ch) && (isdigit(ch) || ch == '.'))
//                     {
//                               buffer += ch; // Adăugați restul numerelor și eventual punctul
//                     }
//                     // Reveniți la ultimul caracter citit, care nu este parte a numărului
//                     sourceFile.putback(ch);
//                     return getConstantToken(buffer);
//           }

//           // Verifică comentariile
//           if (isStartOfComment(ch, sourceFile.peek()))
//           {
//                     // ... logica pentru a consuma tot comentariul ...
//                     return getCommentToken();
//           }
//           else
//                     // Verifică operatorii
//                     if (isOperatorChar(ch))
//                     {
//                               // ... logică similară ca mai sus, sau logica specifică pentru operatori ...
//                               return getOperatorToken(buffer);
//                     }

//           // Verifică simbolurile
//           if (isSymbolChar(ch))
//           {
//                     return getSymbolToken(ch);
//           }

//           // Verifică directivele de preprocesare
//           if (isStartOfPreprocessor(ch))
//           {
//                     if (isStartOfPreprocessor(ch))
//                     {
//                               while (sourceFile.get(ch) && ch != '\n')
//                               {
//                                         buffer += ch;
//                               }
//                               // Eliminați orice caractere de sfârșit de linie de la sfârșitul bufferului
//                               if (buffer.back() == '\r')
//                               {
//                                         buffer.pop_back();
//                               }
//                               return getPreprocessorToken(buffer);
//                     }
//           }

//           // Verifică spațiile albe
//           if (isWhitespace(ch))
//           {
//                     // logica pentru a consuma toate spațiile albe consecutive ...
//                     // Nu returnăm un token pentru spații albe, ci pur și simplu facem recursiv pentru următorul token
//                     return getToken();
//           }

//           std::string error = "Lexical error: Unknown token";
//           return getErrorToken(error);
// }

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
            return getToken(); // Recursive call to skip whitespace
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
            currentState = READING_COMMENT_OR_OP; // Will decide in the next state
        }
        else if (isalpha(ch) || ch == '_')
        {
            currentState = READING_KEYWORD_ID; // Could be a keyword or identifier
        }
        else if (isDigit(ch))
        {
            currentState = READING_INT; // Start by assuming it's an integer
        }
        else if (isOperatorChar(ch))
        {
            currentState = READING_OPERATOR; // Could be a multi-character operator
        }
        else if (isSymbolChar(ch))
        {
            // currentState = READING_SYMBOL;
            // nu schimb starea ptc tot ce as face ar fi
            // sa stau in starea aia pana citesc un altfel de caracter
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
        // Buffer to hold the directive
        while (sourceFile.get(ch) && ch != '\n' && ch != '\r')
        {
            buffer += ch;
        }
        sourceFile.unget(); // Put the last character back, as it's not part of the directive
        Token token = getPreprocessorToken(buffer);
        buffer = "";
        currentState = START; // Reset the state to START for the next token
        return token;         // Assuming you have this method to generate a preprocessor token}
        break;
    }

    case READING_COMMENT_OR_OP:
    {
        sourceFile.get(ch);
        currentState = START;
        if (ch == '/')
        {
            Token token = getCommentToken("//");
            buffer = "";
            return token;
        } // Assuming you have this method to get comments}
        else if (ch == '*')
        {
            Token token = getCommentToken("/*");
            buffer = "";
            return token; // Assuming you have this method to get comments
        }
        else
        {
            std::string slash = "/";
            Token token = getOperatorToken(slash);
            buffer = "";
            return token; // Single '/' is an operator
        }
    }
    break;
    case READING_STRING:
    {
        bool escapeNextChar = false; // Flag for escaping the next character

        // Buffer to hold the string
        while (sourceFile.get(ch))
        {
            if (escapeNextChar)
            {
                // The previous character was a backslash, so add this character to the string regardless of what it is
                buffer += ch;
                escapeNextChar = false; // Reset the flag
            }
            else if (ch == '\\')
            {
                // The current character is a backslash, so set the flag to escape the next character
                buffer += ch;
                escapeNextChar = true;
            }
            else if (ch == '"')
            {
                // Found the end of the string
                buffer += ch;         // Include the closing '"' character
                currentState = START; // Reset the state for the next token
                Token token = getStringToken(buffer);
                buffer = "";
                return token; // Assuming you have this method to get string tokens
            }
            else
            {
                // Regular character within the string
                buffer += ch;
            }
        }

        // If we're here, it means we've hit the end of file before finding the end of the string
        buffer = "";
        std::string err = "Unclosed string";
        return getErrorToken(err);
    }
    break;

    case READING_KEYWORD_ID:
    {

        // Buffer to hold the potential keyword or identifier
        while (sourceFile.get(ch) && isIdentifierChar(ch))
        {
            buffer += ch;
        }

        sourceFile.unget(); // Put the last character back, as it's not part of the token
        currentState = START;
        Token token = getIdentOrKeywordToken(buffer);
        // Reset the state for the next token
        buffer = "";
        return token; // Your existing method to get either a keyword or identifier token
        break;
    } // ... other states
    case READING_INT:
    {

        while (sourceFile.get(ch) && isDigit(ch))
        {
            buffer += ch;
        }

        if (ch == '.')
        {
            // Transition to reading a float
            currentState = READING_FLOAT;
            buffer += ch; // Include the '.' character
        }
        else
        {
            // It's an integer
            currentState = START; // Reset the state for the next token
            sourceFile.unget();   // Put the last character back, as it's not part of the token
            Token token = getConstantToken(buffer);
            buffer = "";
            return token; // Assuming this method can handle integers
        }
    }
    break;

    case READING_FLOAT:
    { // Assume buffer already contains integer part and '.'
        while (sourceFile.get(ch) && isDigit(ch))
        {
            buffer += ch;
        }

        sourceFile.unget();   // Put the last character back, as it's not part of the token
        currentState = START; // Reset the state for the next token
        Token token = getConstantToken(buffer);
        buffer = "";
        return token; // Assuming this method can handle floats
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
    // if (isSymbolChar(ch))
    // {

    //     buffer += ch;
    //    // Your existing method to get a symbol token
    // }
    // else
    // {
    //     // This should technically not happen if your DFA is set up correctly
    //     std::string err = "Unrecognized symbol";
    //     buffer = "";
    //     return getErrorToken(err);
    // }
    break;
    case READING_OPERATOR:
    {
        currentState = START;
        char nextCh;
        if (sourceFile.get(nextCh))
        {
            std::string potentialOp = buffer + nextCh; // Combine current and next characters
            if (isOperatorChar(nextCh) && isValidOperator(potentialOp))
            {
                buffer += nextCh; // It's a multi-character operator
            }
            else
            {
                sourceFile.unget(); // Put the last character back, as it's not part of the operator
            }
        }
        Token token = getOperatorToken(buffer);
        buffer = "";
        return token;

    } // Your existing method to get an operator token
    break;
    default:
        std::string error = "Unrecognized state";
        buffer = "";
        return getErrorToken(error);
    }
    return Token{NONE, "NONE", currentLine, currentColumn};
}
