#include "jynx/lex.h"
#include "jynx/vm.h"

#include <cstdio>
#include <fstream>
#include <string_view>

int main(int argc, char **argv) {
    std::string filepath;
    std::string input;
    bool captureInput = false;
    for (int i = 1; i < argc; i++) {
        std::string_view arg = argv[i];
        if (captureInput) {
            input.reserve(input.size() + arg.size());
            if (input.size() > 0)
                input.append(" ");
            input.append(arg);
            continue;
        }
        
        if (arg == "--") {
            captureInput = true;
            continue;
        }
        if (arg[0] == '-') {
            // options
            continue;
        }
        filepath = arg;
    }

    if (!filepath.empty()) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            printf("Could not open file \"%s\"\n", filepath.c_str());
            return 1;
        }

        file.seekg(0, std::ios::end);
        std::streamsize filesize = file.tellg();
        file.seekg(0, std::ios::beg);
        
        input.resize(filesize);
        file.read(input.data(), input.size());

        printf("file: %s\n", filepath.c_str());
    }

    if (input.empty()) {
        input = "const Vec2 = struct { x: f32, y: f32 };";
    }

    jynx::TokenStream stream(input);

    for (;;) {
        jynx::Token tok = stream.next();
        if (tok == jynx::TOK_EOF) {
            break;
        }
        std::string_view tokstr = stream.getTokenString();
        if (tok == jynx::TOK_UNK) {
            printf("Unkown symbol '%.*s'.\n", (int)tokstr.size(), tokstr.data());
            break;
        }
        if (tok == jynx::TOK_DSLASH) {
            // consume comment
            std::string_view comment = stream.consumeLine();
            printf("%-16s // %.*s\n", jynx::token_string(tok), (int)comment.size(), comment.data());
            continue;
        }
		if (tok == jynx::TOK_DQUOTE) {
			// consume string literal until newline
			std::string_view strlit = stream.consumeString();
			if (strlit.back() == '\n') {
				printf("Error: mismatched \" on line _\n");
				break;
			}
			printf("%-16s '%.*s'\n", jynx::token_string(tok), (int)strlit.size(), strlit.data());
		}
        if (tok == jynx::TOK_NEWLINE) {
            printf("%s\n", jynx::token_string(tok));
        }
        else {
            printf("%-16s %.*s\n", jynx::token_string(tok), (int)tokstr.size(), tokstr.data());
        }
    }

    return 0;
}