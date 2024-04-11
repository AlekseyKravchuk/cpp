#include "Common.h"

#include <chrono>
#include <unordered_map>
#include <set>
#include <utility>  // std::move
#include <mutex>

using namespace std;
using namespace std::chrono;

class LruCache : public ICache {
  public:
    LruCache(shared_ptr<IBooksUnpacker> books_unpacker,
             const Settings& settings)
            : _books_unpacker(std::move(books_unpacker)),
              _settings(settings) {}

    // Кэширование производится методом вытеснения давно неиспользуемых элементов (Least Recently Used, LRU).
    // Каждый элемент кэша имеет ранг.
    //  При вызове GetBook(), если книга с таким названием уже есть в кэше, её ранг поднимается до максимального
    //  (строго больше, чем у всех остальных).
    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> lock(_mu_books);
        Rank new_rank{book_name, std::chrono::steady_clock::now()};

        // Если книга с таким названием уже есть в кэше, её ранг поднимается до максимального (строго больше, чем у всех остальных)
        if (_books.count(book_name)) {
            Item& item = _books.at(book_name);
            _cache.erase(item.rank);
            _cache.insert(new_rank);
            item.rank = std::move(new_rank);

            return item.book_ptr;
        }

        // Если такой книги нет в кэше, то она добавляется в кэш, и её ранг, опять же, выставляется в максимальный.
        std::unique_ptr<IBook> unpacked_book_ptr = _books_unpacker->UnpackBook(book_name);
        BookPtr ptr{unpacked_book_ptr.release()};  // using BookPtr = std::shared_ptr<const IBook>;

        if (ptr->GetContent().size() <= _settings.max_memory) {
            auto it_begin = _cache.begin();
            auto it = it_begin;

            // если общий размер книг превышает ограничение max_memory, из кэша удаляются книги с наименьшим рангом, пока это необходимо
            while (_memory_used > 0 &&
                   _memory_used + ptr->GetContent().size() > _settings.max_memory) {
                _memory_used -= _books.at(it->book_name).book_ptr->GetContent().size();
                _books.erase(it->book_name);
                ++it;
            }

            _cache.erase(it_begin, it);

            _memory_used += ptr->GetContent().size();
            _books[book_name] = {new_rank, ptr};
            _cache.insert(new_rank);
        }

        return ptr;
    }

  private:
    struct Rank {
        string book_name;
        chrono::time_point<chrono::steady_clock> time;
    };

    struct Item {
        Rank rank;         // Rank = {book_name: string, time: chrono::time_point<chrono::steady_clock>}
        BookPtr book_ptr;  // BookPtr = std::shared_ptr<const IBook>;
    };

    struct RankLess {
        bool operator()(const Rank& lhs, const Rank& rhs) const {
            return lhs.time < rhs.time;
        }
    };

    shared_ptr<IBooksUnpacker> _books_unpacker;
    Settings _settings;
    size_t _memory_used{0};
    unordered_map<string, Item> _books;  // name => pair{priority, book_ptr}
    std::mutex _mu_books;
    set<Rank, RankLess> _cache;          // LRU-cache
};


unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker,
                             const ICache::Settings& settings) {
    return make_unique<LruCache>(std::move(books_unpacker), settings);
}
