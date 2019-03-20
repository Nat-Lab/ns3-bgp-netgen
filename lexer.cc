#include "lexer.h"

const char* eat_space(const char *in) {
    while (*in == '\t' || *in == '\r' || *in == '\n' || *in == ' ') in++;
    return in;
}

LexOut any(const Lexers &lexs, const char *in, LexicalItems &out) {
    for (auto lex : lexs) {
        auto result = (*lex)(in, out);
        if (std::get<0>(result)) {
            return result;
        }
    }

    return LexOut (false, NULL);
}

LexOut lex_addr(const char *in, LexicalItems &out) {
    std::cmatch m_v4;

    if (std::regex_search(in, m_v4, r_v4, std::regex_constants::match_continuous)) {
        LexicalItem lex_item;
        lex_item.type = Type::VAR;
        lex_item.mtype = MinorType::VAR_ADDR;
        lex_item.item = m_v4[0];

        out.push_back(lex_item);
        return LexOut (true, in + lex_item.item.size());
    } else return LexOut (false, NULL);
}

LexOut lex_prefix_len(const char *in, LexicalItems &out) {
    std::cmatch m_len;

    if (std::regex_search(in, m_len, r_len, std::regex_constants::match_continuous)) {
        LexicalItem lex_item;
        lex_item.type = Type::VAR;
        lex_item.mtype = MinorType::VAR_PREFIX_LEN;
        lex_item.item = m_len[0];

        out.push_back(lex_item);
        return LexOut (true, in + lex_item.item.size());
    } else return LexOut (false, NULL);
}

LexOut lex_name(const char *in, LexicalItems &out) {
    std::cmatch m_name;

    if (std::regex_search(in, m_name, r_name, std::regex_constants::match_continuous)) {
        LexicalItem lex_item;
        lex_item.type = Type::VAR;
        lex_item.mtype = MinorType::VAR_NAME;
        lex_item.item = m_name[0];

        out.push_back(lex_item);
        return LexOut (true, in + lex_item.item.size());
    } else return LexOut (false, NULL);
}

LexOut lex_path(const char *in, LexicalItems &out) {
    std::cmatch m_path;

    if (std::regex_search(in, m_path, r_path, std::regex_constants::match_continuous)) {
        LexicalItem lex_item;
        lex_item.type = Type::VAR;
        lex_item.mtype = MinorType::VAR_PATH;
        lex_item.item = m_path[0];

        out.push_back(lex_item);
        return LexOut (true, in + lex_item.item.size());
    } else return LexOut (false, NULL);
}


LexOut lex_asn(const char *in, LexicalItems &out) {
    std::cmatch m_asn;

    if (std::regex_search(in, m_asn, r_asn, std::regex_constants::match_continuous)) {
        LexicalItem lex_item;
        lex_item.type = Type::VAR;
        lex_item.mtype = MinorType::VAR_ASN;
        lex_item.item = m_asn[0];

        out.push_back(lex_item);
        return LexOut (true, in + lex_item.item.size());
    } else return LexOut (false, NULL);
}

LexOut lex_keyword(const char *in, LexicalItems &out) {
    std::cmatch m_kw;

    if (std::regex_search(in, m_kw, r_kw, std::regex_constants::match_continuous)) {
        LexicalItem lex_item;
        lex_item.type = Type::KEYWORD;
        lex_item.item = m_kw[1];

        if (lex_item.item == "network") lex_item.mtype = MinorType::KW_NETWORK;
        if (lex_item.item == "prefix") lex_item.mtype = MinorType::KW_PREFIX;
        if (lex_item.item == "tap") lex_item.mtype = MinorType::KW_TAP;
        if (lex_item.item == "tap_name") lex_item.mtype = MinorType::KW_TAP_NAME;
        if (lex_item.item == "tap_mode") lex_item.mtype = MinorType::KW_TAP_MODE;
        if (lex_item.item == "tap_address") lex_item.mtype = MinorType::KW_TAP_ADDRESS;
        if (lex_item.item == "router") lex_item.mtype = MinorType::KW_ROUTER;
        if (lex_item.item == "as") lex_item.mtype = MinorType::KW_AS;
        if (lex_item.item == "dev") lex_item.mtype = MinorType::KW_DEV;
        if (lex_item.item == "device") lex_item.mtype = MinorType::KW_DEVICE;
        if (lex_item.item == "devices") lex_item.mtype = MinorType::KW_DEVICES;
        if (lex_item.item == "peer") lex_item.mtype = MinorType::KW_PEER;
        if (lex_item.item == "peers") lex_item.mtype = MinorType::KW_PEERS;
        if (lex_item.item == "route") lex_item.mtype = MinorType::KW_ROUTE;
        if (lex_item.item == "routes") lex_item.mtype = MinorType::KW_ROUTES;
        if (lex_item.item == "connect") lex_item.mtype = MinorType::KW_CONNECT;
        if (lex_item.item == "address") lex_item.mtype = MinorType::KW_ADDRESS;
        if (lex_item.item == "via") lex_item.mtype = MinorType::KW_VIA;
        if (lex_item.item == "options") lex_item.mtype = MinorType::KW_OPTIONS;
        if (lex_item.item == "log") lex_item.mtype = MinorType::KW_LOG;
        if (lex_item.item == "passive") lex_item.mtype = MinorType::KW_PASSIVE;
        if (lex_item.item == "local") lex_item.mtype = MinorType::KW_LOCAL;
        if (lex_item.item == "accept") lex_item.mtype = MinorType::KW_ACCEPT;
        if (lex_item.item == "reject") lex_item.mtype = MinorType::KW_REJECT;
        if (lex_item.item == "in_filter") lex_item.mtype = MinorType::KW_IN_FILTER;
        if (lex_item.item == "out_filter") lex_item.mtype = MinorType::KW_OUT_FILTER;
        if (lex_item.item == "default_action") lex_item.mtype = MinorType::KW_DEFAULT_ACTION;
        if (lex_item.item == "exact") lex_item.mtype = MinorType::KW_EXACT;
        if (lex_item.item == "monitor_trigger") lex_item.mtype = MinorType::KW_MONITOR_TRIGGER;
        if (lex_item.item == "monitor_output") lex_item.mtype = MinorType::KW_MONITOR_OUTPUT;
        if (lex_item.item == "monitor") lex_item.mtype = MinorType::KW_MONITOR;

        out.push_back(lex_item);
        return LexOut (true, in + lex_item.item.size());

    } else return LexOut (false, NULL);
}

LexOut lex_comment(const char *in, LexicalItems &out) {
    std::cmatch m;

    if (std::regex_search(in, m, r_comment, std::regex_constants::match_continuous)) {
        LexicalItem lex_item;
        std::string s = m[0];

        lex_item.type = Type::IGNORE;
        lex_item.mtype = MinorType::IGN_COMMENT;
        lex_item.item = m[0];

        //out.push_back(lex_item);
        return LexOut(true, in + lex_item.item.size());
    } else return LexOut(false, NULL);
}

LexOut lex_token(const char *in, LexicalItems &out) {
    std::cmatch m_token;

    if (std::regex_search(in, m_token, r_token, std::regex_constants::match_continuous)) {
        LexicalItem lex_item;
        lex_item.type = Type::TOKEN;
        lex_item.item = m_token[0];

        if (lex_item.item == "\r\n" || lex_item.item == "\n") {
            lex_item.type = Type::IGNORE;
            lex_item.mtype = MinorType::IGN_NEWLINE;
        }
        if (lex_item.item == "{") lex_item.mtype = MinorType::TKN_LBRACE;
        if (lex_item.item == "}") lex_item.mtype = MinorType::TKN_RBRACE;
        if (lex_item.item == ";") lex_item.mtype = MinorType::TKN_SEMICOL;

        out.push_back(lex_item);
        return LexOut (true, in + lex_item.item.size());

    } else return LexOut (false, NULL);
}

LexOut lex_bool(const char *in, LexicalItems &out) {
    std::cmatch m_token;

    if (std::regex_search(in, m_token, r_bool, std::regex_constants::match_continuous)) {
        LexicalItem lex_item;
        lex_item.type = Type::BOOL;
        lex_item.item = m_token[1];

        if (lex_item.item == "true" || lex_item.item == "on") 
            lex_item.mtype = MinorType::BOOL_TRUE;
        if (lex_item.item == "false" || lex_item.item == "off") 
            lex_item.mtype = MinorType::BOOL_FALSE;

        out.push_back(lex_item);
        return LexOut (true, in + lex_item.item.size());

    } else return LexOut (false, NULL);
}

LexOut lexer(const char *in, LexicalItems &out) {
    auto tail = in + strlen(in);

    Lexers lexs;
    lexs.push_back(&lex_comment);
    lexs.push_back(&lex_keyword);
    lexs.push_back(&lex_token);
    lexs.push_back(&lex_bool);
    lexs.push_back(&lex_name);
    lexs.push_back(&lex_addr);
    lexs.push_back(&lex_prefix_len);
    lexs.push_back(&lex_asn);
    lexs.push_back(&lex_path);

    in = eat_space(in);

    while (strlen(in) > 0) {
        auto r = any(lexs, in, out);

        if (!std::get<0> (r)) return LexOut (false, in);
        else in = std::get<1>(r);

        if (in > tail) return LexOut (false, tail);

        in = eat_space(in);
    }

    return LexOut (true, in);
}