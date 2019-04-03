#include <unistd.h>
#include <arpa/inet.h>
#include <cmath>
#include "types.h"
#include "generator.h"

std::string toaddrstr(uint32_t src) {
	char addr[INET_ADDRSTRLEN];
	uint32_t this_addr = htonl(src);
	inet_ntop(AF_INET, &this_addr, addr, INET_ADDRSTRLEN);
	std::string addr_str(addr);
	return addr_str;
}

int main (int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "%s: generate full binary tree topology.\n", argv[0]);
		fprintf(stderr, "usage: %s <level>\n", argv[0]);
		return 1;
	}

	int lvl = atoi(argv[1]);
	SimulationConfigurtion conf;

	conf.options.log.push_back("BGPSpeaker");
	conf.options.monitor = false;
	conf.options.realtime = true;
	conf.options.checksum = true;

	if (lvl < 2) {
		fprintf(stderr, "error: level need to be >= 2.");
		return 1;
	}

	int last_lvl = lvl - 1;
	int leaf_start = std::pow(2, last_lvl) - 1;
	uint32_t base;

	inet_pton(AF_INET, "10.0.0.0", &base);
	base = ntohl(base);

	Network net;
	net.name = "net0";
	net.prefix = "10.0.0.0";
	net.len = "/8";
	net.tap_address = "10.255.255.254";
	net.tap_address_len = "/8";
	net.tap_mode = "ConfigureLocal";
	net.tap_name = "tap-bgptree";

	conf.networks.push_back(net);

	const std::string r_prefix ("r");
	for (int i = 1; i < std::pow(2, lvl); i++) {
		std::string addr_str = toaddrstr(base + i);
		std::string r_name = r_prefix + std::to_string(i);
		Router r;
		r.name = r_name;
		r.asn = i;
		Device d;
		d.address = addr_str;
		d.len = "/8";
		d.name = "dev_" + r_name;
		d.connect_to = "net0";
		r.devices.push_back(d);
		if (i <= leaf_start) {
			Peer p1;
			p1.passive = false;
			p1.address = toaddrstr (base + (2 * i));
			p1.asn = 2 * i;
			p1.device = d.name;
			p1.in_filter.filter_defualt_accept = true;
			p1.out_filter.filter_defualt_accept = true;
			r.peers.push_back(p1);
			Peer p2;
			p2.passive = false;
			p2.address = toaddrstr (base + (2 * i + 1));
			p2.asn = 2 * i + 1;
			p2.device = d.name;
			p2.in_filter.filter_defualt_accept = true;
			p2.out_filter.filter_defualt_accept = true;
			r.peers.push_back(p2);
		}
		if (i > 0) {
			Peer pp;
			pp.passive = false;
			pp.address = toaddrstr (base + (i / 2));
			pp.asn = i / 2;
			pp.device = d.name;
			pp.in_filter.filter_defualt_accept = true;
			pp.out_filter.filter_defualt_accept = true;
			r.peers.push_back(pp);
		}
		conf.routers.push_back(r);
	}

	generate(conf);

	return 0;
}