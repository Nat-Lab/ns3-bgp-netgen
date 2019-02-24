#include "lexer.h"
#include <iostream>

LexOut eat_garbage(std::string &in) {
    std::regex r_space ("^ *\t*(#.*\n)? *\t*");
    std::smatch m_space;

    if (std::regex_search(in, m_space, r_space)) {
        std::string space = m_space[0];
        str_shift(in, space.length());
    }
}

std::string str_shift(std::string &str, uint32_t n) {
    auto shifted = str.substr(0, n);
    str = str.substr(n, str.size() - n);
    return shifted;
}

LexOut any(const Lexers lexs, std::string &in) {
    for (auto lex : lexs) {
        auto out = (*lex)(in);
        if (std::get<0>(out)) return out;
    }

    return LexOut (false, NULL);
}

LexOut lex_addr(std::string &in) {
    std::regex r_v4 ("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
    std::smatch m_v4;

    if (std::regex_search(in, m_v4, r_v4)) {
        auto lex_item = new LexicalItem;
        lex_item->type = Type::VAR;
        lex_item->mtype = MinorType::VAR_ADDR;
        lex_item->item = m_v4[0];

        str_shift(in, lex_item->item.length());
        return LexOut (true, lex_item);
    } else return LexOut (false, NULL);
}

LexOut lex_prefix_len(std::string &in) {
    std::regex r_len ("^\\/[0-9]{1,2}");
    std::smatch m_len;

    if (std::regex_search(in, m_len, r_len)) {
        auto lex_item = new LexicalItem;
        lex_item->type = Type::VAR;
        lex_item->mtype = MinorType::VAR_PREFIX_LEN;
        lex_item->item = m_len[0];

        str_shift(in, lex_item->item.length());
        return LexOut (true, lex_item);
    } else return LexOut (false, NULL);
}

LexOut lex_name(std::string &in) {
    std::regex r_name ("^[a-zA-Z]+[a-zA-Z0-9\\-_]*");
    std::smatch m_name;

    if (std::regex_search(in, m_name, r_name)) {
        auto lex_item = new LexicalItem;
        lex_item->type = Type::VAR;
        lex_item->mtype = MinorType::VAR_NAME;
        lex_item->item = m_name[0];

        str_shift(in, lex_item->item.length());
        return LexOut (true, lex_item);
    } else return LexOut (false, NULL);
}

LexOut lex_asn(std::string &in) {
    std::regex r_asn ("^[0-9]+");
    std::smatch m_asn;

    if (std::regex_search(in, m_asn, r_asn)) {
        auto lex_item = new LexicalItem;
        lex_item->type = Type::VAR;
        lex_item->mtype = MinorType::VAR_ASN;
        lex_item->item = m_asn[0];

        str_shift(in, lex_item->item.length());
        return LexOut (true, lex_item);
    } else return LexOut (false, NULL);
}

LexOut lex_keyword(std::string &in) {
    std::regex r_kw ("^(network |prefix |tap(_(name|mode))? |router |as |dev(ices?)? |peers? |routes? |connect |address |via |options |log |tap_address |passive |passive;)");
    std::smatch m_kw;

    if (std::regex_search(in, m_kw, r_kw)) {
        auto lex_item = new LexicalItem;
        lex_item->type = Type::KEYWORD;
        lex_item->item = m_kw[0];

        if (lex_item->item == "passive;")
            str_shift(in, lex_item->item.length() - 1); // dirty
        else str_shift(in, lex_item->item.length());

        if (lex_item->item == "network ") lex_item->mtype = MinorType::KW_NETWORK;
        if (lex_item->item == "prefix ") lex_item->mtype = MinorType::KW_PREFIX;
        if (lex_item->item == "tap ") lex_item->mtype = MinorType::KW_TAP;
        if (lex_item->item == "tap_name ") lex_item->mtype = MinorType::KW_TAP_NAME;
        if (lex_item->item == "tap_mode ") lex_item->mtype = MinorType::KW_TAP_MODE;
        if (lex_item->item == "tap_address ") lex_item->mtype = MinorType::KW_TAP_ADDRESS;
        if (lex_item->item == "router ") lex_item->mtype = MinorType::KW_ROUTER;
        if (lex_item->item == "as ") lex_item->mtype = MinorType::KW_AS;
        if (lex_item->item == "dev ") lex_item->mtype = MinorType::KW_DEV;
        if (lex_item->item == "device ") lex_item->mtype = MinorType::KW_DEVICE;
        if (lex_item->item == "devices ") lex_item->mtype = MinorType::KW_DEVICES;
        if (lex_item->item == "peer ") lex_item->mtype = MinorType::KW_PEER;
        if (lex_item->item == "peers ") lex_item->mtype = MinorType::KW_PEERS;
        if (lex_item->item == "route ") lex_item->mtype = MinorType::KW_ROUTE;
        if (lex_item->item == "routes ") lex_item->mtype = MinorType::KW_ROUTES;
        if (lex_item->item == "connect ") lex_item->mtype = MinorType::KW_CONNECT;
        if (lex_item->item == "address ") lex_item->mtype = MinorType::KW_ADDRESS;
        if (lex_item->item == "via ") lex_item->mtype = MinorType::KW_VIA;
        if (lex_item->item == "options ") lex_item->mtype = MinorType::KW_OPTIONS;
        if (lex_item->item == "log ") lex_item->mtype = MinorType::KW_LOG;
        if (lex_item->item == "passive " || lex_item->item == "passive;") lex_item->mtype = MinorType::KW_PASSIVE;

        return LexOut (true, lex_item);

    } else return LexOut (false, NULL);
}

LexOut lex_comment(std::string &in) {
    std::regex r ("^#.*\\r?\\n");
    std::smatch m;

    if (std::regex_search(in, m, r)) {
        auto lex_item = new LexicalItem;
        std::string s = m[0];
        str_shift(in, s.length());

        lex_item->type = Type::IGNORE;
        lex_item->mtype = MinorType::IGN_COMMENT;
        lex_item->item = m[0];

        return LexOut(true, lex_item);
    } else return LexOut(false, NULL);
}

LexOut lex_token(std::string &in) {
    std::regex r_token ("^((\\r)?\\n|;|\\{|\\})");
    std::smatch m_token;

    if (std::regex_search(in, m_token, r_token)) {
        auto lex_item = new LexicalItem;
        lex_item->type = Type::TOKEN;
        lex_item->item = m_token[0];
        str_shift(in, lex_item->item.length());

        if (lex_item->item == "\r\n" || lex_item->item == "\n") {
            lex_item->type = Type::IGNORE;
            lex_item->mtype = MinorType::IGN_NEWLINE;
        }
        if (lex_item->item == "{") lex_item->mtype = MinorType::TKN_LBRACE;
        if (lex_item->item == "}") lex_item->mtype = MinorType::TKN_RBRACE;
        if (lex_item->item == ";") lex_item->mtype = MinorType::TKN_SEMICOL;

        return LexOut (true, lex_item);

    } else return LexOut (false, NULL);
}

LexOut lex_bool(std::string &in) {
    std::regex r_token ("^(on;|off;|true;|false;|on |off |true |false )");
    std::smatch m_token;

    if (std::regex_search(in, m_token, r_token)) {
        auto lex_item = new LexicalItem;
        lex_item->type = Type::BOOL;
        lex_item->item = m_token[0];
        if (lex_item->item == "on;" || lex_item->item == "off;" || lex_item->item == "true;" || lex_item->item == "false;") 
            str_shift(in, lex_item->item.length() - 1); // dirty
        else str_shift(in, lex_item->item.length());

        if (lex_item->item == "true " || lex_item->item == "on " || lex_item->item == "true;" || lex_item->item == "on;") 
            lex_item->mtype = MinorType::BOOL_TRUE;
        if (lex_item->item == "false " || lex_item->item == "off " || lex_item->item == "false;" || lex_item->item == "off;") 
            lex_item->mtype = MinorType::BOOL_FALSE;

        return LexOut (true, lex_item);

    } else return LexOut (false, NULL);
}

LexResult lexer(const std::string &in_c, LexicalItems &out) {
    std::string in (in_c);
    Lexers lexs;
    lexs.push_back(&lex_comment);
    lexs.push_back(&lex_keyword);
    lexs.push_back(&lex_token);
    lexs.push_back(&lex_bool);
    lexs.push_back(&lex_name);
    lexs.push_back(&lex_addr);
    lexs.push_back(&lex_prefix_len);
    lexs.push_back(&lex_asn);

    while (in.length() > 0) {
        eat_garbage(in);
        auto r = any(lexs, in);
        if(!std::get<0> (r)) return LexResult (false, in);
        else {
            auto item = std::get<1>(r);
            if (item->type != Type::IGNORE) out.push_back(*item);
        }
    }

    return LexResult (true, in);;
}