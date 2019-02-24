#ifndef BGP_CONF_P_H_
#define BGP_CONF_P_H_

#include <vector>
#include <string>
#include <unistd.h>

typedef struct Network {
    std::string name;
    std::string prefix;
    std::string len;

    bool tap;
    std::string tap_name;
    std::string tap_mode;
    std::string tap_address;
    std::string tap_address_len;
} Network;
typedef std::vector<Network> Networks;

typedef struct Device {
    std::string name;
    std::string connect_to;
    std::string address;
    std::string len;
} Device;
typedef std::vector<Device> Devices;

typedef struct Peer {
    std::string address;
    uint32_t asn;
    std::string device;
} Peer;
typedef std::vector<Peer> Peers;

typedef struct Route {
    std::string prefix;
    std::string len;
    std::string nexthop;
    std::string device;
} Route;
typedef std::vector<Route> Routes;

typedef struct Router {
    std::string name;
    uint32_t asn;
    Devices devices;
    Peers peers;
    Routes routes;
} Router;
typedef std::vector<Router> Routers;

typedef struct Option {
    std::string log;
} Option;
typedef std::vector<Option> Options;

typedef struct SimulationConfigurtion {
    Networks networks;
    Routers routers;
    Options options;
} SimulationConfigurtion;

typedef std::tuple<bool, uint32_t> ParOut;
typedef ParOut (*Parser) (const LexicalItems &in, uint32_t offset, SimulationConfigurtion &out);
typedef std::vector<Parser> Parsers;

/* First level */
ParOut par_network(const LexicalItems &in, uint32_t offset, SimulationConfigurtion &out);
ParOut par_router(const LexicalItems &in, uint32_t offset, SimulationConfigurtion &out);
ParOut par_options(const LexicalItems &in, uint32_t offset, SimulationConfigurtion &out);

/* router block member */
ParOut par_devices (const LexicalItems &in, uint32_t offset, Devices &out);
ParOut par_peers (const LexicalItems &in, uint32_t offset, Peers &out);
ParOut par_routes (const LexicalItems &in, uint32_t offset, Routes &out);

ParOut parse(const LexicalItems &in, SimulationConfigurtion &out);

#endif // BGP_CONF_P_H_