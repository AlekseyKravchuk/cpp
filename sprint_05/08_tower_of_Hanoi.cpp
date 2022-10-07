// usefull links:
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
        return _disksStorage.size();
    }

    void SetDisks(int numberOfDisks) {
        FillTower(numberOfDisks);
    }

    // добавляем диск на верх собственной башни
    // обратите внимание на исключение, которое выбрасывается этим методом
    void AddToTop(int disk) {
        int topDiskIndex = _disksStorage.size() - 1;
        if (_disksStorage.size() != 0 && disk >= _disksStorage[topDiskIndex]) {
            throw invalid_argument("Невозможно поместить большой диск на маленький");
        } else {
            // допишите этот метод и используйте его в вашем решении
        }
    }

    void RemoveTopDisk() {
        if (!_disksStorage.empty()) {
            _disksStorage.pop_back();
        }
    }

   private:
    vector<int> _disksStorage;

    // используем приватный метод FillTower, чтобы избежать дубликации кода
    void FillTower(int numberOfDisks) {
        for (int i = numberOfDisks; i > 0; i--) {
            _disksStorage.push_back(i);
        }
    }
};

void Hanoi(int towerSize, int sourceIndex, int targetIndex) {
    if (towerSize == 1) {
        // move disk 1 from tower 0 to tower with "targetIndex"
    } else {
        // вычисляем индекс временной башни
        int tmp = 3 - sourceIndex - targetIndex;

        // теперь нужно переложить пирамидку высоты (towerSize-1) с башни с индексом "sourceIndex" на башню с индексом "tmp"
        Hanoi(towerSize-1, sourceIndex, tmp);

        // теперь нужно переложить эту пирамидку с временного штыря на конечный
        Hanoi(towerSize-1, tmp, targetIndex);

    }
}

void SolveHanoi(vector<Tower>& towers) {
    int numberOfDisks = towers[0].GetDisksNum();

    if (numberOfDisks == 1) {
    }

    // допишите функцию, чтобы на towers[0] было 0 дисков,
    // на towers[1] 0 дисков,
    // и на towers[2] было disks_num дисков
}

int main() {
    int towersNum = 3;
    int diskNum = 3;

    vector<Tower> towers;

    // добавим в вектор три пустые башни
    for (int i = 0; i < towersNum; ++i) {
        towers.push_back(0);
    }
    // добавим на первую башню три кольца
    towers[0].SetDisks(diskNum);
    SolveHanoi(towers);
}