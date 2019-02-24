#include "lexer.h"
#include "parser.h"
#include "generator.h"

void generate_header() {
    std::cout << R".(#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/bgp-speaker.h"
#include "ns3/bgp-peer.h"
#include "ns3/bgp-route.h"
#include "ns3/bgp-helper.h"
#include "ns3/ipv4-address.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/packet.h"
using namespace ns3;
int main () {
    InternetStackHelper _inet;
    LogComponentEnable("BGPSpeaker", LOG_LEVEL_ALL);
    GlobalValue::Bind("SimulatorImplementationType", StringValue("ns3::RealtimeSimulatorImpl"));
    GlobalValue::Bind("ChecksumEnabled", BooleanValue (true));).";
}

void generate_footer() {
    std::cout << R".(    Simulator::Run();
    return 0;
}
).";
}

void generate_device_setup(std::string ch, std::string node, std::string dev_name, std::string addr, std::string mask) {
    printf("    Ptr<CsmaNetDevice> %s = CreateObject<CsmaNetDevice> ();\n", dev_name.c_str());
    printf("    %s->SetAddress (Mac48Address::Allocate ());\n", dev_name.c_str());
    printf("    %s->AddDevice(%s);\n", node.c_str(), dev_name.c_str());
    printf("    %s->AddDevice(%s);\n", node.c_str(), dev_name.c_str());
    printf("    Ptr<Queue<Packet>> _queue_%s = CreateObject<DropTailQueue<Packet>> ();\n", dev_name.c_str());
    printf("    %s->SetQueue (_queue_%s);\n", dev_name.c_str(), dev_name.c_str());
    printf("    %s->Attach (%s);\n", dev_name.c_str(), ch.c_str());
    printf("    Ptr<Ipv4> _ipv4_%s = %s->GetObject<Ipv4> ();\n", dev_name.c_str(), node.c_str());
    printf("    int32_t %s_id = _ipv4_%s->AddInterface(%s);\n", dev_name.c_str(), dev_name.c_str(), dev_name.c_str());
    printf("    Ipv4InterfaceAddress _addr_%s = Ipv4InterfaceAddress (Ipv4Address (\"%s\"), Ipv4Mask (\"%s\"));\n", dev_name.c_str(), addr.c_str(), mask.c_str());
    printf("    _ipv4_%s->AddAddress(%s_id, _addr_%s);\n", dev_name.c_str(), dev_name.c_str(), dev_name.c_str());
    printf("    _ipv4_%s->SetMetric(%s_id, 1);\n", dev_name.c_str(), dev_name.c_str());
    printf("    _ipv4_%s->SetUp(%s_id);\n", dev_name.c_str(), dev_name.c_str());
}

void generate (SimulationConfigurtion &conf) {
    for (auto option : conf.options) {
        printf("    LogComponentEnable(\"%s\", LOG_LEVEL_ALL);\n", option.log.c_str());
    }

    for (auto net : conf.networks) {
        printf("    Ptr<CsmaChannel> _net_%s = CreateObject<CsmaChannel> ();\n", net.name.c_str());

        if (net.tap) {
            std::string ch = "_net_" + net.name;
            std::string node = "_node" + ch + "_tap";
            std::string dev = "_dev" + node;
            printf("    Ptr<Node> %s = CreateObject<Node> ();\n", node.c_str());
            printf("    _inet.Install(NodeContainer (%s));\n", node.c_str());
            generate_device_setup(ch, node, dev, net.tap_address, net.tap_address_len);
            printf("    TapBridgeHelper _tbh_%s;\n", net.name.c_str());
            printf("    _tbh_%s.SetAttribute (\"DeviceName\", StringValue(\"%s\"));\n", net.name.c_str(), net.tap_name.c_str());
            printf("    _tbh_%s.SetAttribute (\"Mode\", StringValue(\"%s\"));\n", net.name.c_str(), net.tap_mode.c_str());
            printf("    _tbh_%s.Install (%s, %s);\n", net.name.c_str(), node.c_str(), dev.c_str());
        }
    }

    for (auto router : conf.routers) {
        std::string router_name = "_r_" + router.name;
        printf("    Ptr<Node> %s = CreateObject<Node> ();\n", router_name.c_str());
        printf("    _inet.Install(NodeContainer (%s));\n", router_name.c_str());

        for (auto device : router.devices) {
            std::string net_name = "_net_" + device.connect_to;
            std::string device_name = "_dev_" + router.name + "_" + device.name;
            generate_device_setup(net_name, router_name, device_name, device.address, device.len);
        }

        if (router.asn == 0) continue; // router does not has BGP

        std::string bgp_app_name = "_bgp_" + router.name;
        printf("    BGPHelper %s (%d);\n", bgp_app_name.c_str(), router.asn);

        

        for (auto peer : router.peers) {
            std::string dev_id_var = "_dev_" + router.name + "_" + peer.device + "_id";
            if (peer.passive) printf("    %s.AddPeer(Ipv4Address(\"%s\"), %d, %s, true);\n", bgp_app_name.c_str(), peer.address.c_str(), peer.asn, dev_id_var.c_str());
            else printf("    %s.AddPeer(Ipv4Address(\"%s\"), %d, %s);\n", bgp_app_name.c_str(), peer.address.c_str(), peer.asn, dev_id_var.c_str());
        }
            
        for (auto route : router.routes) {
            std::string dev_id_var = route.device == "lo" ? "0" : "_dev_" + router.name + "_" + route.device + "_id";
            printf("    %s.AddRoute(Ipv4Address(\"%s\"), %s, Ipv4Address(\"%s\"), %s);\n", bgp_app_name.c_str(), route.prefix.c_str(), route.len.substr(1, route.len.size() - 1).c_str(), route.nexthop.c_str(), dev_id_var.c_str());
        }

        printf("    %s.Install (%s);\n", bgp_app_name.c_str(), router_name.c_str());
    }
    
}

