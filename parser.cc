#include "lexer.h"
#include "parser.h"

ParOut par_network (const LexicalItems &in, uint32_t offset, SimulationConfigurtion &out) {
    auto cur = in[offset];
    if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);

    Network nw;
    nw.name = cur.item;

    cur = in[++offset];
    if (cur.mtype != MinorType::TKN_LBRACE) return ParOut (false, offset);

    cur = in[++offset];
    if (cur.type != Type::KEYWORD) return ParOut (false, offset);

    bool parsed = false;
    do {
        parsed = false;
        switch (cur.mtype) {
            case MinorType::KW_PREFIX:
                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_ADDR) return ParOut (false, offset);
                nw.prefix = cur.item;

                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_PREFIX_LEN) return ParOut (false, offset);
                nw.len = cur.item;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_TAP: 
                cur = in[++offset];
                if (cur.type != Type::BOOL) return ParOut (false, offset);
                nw.tap = (cur.mtype == MinorType::BOOL_TRUE) ? true : false;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_TAP_NAME: 
                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);
                nw.tap_name = cur.item;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_TAP_MODE: 
                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);
                nw.tap_mode = cur.item;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            default: parsed = false;
        }
    } while (parsed);

    if (cur.mtype != MinorType::TKN_RBRACE) return ParOut (false, offset);
    out.networks.push_back(nw);
    offset++;
    return ParOut (true, offset);
}

ParOut par_devices (const LexicalItems &in, uint32_t offset, Devices &out) {
    auto cur = in[offset];
    if (cur.mtype != MinorType::TKN_LBRACE) return ParOut (false, offset);

    while (true) {
        cur = in[++offset];
        if (cur.mtype != MinorType::KW_DEVICE) break;

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);
        Device dev;
        dev.name = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::KW_CONNECT) return ParOut (false, offset);

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);
        dev.connect_to = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::KW_ADDRESS) return ParOut (false, offset);

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_ADDR) return ParOut (false, offset);
        dev.address = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_PREFIX_LEN) return ParOut (false, offset);
        dev.len = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

        out.push_back(dev);
    }

    if (cur.mtype != MinorType::TKN_RBRACE) return ParOut (false, offset);
    offset++;
    return ParOut (true, offset);
}

ParOut par_peers (const LexicalItems &in, uint32_t offset, Peers &out) {
    auto cur = in[offset];
    if (cur.mtype != MinorType::TKN_LBRACE) return ParOut (false, offset);

    while (true) {
        cur = in[++offset];
        if (cur.mtype != MinorType::KW_PEER) break;

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_ADDR) return ParOut (false, offset);
        Peer p;
        p.address = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::KW_AS) return ParOut (false, offset);

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_ASN) return ParOut (false, offset);
        p.asn = std::stoi(cur.item);

        cur = in[++offset];
        if (cur.mtype != MinorType::KW_DEV) return ParOut (false, offset);

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);
        p.device = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

        out.push_back(p);
    }

    if (cur.mtype != MinorType::TKN_RBRACE) return ParOut (false, offset);
    offset++;
    return ParOut (true, offset);
}


ParOut par_routes (const LexicalItems &in, uint32_t offset, Routes &out) {
    auto cur = in[offset];
    if (cur.mtype != MinorType::TKN_LBRACE) return ParOut (false, offset);

    while (true) {
        cur = in[++offset];
        if (cur.mtype != MinorType::KW_ROUTE) break;

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_ADDR) return ParOut (false, offset);
        Route r;
        r.prefix = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_PREFIX_LEN) return ParOut (false, offset);
        r.len = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::KW_VIA) return ParOut (false, offset);

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_ADDR) return ParOut (false, offset);
        r.nexthop = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::KW_DEV) return ParOut (false, offset);

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);
        r.device = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

        out.push_back(r);
    }

    if (cur.mtype != MinorType::TKN_RBRACE) return ParOut (false, offset);
    offset++;
    return ParOut (true, offset);
}

ParOut par_router (const LexicalItems &in, uint32_t offset, SimulationConfigurtion &out) {
    auto cur = in[offset];
    if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);

    Router r;
    r.name = cur.item;

    cur = in[++offset];
    if (cur.mtype != MinorType::TKN_LBRACE) return ParOut (false, offset);

    cur = in[++offset];
    if (cur.type != Type::KEYWORD) return ParOut (false, offset);

    bool parsed = false;
    do {
        cur = in[offset];
        switch(cur.mtype) {
            case MinorType::KW_AS:
                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_ASN) return ParOut (false, offset);
                r.asn = std::stoi(cur.item);

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_DEVICES: {
                Devices devs;
                auto ret = par_devices(in, offset + 1, devs);
                offset = std::get<1> (ret);
                if (!(std::get<0> (ret))) return ret;
                r.devices = devs;
            }
            case MinorType::KW_PEERS: {
                Peers peers;
                auto ret = par_peers(in, offset + 1, peers);
                offset = std::get<1> (ret);
                if (!(std::get<0> (ret))) return ret;
                r.peers = peers;
                parsed = true;
                continue;
            }
            case MinorType::KW_ROUTES: {
                Routes rous;
                auto ret = par_routes(in, offset + 1, rous);
                offset = std::get<1> (ret);
                if (!(std::get<0> (ret))) return ret;
                r.routes = rous;
                parsed = true;
                continue;
            }
            default: parsed = false;
        }
    } while (parsed);

    if (cur.mtype != MinorType::TKN_RBRACE) return ParOut (false, offset);
    offset++;
    out.routers.push_back(r);
    return ParOut (true, offset);
}

ParOut par_options(const LexicalItems &in, uint32_t offset, SimulationConfigurtion &out) {
    auto cur = in[offset];
    if (cur.mtype != MinorType::TKN_LBRACE) return ParOut (false, offset);

    while (true) {
        cur = in[++offset];
        if (cur.mtype != MinorType::KW_LOG) break;

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);
        Option o;
        o.log = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

        out.options.push_back(o);
    }

    if (cur.mtype != MinorType::TKN_RBRACE) return ParOut (false, offset);
    offset++;
    return ParOut (true, offset);
}

ParOut parse(const LexicalItems &in, SimulationConfigurtion &out) {
    uint32_t offset = 0;
    auto cur = in[offset];
    if (cur.type != Type::KEYWORD) return ParOut (false, offset);

    while(offset < in.size()) {
        cur = in[offset];
        switch(cur.mtype) {
            case MinorType::KW_NETWORK: {
                auto ret = par_network(in, offset + 1, out);
                offset = std::get<1> (ret);
                if (!(std::get<0> (ret))) return ParOut (false, offset);
                continue;
            }
            case MinorType::KW_ROUTER: {
                auto ret = par_router(in, offset + 1, out);
                offset = std::get<1> (ret);
                if (!(std::get<0> (ret))) return ParOut (false, offset);
                continue;
            }
            case MinorType::KW_OPTIONS: {
                auto ret = par_options(in, offset + 1, out);
                offset = std::get<1> (ret);
                if (!(std::get<0> (ret))) return ParOut (false, offset);
                continue;
            }
            default: return ParOut (false, offset);
        }
    }

    return ParOut (true, offset);
}