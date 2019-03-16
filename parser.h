#ifndef BGP_CONF_P_H_
#define BGP_CONF_P_H_

#include <vector>
#include <string>
#include <unistd.h>
#include "types.h"

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

/* peer block member */
ParOut par_filter (const LexicalItems &in, uint32_t offset, Filters &out);

ParOut parse(const LexicalItems &in, SimulationConfigurtion &out);

#endif // BGP_CONF_P_H_