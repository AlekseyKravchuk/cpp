// ******************************** Задание 2 ********************************
// Для съёмок блокбастера «Атака октоклонов» нужно изготовить 1000 копий осьминогов на основе имеющегося осьминога-прототипа.
// В классе Octopus не задан пользовательский конструктор копирования. Копирующий конструктор, сгенерированный компилятором,
// копирует осьминогов некорректно: несколько осьминогов используют щупальца прототипа. Это приводит к неопределённому поведению.

// Реализуйте следующий функционал:

//     - Корректное клонирование осьминогов, при котором копия будет иметь свой набор щупалец, а не пользоваться щупальцами оригинала.
//       Щупальца копии осьминога должны прицепляться к тем же щупальцам, что и оригинал.
//     - Метод AddTentacle для добавления новых щупалец. Добавление должно сохранять адрес размещения существующих щупалец в памяти.
//       id щупальца должен быть равен текущему количеству щупалец, увеличенному на 1.

// Проверки, которые есть в функции main, упростят разработку.

// Ограничения
// Гарантируется, что количество щупалец, передаваемое в параметризованный конструктор класса Octopus, неотрицательное.
// Проверять их количество в конструкторе необязательно. Максимальное количество щупалец, которое будет иметь осьминог,
// не превысит несколько десятков.

// ===> ПОДСКАЗКА
// Используйте классы ScopedPtr и PtrVector. С ними класс Octopus станет заметно проще.
// Когда щупальца хранятся внутри PtrVector, создавать собственный
// конструктор копирования и деструктор для класса Octopus не придётся, поскольку
// конструктор копирования PtrVector корректно скопирует щупальца, а деструктор удалит их.


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