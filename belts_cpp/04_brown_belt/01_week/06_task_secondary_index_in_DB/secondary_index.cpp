#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

class Database {
  public:
    bool Put(const Record& record);
    const Record* GetById(const string& id) const;
    bool Erase(const string& id);

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const;

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const;

    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const;

  private:
    template <typename KeyType, typename Callback>
    void RangeBy(const multimap<KeyType, const Record*>& ranged_by_key,
                 const KeyType& low,
                 const KeyType& high,
                 Callback callback) const;

    struct ExtRecord {
        Record record;
        multimap<int, const Record*>::iterator _it_timestamp;
        multimap<int, const Record*>::iterator _it_karma;
        multimap<string, const Record*>::iterator _it_user;
    };

    unordered_map<string, ExtRecord> _storage;
    multimap<int, const Record*> _ranged_by_timestamp;
    multimap<int, const Record*> _ranged_by_karma;
    multimap<string, const Record*> _ranged_by_user;
};

bool Database::Put(const Record& record) {
    auto [it, inserted] = _storage.try_emplace(record.id, ExtRecord{record, {}, {}, {}});
    if (inserted) {
        ExtRecord& ext_record = it->second;
        Record* record_ptr = &(ext_record.record);

        ext_record._it_timestamp = _ranged_by_timestamp.emplace(record_ptr->timestamp, record_ptr);
        ext_record._it_karma = _ranged_by_karma.emplace(record_ptr->karma, record_ptr);
        ext_record._it_user = _ranged_by_user.emplace(record_ptr->user, record_ptr);
    }

    return inserted;
}

const Record* Database::GetById(const string& id) const {
    auto it{_storage.find(id)};
    if (it == _storage.end()) {
        return nullptr;
    }

    return &(it->second.record);
}

bool Database::Erase(const string& id) {
    auto it{_storage.find(id)};

    if (it == _storage.end()) {
        return false;
    }

    auto& [_, ext_record] = *it;
    _ranged_by_timestamp.erase(ext_record._it_timestamp);
    _ranged_by_karma.erase(ext_record._it_karma);
    _ranged_by_user.erase(ext_record._it_user);
    _storage.erase(it);

    return true;
}

template <typename KeyType, typename Callback>
void Database::RangeBy(const multimap<KeyType, const Record*>& ranged_by_key,
                       const KeyType& low,
                       const KeyType& high,
                       Callback callback) const {
    auto lb = ranged_by_key.lower_bound(low);
    auto ub = ranged_by_key.upper_bound(high);

    if (lb == ub) {
        return;
    }

    for (auto it = lb; it != ub && callback(*(it->second)); ++it) {
    }
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
void Database::AllByUser(const string& user, Callback callback) const {
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
    const string final_body = "Feeling sad";

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
