#pragma once

#include <new>  // Для исключения bad_alloc

#include "ptrvector.h"

// Щупальце
class Tentacle {
   public:
    explicit Tentacle(int id) noexcept : _id(id) {}

    int GetId() const noexcept { return _id; }

    Tentacle* GetLinkedTentacle() const noexcept {
        return _linkedTentacle;
    }

    void LinkTo(Tentacle& tentacle) noexcept {
        _linkedTentacle = &tentacle;
    }

    void Unlink() noexcept {
        _linkedTentacle = nullptr;
    }

   private:
    int _id = 0;
    Tentacle* _linkedTentacle = nullptr;
};

// Осьминог
class Octopus {
   public:
    Octopus() : Octopus(8) {}

    explicit Octopus(int numTentacles) {
        Tentacle* tentacle = nullptr;
        try {
            for (int i = 1; i <= numTentacles; ++i) {
                tentacle = new Tentacle(i);                 // Может выбросить исключение bad_alloc
                _tentacles.GetItems().push_back(tentacle);  // Может выбросить исключение bad_alloc

                // Обнуляем указатель на щупальце, которое уже добавили в _tentacles,
                // чтобы не удалить его в обработчике catch повторно
                tentacle = nullptr;
            }
        } catch (const std::bad_alloc&) {
            // Удаляем щупальца, которые успели попасть в контейнер _tentacles
            Cleanup();
            // Удаляем щупальце, которое создали, но не добавили в _tentacles
            delete tentacle;
            // Конструктор не смог создать осьминога с восемью щупальцами,
            // поэтому выбрасываем исключение, чтобы сообщить вызывающему коду об ошибке
            // throw без параметров внутри catch выполняет ПЕРЕВЫБРОС пойманного исключения
            throw;
        }
    }
    

    ~Octopus() {
        // Осьминог владеет объектами в динамической памяти (щупальца),
        // которые должны быть удалены при его разрушении.
        // Деструктор - лучшее место, чтобы прибраться за собой.
        Cleanup();
    }

    // Добавляет новое щупальце с идентификатором,
    // равным (количество_щупалец + 1):
    // 1, 2, 3, ...
    // Возвращает ссылку на добавленное щупальце
    Tentacle& AddTentacle() {
        auto lastTentacle = _tentacles.GetItems().back();
        int lastTentacleID = lastTentacle->GetId();
        Tentacle* tentacle = new Tentacle(lastTentacleID + 1);  // Может выбросить исключение bad_alloc
        _tentacles.GetItems().push_back(tentacle);              // Может выбросить исключение bad_alloc

        return *tentacle;
    }

    int GetTentacleCount() const noexcept {
        return static_cast<int>(_tentacles.GetItems().size());
    }

    const Tentacle& GetTentacle(size_t index) const {
        return *_tentacles.GetItems().at(index);
    }
    Tentacle& GetTentacle(size_t index) {
        return *_tentacles.GetItems().at(index);
    }

   private:
    void Cleanup() {
        // Удаляем щупальца осьминога из динамической памяти
        for (Tentacle* t : _tentacles.GetItems()) {
            delete t;
        }
        // Очищаем массив указателей на щупальца
        _tentacles.GetItems().clear();
    }

    // Вектор хранит указатели на щупальца. Сами объекты щупалец находятся в куче
    PtrVector<Tentacle> _tentacles;
};