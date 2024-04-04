#include "Common.h"

using namespace std;
using namespace std::chrono;

class LruCache : public ICache {
public:
    LruCache(shared_ptr<IBooksUnpacker> books_unpacker,
             const Settings& settings)
             : _books_unpacker(std::move(books_unpacker)),
               _settings(settings) { }

  BookPtr GetBook(const string& book_name) override {
    lock_guard<mutex> lock(_mu_books);
    Priority new_priority{book_name, chrono::steady_clock::now()};

    if (_books.count(book_name)) {
        Item& item = _books.at(book_name);
        _use_times.erase(item.current_priority);
        _use_times.insert(new_priority);
        item.current_priority = new_priority;

        return item.book_ptr;
    }

    std::unique_ptr<IBook> unpacked_book_ptr = _books_unpacker->UnpackBook(book_name);
    BookPtr result{unpacked_book_ptr.release()};  // using BookPtr = std::shared_ptr<const IBook>;

    if (result->GetContent().size() <= _settings.max_memory) {
        auto LRU_begin = _use_times.begin();
        auto LRU_end = LRU_begin;

        while (_memory_used > 0
               &&
               _memory_used + result->GetContent().size() > _settings.max_memory) {
            _memory_used -= _books.at(LRU_end->key).
        }
    }
  }
  private:
    struct Priority {
        string key;
        chrono::time_point<chrono::steady_clock> time;
    };

    struct PriorityLess {
        bool operator()(const Priority& lhs, const Priority& rhs) const {
            return lhs.time < rhs.time;
        }
    };

    struct Item {
        Priority current_priority;
        BookPtr book_ptr;  // BookPtr = std::shared_ptr<const IBook>;
    };

    shared_ptr<IBooksUnpacker> _books_unpacker;
    Settings _settings;
    size_t _memory_used{0};
    unordered_map<string, Item> _books;  // name => pair{priority, book_ptr}
    std::mutex _mu_books;
    set<Priority, PriorityLess> _use_times;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  // реализуйте функцию
}
