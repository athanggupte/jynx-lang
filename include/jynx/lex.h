#pragma once

#include "common.h"

#include <string_view>

namespace jynx {

enum Token {
    TOK_EOF = -1,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_NEWLINE,
    // Brackets
    TOK_LPAREN, // (
    TOK_RPAREN, // )
    TOK_LCURLY, // {
    TOK_RCURLY, // }
    TOK_LSQUARE, // [
    TOK_RSQUARE, // ]
    // Separators
    TOK_COMMA, // ,
    TOK_SEMICOLON, // ;
    TOK_COLON, // :
    TOK_SQUOTE, // '
    TOK_DQUOTE, // "
    // Arithmetic
    TOK_EQ, // =
    TOK_PLUS, // +
    TOK_MINUS, // -
    TOK_MULT, // *
    TOK_DIV, // /
    TOK_MOD, // %
    TOK_INC, // ++
    TOK_DEC, // --
    TOK_PLUSEQ, // +=
    TOK_MINUSEQ, // -=
    TOK_MULTEQ, // *=
    TOK_DIVEQ, // /=
    TOK_MODEQ, // %=
    // Logical
    TOK_OR, // ||
    TOK_AND, // &&
    TOK_NOT, // !
    TOK_OREQ, // ||=
    TOK_ANDEQ, // &&=
    // Comparison
    TOK_LT, // <
    TOK_GT, // >
    TOK_LTEQ, // <=
    TOK_GTEQ, // >=
    TOK_EQEQ, // ==
    TOK_NOTEQ, // !=
    // Bitwise
    TOK_BITOR, // |
    TOK_BITAND, // &
    TOK_NEG, // ~
    TOK_XOR, // ^
    TOK_LSHIFT, // <<
    TOK_RSHIFT, // >>
    TOK_BITOREQ, // |=
    TOK_BITANDEQ, // &=
    TOK_NEGEQ, // ~=
    TOK_XOREQ, // ^=
    TOK_LSHIFTEQ, // <<=
    TOK_RSHIFTEQ, // >>=
    // Special
    TOK_DOT, // .
    TOK_QUEST, // ?
    TOK_DOLLAR, // $
    TOK_HASH, // #
    TOK_AT, // @
    TOK_TICK, // `
    TOK_BSLASH, // \ back slash
    TOK_DSLASH, // // double slash
    
    TOK_UNK,
};

constexpr const char* token_string(Token tok) {
    switch (tok) {
    case TOK_EOF:           return "TOK_EOF";
    case TOK_IDENTIFIER:    return "TOK_IDENTIFIER";
    case TOK_NUMBER:    	return "TOK_NUMBER";
    case TOK_NEWLINE:       return "TOK_NEWLINE";
    case TOK_LPAREN:        return "TOK_LPAREN";
    case TOK_RPAREN:        return "TOK_RPAREN";
    case TOK_LCURLY:        return "TOK_LCURLY";
    case TOK_RCURLY:        return "TOK_RCURLY";
    case TOK_LSQUARE:       return "TOK_LSQUARE";
    case TOK_RSQUARE:       return "TOK_RSQUARE";
    case TOK_COMMA:         return "TOK_COMMA";
    case TOK_SEMICOLON:     return "TOK_SEMICOLON";
    case TOK_COLON:         return "TOK_COLON";
    case TOK_SQUOTE:        return "TOK_SQUOTE";
    case TOK_DQUOTE:        return "TOK_DQUOTE";
    case TOK_EQ:            return "TOK_EQ";
    case TOK_PLUS:          return "TOK_PLUS";
    case TOK_MINUS:         return "TOK_MINUS";
    case TOK_MULT:          return "TOK_MULT";
    case TOK_DIV:           return "TOK_DIV";
    case TOK_MOD:           return "TOK_MOD";
    case TOK_INC:           return "TOK_INC";
    case TOK_DEC:           return "TOK_DEC";
    case TOK_PLUSEQ:        return "TOK_PLUSEQ";
    case TOK_MINUSEQ:       return "TOK_MINUSEQ";
    case TOK_MULTEQ:        return "TOK_MULTEQ";
    case TOK_DIVEQ:         return "TOK_DIVEQ";
    case TOK_MODEQ:         return "TOK_MODEQ";
    case TOK_OR:            return "TOK_OR";
    case TOK_AND:           return "TOK_AND";
    case TOK_NOT:           return "TOK_NOT";
    case TOK_OREQ:          return "TOK_OREQ";
    case TOK_ANDEQ:         return "TOK_ANDEQ";
    case TOK_LT:            return "TOK_LT";
    case TOK_GT:            return "TOK_GT";
    case TOK_LTEQ:          return "TOK_LTEQ";
    case TOK_GTEQ:          return "TOK_GTEQ";
    case TOK_EQEQ:          return "TOK_EQEQ";
    case TOK_NOTEQ:         return "TOK_NOTEQ";
    case TOK_BITOR:         return "TOK_BITOR";
    case TOK_BITAND:        return "TOK_BITAND";
    case TOK_NEG:           return "TOK_NEG";
    case TOK_XOR:           return "TOK_XOR";
    case TOK_LSHIFT:        return "TOK_LSHIFT";
    case TOK_RSHIFT:        return "TOK_RSHIFT";
    case TOK_BITOREQ:       return "TOK_BITOREQ";
    case TOK_BITANDEQ:      return "TOK_BITANDEQ";
    case TOK_NEGEQ:         return "TOK_NEGEQ";
    case TOK_XOREQ:         return "TOK_XOREQ";
    case TOK_LSHIFTEQ:      return "TOK_LSHIFTEQ";
    case TOK_RSHIFTEQ:      return "TOK_RSHIFTEQ";
    case TOK_DOT:           return "TOK_DOT";
    case TOK_QUEST:         return "TOK_QUEST";
    case TOK_DOLLAR:        return "TOK_DOLLAR";
    case TOK_HASH:          return "TOK_HASH";
    case TOK_AT:            return "TOK_AT";
    case TOK_TICK:          return "TOK_TICK";
    case TOK_BSLASH:        return "TOK_BSLASH";
    case TOK_DSLASH:        return "TOK_DSLASH";
    case TOK_UNK:           return "TOK_UNK";
    }
}

class TokenStream {
public:
    TokenStream(std::string_view code);

    Token next();
    std::string_view consumeLine();
	std::string_view consumeString();
    std::string_view getTokenString();

private:
    std::string_view m_code;
    size_t m_rdnext;
    size_t m_rdprev;
};

}
