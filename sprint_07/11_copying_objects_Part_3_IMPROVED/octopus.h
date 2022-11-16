#pragma once

#include <new>  // Для исключения std::bad_alloc
#include <vector>

#include "ptrvector.h"
#include "scopedptr.h"

// Щупальце
class Tentacle {
   public:
    explicit Tentacle(int id) noexcept : _id(id) {}

    int GetId() const noexcept {
        return _id;
    }

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
        Tentacle* tentaclePtr = nullptr;
        try {
            for (int i = 1; i <= numTentacles; ++i) {
                tentaclePtr = new Tentacle(i);                 // Может выбросить исключение bad_alloc
                _tentacles.GetItems().push_back(tentaclePtr);  // Может выбросить исключение bad_alloc

                // Обнуляем указатель на щупальце, которое уже добавили в tentacles_,
                // чтобы не удалить его в обработчике catch повторно
                tentaclePtr = nullptr;
            }
        } catch (const std::bad_alloc&) {
            // Удаляем щупальца, которые успели попасть в контейнер tentacles_
            Cleanup();
            // Удаляем щупальце, которое создали, но не добавили в tentacles_
            delete tentaclePtr;
            // Конструктор не смог создать осьминога с восемью щупальцами,
            // поэтому выбрасываем исключение, чтобы сообщить вызывающему коду об ошибке
            // throw без параметров внутри catch выполняет ПЕРЕВЫБРОС пойманного исключения
            throw;
        }
    }

    // Когда щупальца хранятся внутри PtrVector, создавать собственный деструктор (как и конструктор копирования) для класса Octopus нет необходимости
    // т.к. конструктор копирования PtrVector корректно скопирует щупальца, а деструктор удалит их.
    /* 
    ~Octopus() {
        // Осьминог владеет объектами в динамической памяти (щупальца),
        // которые должны быть удалены при его разрушении.
        // Деструктор - лучшее место, чтобы прибраться за собой.
        Cleanup();
    }

    */

    // Добавляет новое щупальце с идентификатором, равным (количество_щупалец + 1): 1, 2, 3, ...
    // Возвращает ссылку на добавленное щупальце
    Tentacle& AddTentacle() {
        // если возникнет исключение во время создания шупальца или во время добавления его в _tentacles, то наш ScopedPtr почистит память за собой.
        ScopedPtr<Tentacle> tentacleScopedPtr(new Tentacle(GetTentacleCount() + 1));  // оборачиваем вызов new в ScopedPtr
        
        _tentacles.GetItems().push_back(tentacleScopedPtr.GetRawPtr());               // кладем "сырой" указатель в tentacles_
        return *tentacleScopedPtr.Release();                                          // освобождаем владение над шупальцем в tentacle
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