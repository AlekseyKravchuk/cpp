#include <cstdint>  // uint64_t
#include <queue>

/*
Рассмотрим понятие аммортизированной константы на следующем примере.
Путь к нам приходят события с некоторыми временными метками, пусть мы их получаем в порядке возрастания этих временных меток.
Нужно уметь добавлять очередное событие с данной временной меткой и находить в какой-то конкретный момент, сколько событий
произошло за последние 5 минут.
*/

class EventManager {
   public:
    // Добавляет событие
    void Add(uint64_t time) {  // amortized O(1)
        DeleteEventsOlderThanFiveMinutes(time);
        _events.push(time);  // O(1)
    }

    // Возвращает количество событий, произошедших за последние 5 минут
    int Count(uint64_t time) {  // amortized O(1)
        DeleteEventsOlderThanFiveMinutes(time);
        return _events.size();  // O(1)
    }

   private:
    std::queue<uint64_t> _events;

    // принимает временную метку и удаляет все события, которые старше 5 минут (300 секунд) относительно этой метки
    void DeleteEventsOlderThanFiveMinutes(uint64_t time) {           // amortized O(1)
        while (!_events.empty() && _events.front() <= time - 300) {  // amortized O(1)
            _events.pop();                                           // O(1)
        }
    }
};

int main() {
    return 0;
}
