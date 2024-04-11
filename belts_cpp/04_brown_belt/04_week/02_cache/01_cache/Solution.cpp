#include "Common.h"

#include <unordered_map>
#include <mutex>
#include <cassert>
#include <utility>    // std::move
#include <algorithm>  // std::min_element

using namespace std;

// Least Recently Used, LRU
class LruCache : public ICache {
public:
    LruCache(shared_ptr<IBooksUnpacker> books_unpacker,
             const Settings& settings)
            : _books_unpacker(std::move(books_unpacker)),
              _settings(settings) {}

  BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> guard(_mu);
        auto it = _books.find(book_name);

        if (it == _books.end()) {
            Entry entry;
            entry.book_ptr = _books_unpacker->UnpackBook(book_name);
            auto book_size = entry.book_ptr->GetContent().size();

            while (!_books.empty() && (_cache_size + book_size > _settings.max_memory)) {
                Remove_LRU_Entry();
            }

            assert(!_books.empty() || _cache_size == 0);

            if (book_size > _settings.max_memory) {
                return  std::move(entry.book_ptr);
            }

            it = AddEntry(book_name, std::move(entry));
        }

        it->second.rank = ++_current_rank;
        return it->second.book_ptr;
  }

  private:
    struct Entry {
      BookPtr book_ptr;  // std::shared_ptr<const IBook>
      int rank = 0;
  };

  using Entries = unordered_map<string, Entry>;

  private:
    void Remove_LRU_Entry() {
        assert(!_books.empty());

        auto it = std::min_element(_books.begin(),
                                   _books.end(),
                                   [](const Entries::value_type& lhs, const Entries::value_type& rhs) {
                                       return lhs.second.rank > rhs.second.rank;
                                   });
        _cache_size -= it->second.book_ptr->GetContent().size();
        _books.erase(it);
    }

    Entries::iterator AddEntry(const string& book_name, Entry entry) {
        assert(0 == _books.count(book_name));
        _cache_size += entry.book_ptr->GetContent().size();
        return _books.insert({book_name, std::move(entry)}).first;
    }

  private:
    shared_ptr<IBooksUnpacker> _books_unpacker;
    const Settings _settings;

    mutex _mu;
    Entries _books; // unordered_map<string, Entry>
    int _current_rank = 0;
    size_t _cache_size = 0;
};


unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker,
                             const ICache::Settings& settings) {
    return make_unique<LruCache>(std::move(books_unpacker), settings);
}
