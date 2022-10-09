// usefull links, describing the topi:
// https://www.youtube.com/watch?v=rFuQCd4RvI0
// https://www.youtube.com/watch?v=w6wj8OFqQEU
// https://www.youtube.com/watch?v=HsEqxOvL0mk


#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class Tower {
public:
    // конструктор и метод SetDisks нужны, чтобы правильно создать башни
    Tower(int numberOfDisks) {
        FillTower(numberOfDisks);
    }

    int GetDisksNum() const {
        return _disksInTower.size();
    }

    void SetDisks(int numberOfDisks) {
        FillTower(numberOfDisks);
    }

    // добавляем диск на верх собственной башни
    void AddToTop(int diskSize) {
        int topDiskIndex = _disksInTower.size() - 1;
        if (_disksInTower.size() != 0 && diskSize >= _disksInTower[topDiskIndex]) {
            throw invalid_argument("Невозможно поместить большой диск на маленький");
        } else {
            _disksInTower.push_back(diskSize);
        }
    }

    int RemoveFromTop() {
        if (!_disksInTower.empty()) {
            int diskSize = _disksInTower.back();
            _disksInTower.pop_back();

            return diskSize;
        } else {
            return 0;
        }
    }

    void Resize(int newSize) {
        _disksInTower.resize(newSize);
    }

private:
    vector<int> _disksInTower;

    // используем приватный метод FillTower, чтобы избежать дубликации кода
    void FillTower(int numberOfDisks) {
        for (int i = numberOfDisks; i > 0; i--) {
            _disksInTower.push_back(i);
        }
    }
};

void Hanoi(vector<Tower>& towers, int numDisks2Move2TempTower, int sourceIndex, int targetIndex) {
    if (numDisks2Move2TempTower == 1) {
        // move one top disk from tower[sourceIndex] to tower[targetIndex]
        int diskSize = towers[sourceIndex].RemoveFromTop();
        towers[targetIndex].AddToTop(diskSize);
    } else {
        // вычисляем индекс временной башни, для случая 3-х башен возвожные индексы: 0, 1, 2; 0+1+2 = 3
        int tmpIndex = 3 - sourceIndex - targetIndex;

        // теперь нужно переложить пирамидку высоты (towerSize-1) с башни с индексом "sourceIndex" на башню с индексом "tmp"
        Hanoi(towers, numDisks2Move2TempTower-1, sourceIndex, tmpIndex);

        // теперь, после того как верхние (n-1) дисков переложены на временную башню, нужно переложить оставшийся нижний диск на временную башню "tmp"
        Hanoi(towers, 1, sourceIndex, targetIndex);

        // теперь нужно переложить эту пирамидку с временного штыря на конечный
        Hanoi(towers, numDisks2Move2TempTower-1, tmpIndex, targetIndex);
    }
}

void SolveHanoi(vector<Tower>& towers) {
    int numberOfDisks = towers[0].GetDisksNum();
    int sourceIndex = 0; // leftmost tower is source tower
    int targetIndex = 2; // rightmost tower is target tower

    Hanoi(towers, numberOfDisks, sourceIndex, targetIndex);
}

int main() {
    int towersNum = 3;
    int diskNum = 6;

    vector<Tower> towers;

    // добавим в вектор три пустые башни
    for (int i = 0; i < towersNum; ++i) {
        towers.push_back(0);
    }
    // добавим на первую башню три кольца
    towers[0].SetDisks(diskNum);
    SolveHanoi(towers);

    for (int i = 0; i < towers.size(); ++i) {
        cout << "tower #" << i+1 << " contains " << towers[i].GetDisksNum() << " disks." << endl;
    }
}