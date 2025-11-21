#include "jynx/lex.h"

#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstring>

#if 1
#   define debugf(fmt,...) (printf("[debug] " fmt "\n", ##__VA_ARGS__))
#else
#   define debugf(...)
#endif

namespace jynx {

TokenStream::TokenStream(std::string_view code)
    : m_code(code), m_rdnext(0), m_rdprev(0)
{
}

static bool isidentchar(char c) {
    return isalnum(c) || (c) == '_';
}

struct TokenCheckResult {
    Token tok;  // token found
    size_t cnt; // count of chars consumed
};

static TokenCheckResult check_token(char c0, char c1, char c2) {
    switch (c0) {
        case '\0':  return { TOK_EOF,           1 };
        case '(':   return { TOK_LPAREN,        1 };
        case ')':   return { TOK_RPAREN,        1 };
        case '{':   return { TOK_LCURLY,        1 };
        case '}':   return { TOK_RCURLY,        1 };
        case '[':   return { TOK_LSQUARE,       1 };
        case ']':   return { TOK_RSQUARE,       1 };
        case ',':   return { TOK_COMMA,         1 };
        case ';':   return { TOK_SEMICOLON,     1 };
        case ':':   return { TOK_COLON,         1 };
        case '\'':  return { TOK_SQUOTE,        1 };
        case '"':   return { TOK_DQUOTE,        1 };
        case '=': {
            switch (c1) {
            case '=':   return { TOK_EQEQ,      2 };
            default:    return { TOK_EQ,        1 };
            }
        }
        case '+': {
            switch (c1) {
            case '+':   return { TOK_INC,       2 };
            case '=':   return { TOK_PLUSEQ,    2 };
            default:    return { TOK_PLUS,      1 };
            }
        }
        case '-': {
            switch (c1) {
            case '-':   return { TOK_DEC,       2 };
            case '=':   return { TOK_MINUSEQ,   2 };
            default:    return { TOK_MINUS,     1 };
            }
        }
        case '*': {
            switch (c1) {
            case '=':   return { TOK_MULTEQ,    2 };
            default:    return { TOK_MULT,      1 };
            }
        }
        case '/': {
            switch (c1) {
            case '/':   return { TOK_DSLASH,    2 };
            case '=':   return { TOK_DIVEQ,     2 };
            default:    return { TOK_DIV,       1 };
            }
        }
        case '|': {
            switch (c1) {
            case '|': {
                switch (c2) {
                case '=':   return { TOK_OREQ,  3 };
                default:    return { TOK_OR,    2 };
                }
            }
            case '=':   return { TOK_BITOREQ,   2 };
            default:    return { TOK_BITOR,     1 };
            }
        }
        case '&': {
            switch (c1) {
            case '&': {
                switch (c2) {
                case '=':   return { TOK_ANDEQ, 3 };
                default:    return { TOK_AND,   2 };
                }
            }
            case '=':   return { TOK_BITANDEQ,  2 };
            default:    return { TOK_BITAND,    1 };
            }
        }
        case '<': {
            switch (c1) {
            case '<': {
                switch (c2) {
                case '=':   return { TOK_LSHIFTEQ,  3 };
                default:    return { TOK_LSHIFT,    2 };
                }
            }
            case '=':   return { TOK_LTEQ,      2 };
            default:    return { TOK_LT,        1 };
            }
        }
        case '>': {
            switch (c1) {
            case '>': {
                switch (c2) {
                case '=':   return { TOK_RSHIFTEQ,  3 };
                default:    return { TOK_RSHIFT,    2 };
                }
            }
            case '=':   return { TOK_GTEQ,      2 };
            default:    return { TOK_GT,        1 };
            }
        }
        case '.':       return { TOK_DOT,       1 };
    }
    return { TOK_UNK, 0 };
}

Token TokenStream::next() {
    if (m_code.size() == m_rdnext) {
        return TOK_EOF;
    }
    std::string_view tokstr = m_code.substr(m_rdnext);
    Token tok = TOK_UNK;
    size_t cnt = 0;
    while (isspace(tokstr[cnt]) && tokstr[cnt] != '\n') {
        cnt++;
    }
    tokstr.remove_prefix(cnt);
    m_rdnext += cnt;
    m_rdprev = m_rdnext;
    cnt = 0;
    while (cnt < tokstr.size() && tok == TOK_UNK) {
        char c0 = tokstr[cnt];
        char c1 = cnt+1 < tokstr.size() ? tokstr[cnt+1] : 0;
        char c2 = cnt+2 < tokstr.size() ? tokstr[cnt+2] : 0;
        auto [_tok, _cnt] = check_token(c0, c1, c2);
        if (_cnt != 0) {
            tok = _tok;
            cnt = _cnt;
            break;
        }
        // not a known special character
        if (c0 == '\n') {
            tok = TOK_NEWLINE;
            while (isspace(tokstr[cnt])) cnt++;
            break;
        }
		if (c0 == '0' && (c1 == 'x' || c1 == 'X')) {
			tok = TOK_NUMBER;
			while (isxdigit(tokstr[cnt])) cnt++;
			break;
		}
		if (isdigit(c0)) {
			tok = TOK_NUMBER;
			while (isdigit(tokstr[cnt])) cnt++;
			if (tokstr[cnt] == '.') cnt++;
			while (isdigit(tokstr[cnt])) cnt++;
			break;
		}
        if (isalnum(c0)) {
            tok = TOK_IDENTIFIER;
            while (isidentchar(tokstr[cnt])) cnt++;
            break;
        }
    }
    m_rdnext += cnt;
    return tok;
}

std::string_view TokenStream::consumeLine() {
    std::string_view tokstr = m_code.substr(m_rdnext);
    size_t nlpos = tokstr.find_first_of("\n");
    std::string_view line = tokstr.substr(0, nlpos);
    m_rdprev = m_rdnext;
    m_rdnext += nlpos+1; // consume '\n'
    return line;
}

std::string_view TokenStream::consumeString() {
	std::string_view tokstr = m_code.substr(m_rdnext);
	size_t endpos = tokstr.find_first_of("\"\n");
	std::string_view str = tokstr.substr(0, endpos);
	m_rdprev = m_rdnext;
	m_rdnext += endpos+1; // consume " or \n
	return str;
}
std::string_view TokenStream::getTokenString() {
    return m_code.substr(m_rdprev, m_rdnext - m_rdprev);
}

}
