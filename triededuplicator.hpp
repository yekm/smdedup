#ifndef TRIEDEDUPLICATOR_HPP
#define TRIEDEDUPLICATOR_HPP

#include <map>
#include <future>

#include <atomic>

template<typename Key, typename List>
class TrieDeduplicator
{
public:
    typedef std::multimap<typename Key::key_type, typename List::const_iterator> mmap_type;

    TrieDeduplicator(int threads, std::vector<List> & vec_fset)
        : m_threads(threads)
        , f_mmaps(m_threads)
        , mmaps(m_threads)
    {
        for(unsigned i = 0; i<m_threads;++i)
            f_mmaps.at(i) = std::async(
                        std::launch::async,
                        &TrieDeduplicator<Key, List>::dedup,
                        this,
                        std::ref(vec_fset.at(i))
                        );
    }
    void calculate_keys()
    {
        for(unsigned i = 0; i<m_threads;++i)
            mmaps.at(i) = f_mmaps.at(i).get();
        std::cout << "total MB processed: " << m_total_bytes_processed/1024/1024 << std::endl;
    }

    void merge()
    {
        ssize_t m_duplicates = 0;
        for(unsigned i = 0; i<m_threads; ++i)
        {
            dedup_map.merge(mmaps.at(i));
            m_duplicates += mmaps.at(i).size();
        }
        std::cout << "total duplicates: " << m_duplicates << std::endl;
    }

    void print()
    {
        // TODO: find originals in mmaps
        for(unsigned i = 0; i<m_threads;++i)
            for (auto && [k, v] : mmaps.at(i))
                std::cout << "dup: " << v.key() << ", key: " << k << std::endl;
    }

    mmap_type dedup(List & trie)
    {
        std::multimap<typename Key::key_type, typename List::const_iterator> d_map;
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
                std::cerr << "cant open file " << f_buffer << ": "
                          << e.what() << std::endl;
                continue;
            }

            d_map.insert({k, it});
        }
        m_total_bytes_processed += key.get_total_bytes_processed();
        return std::move(d_map);
    }

private:
    int m_threads;
    std::vector<std::future<mmap_type>> f_mmaps;
    std::vector<mmap_type> mmaps;
    std::map<typename Key::key_type, typename List::const_iterator> dedup_map;
    std::atomic<ssize_t> m_total_bytes_processed = 0;
};

#endif // TRIEDEDUPLICATOR_HPP
