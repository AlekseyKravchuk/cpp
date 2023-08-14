#pragma once

#include <array>
#include <map>
#include <string_view>

#include "http_request.h"

class StatPiece {
   public:
    template <typename Container>
    StatPiece(const Container& known_keys, std::string_view default_key)
        : _default_key(default_key) {
        _counts[default_key] = 0;
        
        for (std::string_view key : known_keys) {
            _counts[key] = 0;
        }
    }

    void Add(std::string_view value);

    const std::map<std::string_view, int>& GetValues() const {
        return _counts;
    }

   private:
    const std::string_view _default_key;
    std::map<std::string_view, int> _counts;
};

class Stats {
   public:
    Stats() = default;

    void AddMethod(std::string_view method);
    void AddUri(std::string_view uri);
    const std::map<std::string_view, int>& GetMethodStats() const;
    const std::map<std::string_view, int>& GetUriStats() const;

   private:
    // Ключевое слово inline позволяет определить статические члены known_methods, default_method и т.д. здесь, в .h-файле. Без
    // него нам бы пришлось объявить их здесь, а определеление вынести в stats.cpp
    inline static const std::array<std::string, 4> _known_methods = {"GET", "POST", "DELETE", "PUT"};
    inline static const std::string _default_method = "UNKNOWN";

    inline static const std::array<std::string, 5> _known_uris = {"/", "/product", "/basket", "/help", "/order"};
    inline static const std::string _default_uri = "unknown";

    StatPiece _methods{_known_methods, _default_method};
    StatPiece _uris{_known_uris, _default_uri};
};

HttpRequest ParseRequest(std::string_view line);