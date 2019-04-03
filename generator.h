#include <cstdlib>
#include <iostream>

void generate_header(SimulationConfigurtion &conf);
void generate_footer(SimulationConfigurtion &conf);
void generate_device_setup(std::string ch, std::string node, std::string dev_name, std::string addr, std::string mask);
void generate_body(SimulationConfigurtion &conf);
void generate (SimulationConfigurtion &conf);
