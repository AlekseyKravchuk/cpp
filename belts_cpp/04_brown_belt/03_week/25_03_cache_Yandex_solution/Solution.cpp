#include "Common.h"
#include <unordered_map>
#include <list>
#include <mutex>
#include <utility>  // std::move

using namespace std;

class LruCache : public ICache {
  private:
    shared_ptr<IBooksUnpacker> _books_unpacker;
    list<BookPtr> _usedBooks;
    unordered_map<string, list<BookPtr>::iterator> _cache;
    size_t _memory_usage = 0;
    size_t _max_memory;
    mutex _mu;
  public:
    LruCache(shared_ptr<IBooksUnpacker> books_unpacker,
             const Settings& settings)
            : _books_unpacker(std::move(books_unpacker)),
              _max_memory(settings.max_memory) {}

    BookPtr GetBook(const string& book_name) override {
        unique_lock<mutex> lock(_mu);
        auto it = _cache.find(book_name);

        if (it != _cache.end()) {
            _usedBooks.splice(_usedBooks.end(), _usedBooks, it->second);
            it->second = prev(_usedBooks.end());
            return *it->second;
        } else {
            lock.unlock(); /// CONCURRENT AREA
            BookPtr book = _books_unpacker->UnpackBook(book_name);
            if (book->GetContent().size() > _max_memory)
                return book;
            lock.lock();  /// END OF CONCURRENT AREA

            _memory_usage += book->GetContent().size();
            if (_memory_usage > _max_memory) FreeCache();
            _usedBooks.push_back(std::move(book));
            _cache[book_name] = prev(_usedBooks.end());
            return _usedBooks.back();
        }
    }

  protected:
    void FreeCache() {
        while (!_usedBooks.empty() && _memory_usage > _max_memory) {
            _cache.erase(_usedBooks.front()->GetName());
            _memory_usage -= _usedBooks.front()->GetContent().size();
            _usedBooks.pop_front();
        }
    }
};

unique_ptr<ICache> MakeCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const ICache::Settings& settings
) {
    return make_unique<LruCache>(move(books_unpacker), settings);
}
