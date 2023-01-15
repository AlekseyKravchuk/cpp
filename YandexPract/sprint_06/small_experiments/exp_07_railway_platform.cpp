#include <algorithm>
#include <deque>
#include <iostream>
#include <string>

using namespace std;

class TrainTerminal {
   public:
    void OnPassengerArrive(const string& name) {
        passengers_on_platform_.push_back(name);
    }

    void OnTrainArrive() {
        cout << "Список пассажиров поезда:"s << endl;
        int id = 1;
        while (!passengers_on_platform_.empty()) {
            cout << (id++) << passengers_on_platform_.front() << endl;
            passengers_on_platform_.pop_front();
        }
    }

    bool FindPassenger(const string& name) const {
        auto iter = std::find(passengers_on_platform_.begin(), passengers_on_platform_.end(), name);
        return iter != passengers_on_platform_.end();
    }

   private:
    deque<std::string> passengers_on_platform_;
};

int main(int argc, char const* argv[]) {
    int numOfPassengers = 100000;
    int numOfTrains = numOfPassengers / 100;
    return 0;
}
