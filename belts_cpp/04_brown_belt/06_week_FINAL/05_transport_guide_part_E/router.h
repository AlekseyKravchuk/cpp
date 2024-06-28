#pragma once

#include "graph.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Graph {

    template<typename Weight>
    class Router {
      private:
        using Graph = DirectedWeightedGraph<Weight>;

      public:
        explicit Router(const Graph& graph);

        using RouteId = uint64_t;
        struct RouteInfo {
            RouteId id;
            Weight weight;
            size_t edge_count;
        };

        std::optional<RouteInfo> BuildRoute(VertexId from, VertexId to) const;
        EdgeId GetRouteEdge(RouteId route_id, size_t edge_idx) const;
        void ReleaseRoute(RouteId route_id);

      private:
        const Graph& _graph;

        struct RouteInternalData {
            Weight weight;
            std::optional<EdgeId> prev_edge;
        };

        using RoutesInternalData = std::vector<std::vector<std::optional<RouteInternalData>>>;
        using ExpandedRoute = std::vector<EdgeId>;

        mutable RouteId _next_route_id = 0;
        mutable std::unordered_map<RouteId, ExpandedRoute> _expanded_routes_cache;

        void InitializeRoutesInternalData(const Graph& graph) {
            const size_t vertex_count = graph.GetVertexCount();
            for (VertexId vertex = 0; vertex < vertex_count; ++vertex) {
                _routes_internal_data[vertex][vertex] = RouteInternalData{0, std::nullopt};
                for (const EdgeId edge_id: graph.GetIncidentEdges(vertex)) {
                    const auto& edge = graph.GetEdge(edge_id);
                    assert(edge.weight >= 0);
                    auto& route_internal_data = _routes_internal_data[vertex][edge.to];
                    if (!route_internal_data || route_internal_data->weight > edge.weight) {
                        route_internal_data = RouteInternalData{edge.weight, edge_id};
                    }
                }
            }
        }

        void RelaxRoute(VertexId vertex_from,
                        VertexId vertex_to,
                        const RouteInternalData& route_from,
                        const RouteInternalData& route_to) {
            auto& route_relaxing = _routes_internal_data[vertex_from][vertex_to];
            const Weight candidate_weight = route_from.weight + route_to.weight;
            if (!route_relaxing || candidate_weight < route_relaxing->weight) {
                route_relaxing = {
                        candidate_weight,
                        route_to.prev_edge
                        ? route_to.prev_edge
                        : route_from.prev_edge
                };
            }
        }

        void RelaxRoutesInternalDataThroughVertex(size_t vertex_count, VertexId vertex_through) {
            for (VertexId vertex_from = 0; vertex_from < vertex_count; ++vertex_from) {
                if (const auto& route_from = _routes_internal_data[vertex_from][vertex_through]) {
                    for (VertexId vertex_to = 0; vertex_to < vertex_count; ++vertex_to) {
                        if (const auto& route_to = _routes_internal_data[vertex_through][vertex_to]) {
                            RelaxRoute(vertex_from, vertex_to, *route_from, *route_to);
                        }
                    }
                }
            }
        }

        RoutesInternalData _routes_internal_data;  // RoutesInternalData = vector<vector<optional<RouteInternalData>>>;
    };  // END of class Router DECLARATION
// ====================================================================================================================

    template<typename Weight>
    Router<Weight>::Router(const Graph& graph)
            : _graph(graph),
              _routes_internal_data(graph.GetVertexCount(),
                                    std::vector<std::optional<RouteInternalData>>(graph.GetVertexCount())) {
        InitializeRoutesInternalData(graph);

        const size_t vertex_count = graph.GetVertexCount();
        for (VertexId vertex_through = 0; vertex_through < vertex_count; ++vertex_through) {
            RelaxRoutesInternalDataThroughVertex(vertex_count, vertex_through);
        }
    }

    template<typename Weight>
    std::optional<typename Router<Weight>::RouteInfo> Router<Weight>::BuildRoute(VertexId from, VertexId to) const {
        const auto& route_internal_data = _routes_internal_data[from][to];
        if (!route_internal_data) {
            return std::nullopt;
        }
        const Weight weight = route_internal_data->weight;
        std::vector<EdgeId> edges;
        for (std::optional<EdgeId> edge_id = route_internal_data->prev_edge;
             edge_id;
             edge_id = _routes_internal_data[from][_graph.GetEdge(*edge_id).from]->prev_edge) {
            edges.push_back(*edge_id);
        }
        std::reverse(std::begin(edges), std::end(edges));

        const RouteId route_id = _next_route_id++;
        const size_t route_edge_count = edges.size();
        _expanded_routes_cache[route_id] = std::move(edges);
        return RouteInfo{route_id, weight, route_edge_count};
    }

    template<typename Weight>
    EdgeId Router<Weight>::GetRouteEdge(RouteId route_id, size_t edge_idx) const {
        return _expanded_routes_cache.at(route_id)[edge_idx];
    }

    template<typename Weight>
    void Router<Weight>::ReleaseRoute(RouteId route_id) {
        _expanded_routes_cache.erase(route_id);
    }
}  // END of namespace Graph
