#ifndef BGP_CONF_P_H_
#define BGP_CONF_P_H_

#include <vector>
#include <string>
#include <unistd.h>

typedef struct Network {
	std::string name;
	std::string prefix;
	std::string len;
} Network;
typedef std::vector<Network> Networks;

typedef struct Device {
	std::string name;
	std::string connect_to;
	std::string address;
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
	std::string nexthop;
	std::string device;
} Route;
typedef std::vector<Route> Routes;

typedef struct Router {
	uint32_t asn;
	Devices devices;
	Peers peers;
	Routes routes;
} Router;
typedef std::vector<Router> Routers;

typedef struct SimulationConfigurtion {
	Networks networks;
	Routers routers;
} SimulationConfigurtion;

#endif // BGP_CONF_P_H_