#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <numeric>

//#include "htrie_map.h"
#include "htrie_set.h"

#include "keycrc.hpp"
#include "keysha3.hpp"
#include "utils.hpp"
#include "triededuplicator.hpp"

namespace fs = std::experimental::filesystem;

// 1347835 files
typedef tsl::htrie_set<char>               filelist_type; // VmSize:	  108308 kB
//typedef std::vector<std::string> filelist_type; // VmSize:	  210712 kB
//typedef std::set<std::string>    filelist_type; // VmSize:	  256236 kB

int main(int argc, char *argv[])
{
    unsigned N = sysconf (_SC_NPROCESSORS_ONLN);
    std::vector<filelist_type> vec_fset;
    vec_fset.resize(N);

    std::string start_dir(".");
    if (argc > 1)
        start_dir.assign(argv[1]);
    unsigned v = 0;

    auto dopts = fs::directory_options::skip_permission_denied;// || fs::directory_options::follow_directory_symlink;
    for(auto i = fs::recursive_directory_iterator(start_dir, dopts);
        i != fs::recursive_directory_iterator();
        ++i)
    {
/*
        std::cout << std::string(i.depth(), ' ') << *i;
        if(fs::is_symlink(i->symlink_status()))
            std::cout << " -> " << fs::read_symlink(*i);
        std::cout << '\n';
*/
        if(fs::is_regular_file(i->symlink_status()))
            vec_fset.at(v%N).insert(i->path().c_str());
        ++v;
        //fset.push_back(i->path().c_str());
    }

    std::cout << "done collecting filelist: "
              << std::accumulate(vec_fset.begin(), vec_fset.end(), 0UL,
                                           [](unsigned sum, filelist_type & a){ return sum + a.size(); })
              << std::endl;

    //typedef keys::KeyCRC key_computer;
    typedef keys::KeySHA3<64> key_computer;
    TrieDeduplicator<key_computer, filelist_type> tdd(N, vec_fset); // TODO: add some slow hash

    tdd.calculate_keys();
    std::cout << "done calculating keys" << std::endl;
    tdd.merge();
    std::cout << "done merging" << std::endl;
    tdd.print();

    return 0;
}
