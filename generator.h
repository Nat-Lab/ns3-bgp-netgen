#include <cstdlib>
#include <iostream>

void generate_header();
void generate_footer();
void generate_device_setup(std::string ch, std::string node, std::string dev_name, std::string addr, std::string mask);
void generate (SimulationConfigurtion &conf);
