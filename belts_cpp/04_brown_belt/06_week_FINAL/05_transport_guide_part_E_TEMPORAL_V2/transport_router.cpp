#include "transport_router.h"

namespace routing {

TransportRouter::TransportRouter(const catalogue::TransportCatalogue& catalogue, Settings settings)
    : _catalogue(catalogue), _settings(settings) {
    BuildVerticesForStops(catalogue.GetUniqueStops());
    BuildRoutesGraph(catalogue.GetBuses());

    _router = std::make_unique<Router>(*_routes);
}

void TransportRouter::BuildVerticesForStops(const std::set<std::string_view>& stops) {
    graph::VertexId start{0};
    graph::VertexId end{1};

    _stop_to_vertex.reserve(stops.size());

    for (std::string_view stop : stops) {
        _stop_to_vertex.emplace(stop, StopVertices{start, end});

        // Stops have 'begin' and 'end' so the ids difference is count('begin', 'end') = 2
        start += 2;
        end += 2;
    }
}

void TransportRouter::AddBusRouteEdges(const catalogue::Bus& bus) {
    const auto& distances = _catalogue.GetAllDistancesOnTheRoute(bus.number, _settings.bus_velocity_);

    graph::VertexId from{0};
    graph::VertexId to{0};

    for (const auto& [route, info] : distances) {
        from = _stop_to_vertex[route.first].end;
        to = _stop_to_vertex[route.second].start;

        auto edge = graph::Edge<Weight>{from, to, info.time};

        _routes->AddEdge(edge);
        _edge_to_response.emplace(edge, BusResponse(info.time, bus.number, info.stops_count));
    }
}

void TransportRouter::BuildRoutesGraph(const std::deque<catalogue::Bus>& buses) {
    _routes = std::make_unique<Graph>(_stop_to_vertex.size() * 2);

    // Step 1. Create "wait"-type edges for each stop
    auto wait_time = static_cast<double>(_settings.bus_wait_time_);
    auto stop_edge = graph::Edge<Weight>{};

    for (auto [stop_name, stop_vertices] : _stop_to_vertex) {
        stop_edge = graph::Edge<Weight>{stop_vertices.start, stop_vertices.end, wait_time};

        _routes->AddEdge(stop_edge);
        _edge_to_response.emplace(stop_edge, WaitResponse(wait_time, stop_name));
    }

    // Step 2. Add "bus"-type edges for each stop in bus route
    for (const auto& bus : buses)
        AddBusRouteEdges(bus);
}

ResponseDataOpt TransportRouter::BuildRoute(std::string_view from, std::string_view to) const {
    ResponseDataOpt response{std::nullopt};

    graph::VertexId id_from = _stop_to_vertex.at(from).start;
    graph::VertexId id_to = _stop_to_vertex.at(to).start;

    if (auto route = _router->BuildRoute(id_from, id_to)) {
        response.emplace(ResponseData{});
        response->total_time = route->weight;

        for (auto edge_id : route->edges) {
            graph::Edge<Weight> edge = _routes->GetEdge(edge_id);

            response->items.emplace_back(_edge_to_response.at(edge));
        }
    }

    return response;
}

}  // namespace routing