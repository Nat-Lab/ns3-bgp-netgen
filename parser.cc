#include "lexer.h"
#include "parser.h"

ParOut par_network (const LexicalItems &in, uint32_t offset, SimulationConfigurtion &out) {
    auto cur = in[offset];
    if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);

    Network nw;
    nw.tap = false; // default
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
            case MinorType::KW_TAP_ADDRESS:
                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_ADDR) return ParOut (false, offset);
                nw.tap_address = cur.item;

                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_PREFIX_LEN) return ParOut (false, offset);
                nw.tap_address_len = cur.item;

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

ParOut par_filter (const LexicalItems &in, uint32_t offset, Filters &out) {
    auto cur = in[offset];

    if (cur.mtype != MinorType::TKN_LBRACE) return ParOut (false, offset);

    cur = in[++offset];
    if (cur.type != Type::KEYWORD) return ParOut (false, offset);

    bool parsed = false;
    do {
        cur = in[offset];
        switch(cur.mtype) {
            case MinorType::KW_DEFAULT_ACTION:
                cur = in[++offset];
                if (cur.mtype != MinorType::KW_ACCEPT && cur.mtype != MinorType::KW_REJECT)
                    return ParOut (false, offset);
                out.filter_defualt_accept = cur.mtype == MinorType::KW_ACCEPT;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_ACCEPT:
            case MinorType::KW_REJECT: {
                Filter f;
                f.is_accept = cur.mtype == MinorType::KW_ACCEPT;
                f.is_exact = false;

                cur = in[++offset];
                if (cur.mtype == MinorType::KW_EXACT) {
                    f.is_exact = true;
                    cur = in[++offset];
                }

                if (cur.mtype != MinorType::VAR_ADDR) return ParOut (false, offset);
                f.prefix = cur.item;

                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_PREFIX_LEN) return ParOut (false, offset);
                f.len = cur.item;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                out.filters.push_back(f);
                parsed = true;
                continue;
            }
            default: parsed = false;
        }
    } while (parsed);

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
        p.passive = false;
        p.in_filter.filter_defualt_accept = true;
        p.out_filter.filter_defualt_accept = true;

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

        if (cur.mtype == MinorType::KW_PASSIVE) {
            p.passive = true;
            cur = in[++offset];
        }

        if (cur.mtype == MinorType::TKN_LBRACE) {
            cur = in[++offset];
            if (cur.type != Type::KEYWORD) return ParOut (false, offset);

            bool parsed = false;
            do {
                cur = in[offset];
                parsed = false;
                switch (cur.mtype) {
                    case MinorType::KW_IN_FILTER: {
                        Filters f;
                        auto ret = par_filter(in, offset + 1, f);
                        offset = std::get<1> (ret);
                        if (!(std::get<0> (ret))) return ret;
                        p.in_filter = f;
                        parsed = true;
                        continue;
                    }
                    case MinorType::KW_OUT_FILTER: {
                        Filters f;
                        auto ret = par_filter(in, offset + 1, f);
                        offset = std::get<1> (ret);
                        if (!(std::get<0> (ret))) return ret;
                        p.out_filter = f;
                        parsed = true;
                        continue;
                    }
                    default: parsed = false;
                }
            } while(parsed);

        }

        if (cur.mtype == MinorType::TKN_SEMICOL) {
            out.push_back(p);
            continue;
        }

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
        r.local = false;

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_PREFIX_LEN) return ParOut (false, offset);
        r.len = cur.item;

        cur = in[++offset];
        if (cur.mtype == MinorType::KW_DEV) {
            r.nexthop = "0.0.0.0";
            goto p_r_dev;
        }
        if (cur.mtype != MinorType::KW_VIA) return ParOut (false, offset);

        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_ADDR) return ParOut (false, offset);
        r.nexthop = cur.item;

        cur = in[++offset];
        if (cur.mtype != MinorType::KW_DEV) return ParOut (false, offset);

        p_r_dev:
        cur = in[++offset];
        if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);
        r.device = cur.item;

        cur = in[++offset];
        if (cur.mtype == MinorType::KW_LOCAL) r.local = true;
        if (cur.mtype == MinorType::TKN_SEMICOL) {
            out.push_back(r);
            continue;
        }

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
                parsed = true;
                continue;
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

    cur = in[++offset];
    if (cur.type != Type::KEYWORD) return ParOut (false, offset);

    out.options.checksum = true;
    out.options.realtime = true;

    bool parsed = false;
    do {
        parsed = false;
        switch (cur.mtype) {
            case MinorType::KW_LOG:
                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_NAME) return ParOut (false, offset);
                out.options.log.push_back(cur.item);

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_MONITOR: 
                cur = in[++offset];
                if (cur.type != Type::BOOL) return ParOut (false, offset);
                out.options.monitor = (cur.mtype == MinorType::BOOL_TRUE) ? true : false;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_MONITOR_TRIGGER: 
                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_PATH) return ParOut (false, offset);
                out.options.monitor_trigger = cur.item;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_MONITOR_OUTPUT: 
                cur = in[++offset];
                if (cur.mtype != MinorType::VAR_PATH) return ParOut (false, offset);
                out.options.monitor_output = cur.item;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_CHECKSUM:
                cur = in[++offset];
                if (cur.type != Type::BOOL) return ParOut (false, offset);
                out.options.checksum = cur.mtype == MinorType::BOOL_TRUE;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            case MinorType::KW_REALTIME:
                cur = in[++offset];
                if (cur.type != Type::BOOL) return ParOut (false, offset);
                out.options.realtime = cur.mtype == MinorType::BOOL_TRUE;

                cur = in[++offset];
                if (cur.mtype != MinorType::TKN_SEMICOL) return ParOut (false, offset);

                cur = in[++offset];
                parsed = true;
                continue;
            default: parsed = false;
        }
    } while (parsed);

    if (cur.mtype != MinorType::TKN_RBRACE) return ParOut (false, offset);
    offset++;
    return ParOut (true, offset);
}

ParOut parse(const LexicalItems &in, SimulationConfigurtion &out) {
    memset(&out, 0, sizeof(SimulationConfigurtion));

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