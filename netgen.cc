#include <cstdlib>
#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "generator.h"
#include "preprocessor.h"

int main (int argc, char **argv) {
    if (argc <= 1) {
        std::cerr << "usage: " << argv[0] << " simulation.conf" << std::endl;
        return 1;
    }

    std::ifstream conf (argv[1]);
    std::string in ((std::istreambuf_iterator<char> (conf)), std::istreambuf_iterator<char>());

    preprocessor_apply(in);

    LexicalItems lex_out;
    auto lex_ret = lexer(in, lex_out);

    if (!std::get<0> (lex_ret)) {
        std::cerr << "Lexer returned error." << std::endl;
        std::cerr << "Stppoed at: \"" << std::get<1> (lex_ret) << "\"";
        return 1;
    }

    SimulationConfigurtion par_out;
    auto par_ret = parse(lex_out, par_out);

    if (!std::get<0> (par_ret)) {
        std::cerr << "Parser returned error." << std::endl;
        uint32_t offset = std::get<1> (par_ret);
        std::cerr << "Unexpected token at token #" << offset << std::endl;
        std::cerr << "Token List: " << std::endl; 

        uint32_t cur = 0;
        for (auto tkn : lex_out) {
            cur++;
            std::cerr << cur << ": "  << tkn.item;
            if (cur == offset) std::cerr << " (parser error here)";
            std::cerr << std::endl;
        }
        return 1;
    }

    generate_header ();
    generate (par_out);
    generate_footer ();

    return 0;
}