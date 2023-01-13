#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <utility>

using namespace std::literals;

class Route {
   private:
    std::string _source;
    std::string _destination;
    std::optional<int> _len;

    std::map<std::pair<std::string, std::string>, int> _routes =
        {
            {{"Moscow"s, "Saint Petersburg"s}, 900},
            {{"Saint Petersburg"s, "Pskov"s}, 290},
            {{"Saint Petersburg"s, "Veliky Novgorod"s}, 190},
            {{"Saint Petersburg"s, "Nizhny Novgorod"s}, 1124},
            {{"Saint Petersburg"s, "Vologda"s}, 660}
    };

    std::optional<int> ComputeDistance(const std::string& src, const std::string& dst) {
        if (_routes.count({src, dst})) {
            return _routes.at({src, dst});
        } else if (_routes.count({dst, src})) {
            return _routes.at({dst, src});
        } else {
            return std::nullopt;
        }
    }

    void UpdateLength() {
        _len = ComputeDistance(_source, _destination);
    }

   public:
    Route(std::string source, std::string destination, int lenght)
        : _source(source), _destination(destination), _len(lenght) {}

    std::string GetSource() const { return _source; }
    std::string GetDestination() const { return _destination; }
    std::optional<int> GetLength() const { return _len; }

    void SetSource(std::string source) {
        _source = source;
        UpdateLength();
    }

    void SetDestination(std::string destination) {
        _destination = destination;
        UpdateLength();
    }

    void SetLength(int lenght) { _len = lenght; }
};

std::ostream& operator<<(std::ostream& os, const Route& route) {
    // по константной ссылке "const Route&" можно вызвать ТОЛЬКО КОНСТАНТНЫЙ метод
    os << route.GetSource()
       << " -> "s << route.GetDestination() << ": "s;
    if (route.GetLength().has_value()) {
        os << route.GetLength().value() << " km"s << std::endl;
    } else {
        os << "None"s << std::endl;
    }

    return os;
}

void ReverseRoot(Route route) {
    // по НЕконстантной ссылке мы можем вызвать КОНСТАНТНЫЙ метод "GetSource()"
    std::string bkp_source = route.GetSource();

    // по НЕконстантной ссылке мы также можем вызвать НЕконстантный метод "SetSource()"
    route.SetSource(route.GetDestination());

    // т.е. по НЕконстантной ссылке мы можем вызвать ЛЮБОЙ метод текущего объекта
    route.SetDestination(bkp_source);
}

int main() {
    Route route_1{"Moskow"s, "Saint Petersburg"s, 900};
    std::cout << route_1 << std::endl;

    route_1.SetSource("Saint Petersburg"s);
    route_1.SetDestination("Veliky Novgorod"s);
    std::cout << route_1 << std::endl;

    route_1.SetSource("Vologda"s);
    route_1.SetDestination("Saint Petersburg"s);
    std::cout << route_1 << std::endl;

    return 0;
}
