#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>

struct Record;  // forward declaration

struct DBIterators {
    std::multimap<int, Record*>::iterator it_by_timestamp;
    std::multimap<int, Record*>::iterator it_by_karma;
    std::multimap<std::string, Record*>::iterator it_by_user;
};

struct Record {
    std::string id;
    std::string title;
    std::string user;
    int timestamp;
    int karma;
    DBIterators db_iterators{};

    bool operator==(const Record& other) const;
};

bool Record::operator==(const Record& other) const {
    return std::tuple(id, title, timestamp, karma) ==
           std::tuple(other.id, other.title, other.timestamp, other.karma);
}

class Database {
  public:
    bool Put(const Record& record);
    const Record* GetById(const std::string& id) const;
    bool Erase(const std::string& id);

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const;

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const;

    template <typename Callback>
    void AllByUser(const std::string& user, Callback callback) const;

  private:
    std::unordered_map<std::string, Record> _id_to_record;
    std::multimap<int, Record*> _ranged_by_timestamp;
    std::multimap<int, Record*> _ranged_by_karma;
    std::multimap<std::string, Record*> _ranged_by_user;

    template <typename Key, typename Callback>
    void RangeBy(const std::multimap<Key, Record*>& ranged_by_key,
                 const Key& low, const Key& high, Callback callback) const;
};

bool Database::Put(const Record& record) {
    auto [it, inserted] = _id_to_record.try_emplace(record.id, record);

    if (inserted) {
        auto address = std::addressof(it->second);
        it->second.db_iterators = {
            _ranged_by_timestamp.emplace(record.timestamp, address),
            _ranged_by_karma.emplace(record.karma, address),
            _ranged_by_user.emplace(record.user, address),
        };
    }

    return inserted;
}

const Record* Database::GetById(const std::string& id) const {
    auto it = _id_to_record.find(id);
    return it == end(_id_to_record) ? nullptr : std::addressof(it->second);
}

bool Database::Erase(const std::string& id) {
    auto it = _id_to_record.find(id);

    if (it == end(_id_to_record)) {
        return false;
    }

    _ranged_by_timestamp.erase(it->second.db_iterators.it_by_timestamp);
    _ranged_by_karma.erase(it->second.db_iterators.it_by_karma);
    _ranged_by_user.erase(it->second.db_iterators.it_by_user);
    _id_to_record.erase(it);

    return true;
}

template <typename Key, typename Callback>
void Database::RangeBy(const std::multimap<Key, Record*>& ranged_by_key,
                       const Key& low, const Key& high, Callback callback) const {
    if (low > high) {
        return;
    }

    auto lb = ranged_by_key.lower_bound(low);
    auto ub = ranged_by_key.upper_bound(high);
    for (auto it = lb; it != ub && callback(*it->second); ++it)
        ;
}

template <typename Callback>
void Database::RangeByTimestamp(int low, int high, Callback callback) const {
    RangeBy(_ranged_by_timestamp, low, high, callback);
}

template <typename Callback>
void Database::RangeByKarma(int low, int high, Callback callback) const {
    RangeBy(_ranged_by_karma, low, high, callback);
}

template <typename Callback>
void Database::AllByUser(const std::string& user, Callback callback) const {
    RangeBy(_ranged_by_user, user, user, callback);
}

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const std::string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    return 0;
}