#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
  public:
    ReadingManager();

    void Read(int uid, int page_count);
    double Cheer(int uid) const;

  private:
    int GetUserCount() const;
    void UpdatePageRange(int lower, int upper);  // lower: inclusive; upper: exclusive

  private:
    static const int _PAGES_MAX_COUNT = 1'000;
    static const int _USERS_MAX_COUNT = 100'000;

    vector<int> _uid_to_pagecount;    // mapping: user_id ==> book_pages_already_read_by_this_user
    vector<int> _pgcount_to_reached;  // mapping: page_count ==> number_of_users_who_reached_this_page
};
// // ==================================================================================

ReadingManager::ReadingManager()
        : _uid_to_pagecount(_USERS_MAX_COUNT + 1, -1),  // '-1' indicates that no 'READ' query for user_id occured
          _pgcount_to_reached(_PAGES_MAX_COUNT + 1, 0) {}

void ReadingManager::Read(int uid, int page_count) {
    UpdatePageRange(_uid_to_pagecount[uid] + 1, page_count + 1);
    _uid_to_pagecount[uid] = page_count;
}

double ReadingManager::Cheer(int uid) const {
    const int pages_count = _uid_to_pagecount[uid];
    if (pages_count == -1) {
        return 0.0;
    }

    const int users_count = GetUserCount();
    if (users_count == 1) {
        return 1.0;
    }

    return ((users_count - _pgcount_to_reached[pages_count]) * 1.0 ) / (users_count - 1);
}


int ReadingManager::GetUserCount() const {
    return _pgcount_to_reached[0];
}

// lower: inclusive; upper: exclusive
void ReadingManager::UpdatePageRange(int lower, int upper) {
    for (int i = lower; i < upper; ++i) {
        ++_pgcount_to_reached[i];
    }
}


int main() {
    // Для ускорения чтения данных отключается синхронизация cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}