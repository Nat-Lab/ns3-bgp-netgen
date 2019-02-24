ns3-bgp-netgen
---

ns3-bgp-netgen is a script generator for ns3 that allows you to create BGP networks in ns3 with ns3-bgp with few lines of configuration.

To get it, clone this repo and run `make`. 

To use it, do `./netgen config.conf > network.cc`

### Configuration

In the config, everything on a line after `#` is a comment, whitespace characters are treated as a single space. If options for a variable has variable length, they are grouped using the `{ }` brackets. Each option is terminated by a `;`. 

Here is an example of a config file. It defines a network with two routes and three networks. `home_router` and `office_router` have their own networks. (`10.254.0.0/24` for home and `10.254.3.0/24` for office.) And they have a BGP peering session on `10.254.2.0/24` to exchange that routing information.

```
options {
	log BGPSpeaker;
}

network home_network {
	prefix 10.254.0.0/24;
}

network peering_network {
	prefix 10.254.1.0/24;
}

network office_network {
	prefix 10.254.2.0/24;
}

router home_router {
	as 65001;
	devices {
		device dev_home connect home_network address 10.254.0.1/24;
		device dev_peer connect peering_network address 10.254.1.1/24;
	}
	peers {
		peer 10.254.1.2 as 65002 dev dev_peer;
	}
	routes {
		route 10.254.0.0/24 via 127.0.0.1 dev lo; # route to self
	}
}

router office_router {
	as 65002;
	devices {
		device dev_office connect office_network address 10.254.2.1/24;
		device dev_peer connect peering_network address 10.254.1.2/24;
	}
	peers {
		peer 10.254.1.1 as 65001 dev dev_peer;
	}
	routes {
		route 10.254.2.0/24 via 127.0.0.1 dev lo; # route to self
	}
}
```

### Blocks

In the root of the configuration document, three variables can be set. `options` for options, `network` for defining a network, and `router` for defining a router. 

#### The `options {}` block

The options block allows you to specify options for simulation. Currently, only `log` is supported. `log` will enable logging for the given ns3 component. For example:

```
options {
    log BGPSpeaker;
}
```

Set `LOG_LEVEL_ALL` for `BGPSpeaker`.

#### The `network {}` block

A network block defines a network. It has the following options:

option | required? | description | example
---|---|---|---
prefix | yes | The network prefix of this netowrk | `prefix 10.254.0.0/24;`
tap | no | Enable `TapBridge` on this network. | `tap on;`
tap\_name | if `tap on;`| The name of TAP interface. | `tap_name tap-ns3-in;`
tap\_mode | if `tap on;` | Mode of `TapBridge`. See [here](https://www.nsnam.org/docs/release/3.29/models/html/tap.html). | `tap_mode UseBridge;`
tap\_address | if `tap on;` | Address on TAP interface. | `tap_address 10.254.0.1/24;`

For example:

```
network net0 {
	prefix 10.254.0.0/24;
	tap on;
	tap_name tap-ns3-in;
	tap_mode ConfigureLocal;
	tap_address 10.254.0.1/24;
}
```

Will create a network with name `net0` and creates a TAP interface on ns3 host with name `tap-ns3-in` and address `10.254.0.1/24`, that allows the ns3 host to communicate with nodes connected to this network.

#### The `router {}` block

A router block defines a router. It has the following options:

option | required? | description | example
---|---|---|---
as | no | BGP ASN of this router | `as 65001;`
devices | no | List of devices. | n/a
peers | no | List of BGP peers. | n/a
routes | no | List of routes to insert to BGP NLRI. | n/a

#### The `devices {}` block

A devices block define devices for a router. It must be used inside the `router {}` block. It has only one option, `device`, which define a device. `device` option has the following syntax:

```
device <device_name> connect <network_name> address <address>/<prefix_length>;
```

Where `device_name` is the name of the device, you'll need this when specifying routes and peers. `network_name` is the name of the network to connect, and `<address>/<prefix_length>` is the address to use. For example:

```
devices {
	device dev_office connect office_network address 10.254.2.1/24;
}
```

Will add a network device onto the router with the name of `dev_office`, connects to `office_network` and has an IP address of `10.254.2.1/24`.

#### The `peers {}` block

A peers block define BGP peers for a router. It must be used inside the `router {}` block. It has only one option, `peer`, which define a BGP peer. `peer` option has the following syntax:

```
peer <peer_address> as <peer_asn> dev <device_name> [passive];
```

Where `peer_address` is the address of the peer, `peer_asn` is ASN of the peer, and `device_name` is the name of the device that the peer is on. Optionally, you can set the peering as passive with `passive`. This will instruct `BGPSpeaker` not to initiate the peering but wait for a connection from the peer.

For example:

```
peers {
	peer 10.254.2.1 as 65001 dev dev_1 passive;
}
```
Will create a passive BGP session with AS65001.

#### The `routes {}` block

A routes block define routes that are initially in a BGP router's NLRI (Network Layer Reachability Information). It must be used inside the `router {}` block. It has only one option, `route`, which define a route. `route` option has the following syntax:

```
route <network_prefix>/<prefix_length> via <nexthop_address> dev <device_name>;
```

Where `network_prefix/prefix_length` identifies the network, `nexthop_address` specify the network, and `device_name` define the device that nexthop is on.

For example:

```
routes {
	route 0.0.0.0/0 via 10.254.0.1 dev dev_internet;
}
```
Will do ns3 equivalent of iproute2 `ip rou add 0.0.0.0/0 via 10.254.0.1 dev dev_internet`.

### Licenses

MIT
