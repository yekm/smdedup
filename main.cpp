#include <iostream>
#include <string>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main(int argc, char *argv[])
{
    for(auto i = fs::recursive_directory_iterator(argv[1]);
             i != fs::recursive_directory_iterator();
           ++i ) {
        std::cout << std::string(i.depth(), ' ') << *i;
        if(fs::is_symlink(i->symlink_status()))
            std::cout << " -> " << fs::read_symlink(*i);
        std::cout << '\n';
    }

    return 0;
}
