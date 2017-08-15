#include <iostream>
#include <string>
#include <set>
#include <map>
#include <experimental/filesystem>
#include <numeric>

#include <chrono>
#include <thread>
#include <future>

//#include "htrie_map.h"
#include "htrie_set.h"

#include "keycrc.hpp"

namespace fs = std::experimental::filesystem;

// 1347835 files
typedef tsl::htrie_set<char>               filelist_type; // VmSize:	  108308 kB
//typedef std::vector<std::string> filelist_type; // VmSize:	  210712 kB
//typedef std::set<std::string>    filelist_type; // VmSize:	  256236 kB

//filelist_type fset;

std::map<keys::KeyCRC::key_type, filelist_type::const_iterator> dedup_map_crc;


template<typename Key>
struct TrieDeduplicator
{
    typedef std::multimap<typename Key::key_type, filelist_type::const_iterator> mmap_type;
    static mmap_type dedup(filelist_type & trie)
    {
        std::multimap<typename Key::key_type, filelist_type::const_iterator> d_map;
        std::string f_buffer;
        Key key;
        for(auto it = trie.begin(); it != trie.end(); ++it)
        {
            it.key(f_buffer);
            typename Key::key_type k;
            try
            {
                k = key.compute(f_buffer);
            }
            catch (std::runtime_error & e)
            {
                std::cerr << "cant open file " << f_buffer << std::endl;
                continue;
            }

            d_map.insert({k, it});
        }
        return std::move(d_map);
    }
};

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

    std::vector<std::future<TrieDeduplicator<keys::KeyCRC>::mmap_type>> f_mmaps;
    std::vector<TrieDeduplicator<keys::KeyCRC>::mmap_type> mmaps;
    f_mmaps.resize(N);
    mmaps.resize(N);
    for(unsigned i = 0; i<N;++i)
        f_mmaps.at(i) = std::async(
                    std::launch::async,
                    TrieDeduplicator<keys::KeyCRC>::dedup,
                    std::ref(vec_fset.at(i))
                    );

    for(unsigned i = 0; i<N;++i)
    {
        mmaps.at(i) = f_mmaps.at(i).get();
        //dedup_map_crc.merge(mmaps.at(i));
    }

    std::cout << "done calculating keys" << std::endl;

    for(unsigned i = 0; i<N;++i)
        dedup_map_crc.merge(mmaps.at(i));

    std::cout << "done merging" << std::endl;

    for(unsigned i = 0; i<N;++i)
    {
        for (auto && [k, v] : mmaps.at(i))
        {
            std::cout << "dup: " << v.key() << ", crc: " << k << std::endl;
        }
    }

    return 0;
}
