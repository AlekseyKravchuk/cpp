#pragma once

namespace RAII {

    // ====================================== Interface ======================================
    template<typename Provider>
    class Booking {
      public:
        Booking(Provider* provider, int id);

        Booking(const Booking&) = delete;
        Booking& operator=(const Booking& other) = delete;

        Booking(Booking&& other);
        Booking& operator=(Booking&& other);

        ~Booking();

      private:
        Provider* _provider;
        int _id;
    };

    // =================================== Implementation ===================================

    template<typename Provider>
    Booking<Provider>::Booking(Provider* provider, int id)
            : _provider(provider),
              _id(id) {}

    template<typename Provider>
    Booking<Provider>::Booking(Booking&& other) {
        _id = other._id;
        _provider = other._provider;

        other._provider = nullptr;
    }

//    Booking& operator=(Booking&& other);
    template<typename Provider>
    Booking<Provider>& Booking<Provider>::operator=(Booking<Provider>&& other) {
        _id = other._id;
        _provider = other._provider;

        other._provider = nullptr;
        return *this;
    }

    template<typename Provider>
    Booking<Provider>::~Booking() {
        if (_provider) {
            _provider->CancelOrComplete(*this);
        }
    }
}