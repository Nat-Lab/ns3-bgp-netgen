#include "preprocessor.h"

void preprocessor_apply (std::string &in) {
    std::regex r_incl ("\\t* *@include ([^ \\t]+) *\\t*;");
    std::smatch m_incl;

    while (std::regex_search (in, m_incl, r_incl)) {
        std::ifstream incl_f (m_incl[1]);
        std::string incl_str ((std::istreambuf_iterator<char> (incl_f)), std::istreambuf_iterator<char>());
        preprocessor_apply (incl_str);
        in = std::regex_replace (in, r_incl, incl_str, std::regex_constants::format_first_only);
    }

}