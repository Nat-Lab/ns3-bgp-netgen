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

typedef struct Filter {
    bool is_accept;
	bool is_exact;
    std::string prefix;
    std::string len;
} Filter;

typedef struct Filters {
    std::vector<Filter> filters;
    bool filter_defualt_accept;
} Filters;

typedef struct Peer {
    std::string address;
    uint32_t asn;
    std::string device;
    bool passive;
   
    Filters in_filter;
    Filters out_filter;
} Peer;
typedef std::vector<Peer> Peers;

typedef struct Route {
    std::string prefix;
    std::string len;
    std::string nexthop;
    std::string device;
    bool local;
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

typedef struct Options {
    std::vector<std::string> log;
    bool monitor;
    std::string monitor_trigger;
    std::string monitor_output;
    bool checksum;
    bool realtime;
} Options;

typedef struct SimulationConfigurtion {
    Networks networks;
    Routers routers;
    Options options;
} SimulationConfigurtion;