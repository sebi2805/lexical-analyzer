#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <map>
#include <unordered_map>

enum LexerState
{
          START,                 // STAREA INITIALA
          READING_PREPROCESSOR,  // Reading after #
          READING_STRING,        // Reading after "
          READING_COMMENT_OR_OP, // Reading after /
          READING_KEYWORD_ID,    // Reading alphabets or _
          READING_SYMBOL,        // Reading potential symbols like (, ), {, }, etc.
          READING_OPERATOR,      // Reading potential operators like +, -, *, etc.
          READING_INT,           // Reading digits for an integer
          READING_FLOAT,         // Reading digits for a float after a '.'
          READING_CHAR
};

enum TokenType
{
          NONE,
          KEYWORD,
          IDENTIFIER,
          SYMBOL,
          INTEGER_CONSTANT,
          FLOAT_CONSTANT,
          STRING,
          ERROR,
          PREPROCESSOR,
          OPERATOR,
          CHAR,
          END_OF_FILE
};

// Structura pentru reprezentarea unui token
struct Token
{
          TokenType type;
          int index;
};

#endif