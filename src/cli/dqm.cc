#include <iostream>
#include <fstream>
#include <vector>
#include "../readers/JSONReader.hh"
#include "../readers/RunListReader.hh"
#include "../plugins/Plugins.hh"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " runlist.txt" << std::endl;
        std::cout << "Plugins: " << std::endl;
        for (auto &name : Plugins::list()) {
            std::cout << "  " << name << std::endl;
        }
        return 0;
    }
    auto reader = new JSONReader();
    auto plugin = Plugins::get(argv[1]);
    if (!plugin) {
        exit(1);
    }
    plugin->setReader(reader);
    RunListReader rlr(argv[2]);
    plugin->setRunListReader(&rlr);
    plugin->Process();
    return 0;
}