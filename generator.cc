#include "lexer.h"
#include "parser.h"
#include "generator.h"

void generate_header(SimulationConfigurtion &conf) {
    std::cout << R".(#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/bgp-helper.h"
#include "ns3/ipv4-address.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/bgp-monitor.h")." << std::endl;
    if (conf.options.prio_queue)
        std::cout << R".(#include "ns3/prio-queue.h")." << std::endl;
    else
        std::cout << R".(#include "ns3/drop-tail-queue.h")." << std::endl;
    std::cout << R".(using namespace ns3;)." << std::endl;
    if (conf.options.prio_queue) {
        std::cout << "bool _prio_queue_cb(Ptr<const Packet> " << conf.options.prio_queue_var << ") {" << conf.options.prio_queue_code << std::endl << "}" << std::endl;
    }
    std::cout << R".(int main () {
    InternetStackHelper _inet;)." << std::endl;
}

void generate_footer(SimulationConfigurtion &conf) {
    std::cout << R".(    Simulator::Run();
    return 0;
}
).";
}

void generate_device_setup(std::string ch, std::string node, std::string dev_name, std::string addr, std::string mask, Options &options) {
    printf("    Ptr<CsmaNetDevice> %s = CreateObject<CsmaNetDevice> ();\n", dev_name.c_str());
    printf("    %s->SetAddress (Mac48Address::Allocate ());\n", dev_name.c_str());
    printf("    %s->AddDevice(%s);\n", node.c_str(), dev_name.c_str());
    if (options.prio_queue) {
        printf("    Ptr<PrioQueue<Packet>> _queue_%s = CreateObject<PrioQueue<Packet>> ();\n", dev_name.c_str());
        printf("    _queue_%s->SetPrioCallback (MakeCallback(&_prio_queue_cb));\n", dev_name.c_str());
    } else printf("    Ptr<Queue<Packet>> _queue_%s = CreateObject<DropTailQueue<Packet>> ();\n", dev_name.c_str());
    printf("    %s->SetQueue (_queue_%s);\n", dev_name.c_str(), dev_name.c_str());
    printf("    %s->Attach (%s);\n", dev_name.c_str(), ch.c_str());
    printf("    int32_t %s_id = _ipv4_%s->AddInterface(%s);\n", dev_name.c_str(), node.c_str(), dev_name.c_str());
    printf("    Ipv4InterfaceAddress _addr_%s = Ipv4InterfaceAddress (Ipv4Address (\"%s\"), Ipv4Mask (\"%s\"));\n", dev_name.c_str(), addr.c_str(), mask.c_str());
    printf("    _ipv4_%s->AddAddress(%s_id, _addr_%s);\n", node.c_str(), dev_name.c_str(), dev_name.c_str());
    printf("    _ipv4_%s->SetMetric(%s_id, 1);\n", node.c_str(), dev_name.c_str());
    printf("    _ipv4_%s->SetUp(%s_id);\n", node.c_str(), dev_name.c_str());
}

void generate_body (SimulationConfigurtion &conf) {
    if (conf.options.realtime)
        printf("    GlobalValue::Bind(\"SimulatorImplementationType\", StringValue(\"ns3::RealtimeSimulatorImpl\"));\n");
    
    if (conf.options.checksum)
        printf("    GlobalValue::Bind(\"ChecksumEnabled\", BooleanValue (true));\n");

    for (auto option : conf.options.log) {
        printf("    LogComponentEnable(\"%s\", LOG_LEVEL_ALL);\n", option.c_str());
    }

    if (conf.options.monitor) printf("    BGPMonitor _mon (\"%s\", \"%s\");\n", conf.options.monitor_trigger.c_str(), conf.options.monitor_output.c_str());

    for (auto net : conf.networks) {
        printf("    Ptr<CsmaChannel> _net_%s = CreateObject<CsmaChannel> ();\n", net.name.c_str());

        if (net.tap) {
            std::string ch = "_net_" + net.name;
            std::string node = "_node" + ch + "_tap";
            std::string dev = "_dev" + node;
            printf("    Ptr<Node> %s = CreateObject<Node> ();\n", node.c_str());
            printf("    _inet.Install(NodeContainer (%s));\n", node.c_str());
            printf("    Ptr<Ipv4> _ipv4_%s = %s->GetObject<Ipv4> ();\n", node.c_str(), node.c_str());
            generate_device_setup(ch, node, dev, net.tap_address, net.tap_address_len, conf.options);
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
        printf("    Ptr<Ipv4> _ipv4_%s = %s->GetObject<Ipv4> ();\n", router_name.c_str(), router_name.c_str());
        for (auto device : router.devices) {
            std::string net_name = "_net_" + device.connect_to;
            std::string device_name = "_dev_" + router.name + "_" + device.name;
            generate_device_setup(net_name, router_name, device_name, device.address, device.len, conf.options);
        }

        //if (router.asn == 0) continue; // router does not has BGP

        std::string bgp_app_name = "_bgp_" + router.name;
        printf("    BGPHelper %s (%d);\n", bgp_app_name.c_str(), router.asn);

        for (auto peer : router.peers) {
            std::string dev_id_var = "_dev_" + router.name + "_" + peer.device + "_id";
            std::string filter_var = "_filter_" + router.name + peer.address;
            std::replace(filter_var.begin(), filter_var.end(), '.', '_');
            if (peer.passive) printf("    auto %s = %s.AddPeer(Ipv4Address(\"%s\"), %d, %s, true);\n", filter_var.c_str(), bgp_app_name.c_str(), peer.address.c_str(), peer.asn, dev_id_var.c_str());
            else printf("    auto %s = %s.AddPeer(Ipv4Address(\"%s\"), %d, %s);\n", filter_var.c_str(), bgp_app_name.c_str(), peer.address.c_str(), peer.asn, dev_id_var.c_str());

            if (peer.in_filter.filter_defualt_accept)
                printf("    %s.in_filter->default_op = BGPFilterOP::ACCEPT;\n", filter_var.c_str());
            else printf("    %s.in_filter->default_op = BGPFilterOP::REJECT;\n", filter_var.c_str());
            for (auto filter : peer.in_filter.filters) {
                if (filter.is_accept)
                    printf("    %s.in_filter->append (BGPFilterOP::ACCEPT, Ipv4Address(\"%s\"), Ipv4Mask(\"%s\"), %s);\n", filter_var.c_str(), filter.prefix.c_str(), filter.len.c_str(), filter.is_exact ? "true" : "false");
                else printf("    %s.in_filter->append (BGPFilterOP::REJECT, Ipv4Address(\"%s\"), Ipv4Mask(\"%s\"), %s);\n", filter_var.c_str(), filter.prefix.c_str(), filter.len.c_str(), filter.is_exact ? "true" : "false");
            }

            if (peer.out_filter.filter_defualt_accept)
                printf("    %s.out_filter->default_op = BGPFilterOP::ACCEPT;\n", filter_var.c_str());
            else printf("    %s.out_filter->default_op = BGPFilterOP::REJECT;\n", filter_var.c_str());
            for (auto filter : peer.out_filter.filters) {
                if (filter.is_accept)
                    printf("    %s.out_filter->append (BGPFilterOP::ACCEPT, Ipv4Address(\"%s\"), Ipv4Mask(\"%s\"), %s);\n", filter_var.c_str(), filter.prefix.c_str(), filter.len.c_str(), filter.is_exact ? "true" : "false");
                else printf("    %s.out_filter->append (BGPFilterOP::REJECT, Ipv4Address(\"%s\"), Ipv4Mask(\"%s\"), %s);\n", filter_var.c_str(), filter.prefix.c_str(), filter.len.c_str(), filter.is_exact ? "true" : "false");
            }
        }
            
        for (auto route : router.routes) {
            std::string dev_id_var = route.device == "lo" ? "0" : "_dev_" + router.name + "_" + route.device + "_id";
            if (route.local)printf("    %s.AddRoute(Ipv4Address(\"%s\"), %s, Ipv4Address(\"%s\"), %s, true);\n", bgp_app_name.c_str(), route.prefix.c_str(), route.len.substr(1, route.len.size() - 1).c_str(), route.nexthop.c_str(), dev_id_var.c_str());
            else printf("    %s.AddRoute(Ipv4Address(\"%s\"), %s, Ipv4Address(\"%s\"), %s);\n", bgp_app_name.c_str(), route.prefix.c_str(), route.len.substr(1, route.len.size() - 1).c_str(), route.nexthop.c_str(), dev_id_var.c_str());
        }

        printf("    auto %s_appc = %s.Install (%s);\n", bgp_app_name.c_str(), bgp_app_name.c_str(), router_name.c_str());
        if (conf.options.monitor) printf("    _mon.AddSpeaker(%s_appc.Get(0)->GetObject<BGPSpeaker> ());\n", bgp_app_name.c_str());
    }
    
    if (conf.options.monitor) printf("    _mon.Start();\n");
}

void generate (SimulationConfigurtion &conf) {
    generate_header(conf);
    generate_body(conf);
    generate_footer(conf);
}

