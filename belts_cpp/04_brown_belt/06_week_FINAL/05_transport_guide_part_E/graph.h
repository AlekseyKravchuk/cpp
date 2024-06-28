#pragma once

#include <cstdlib>
#include <deque>
#include <vector>

template<typename It>
class Range {
  public:
    using ValueType = typename std::iterator_traits<It>::value_type;

    Range(It begin, It end) : _begin(begin), _end(end) {}

    It begin() const { return _begin; }

    It end() const { return _end; }

  private:
    It _begin;
    It _end;
};

namespace Graph {

    using VertexId = size_t;
    using EdgeId = size_t;

    template<typename Weight>
    struct Edge {
        VertexId from;
        VertexId to;
        Weight weight;
    };

    template<typename Weight>
    class DirectedWeightedGraph {
      private:
        using IncidenceList = std::vector<EdgeId>;
        using IncidentEdgesRange = Range<typename IncidenceList::const_iterator>;

      public:
        explicit DirectedWeightedGraph(size_t vertex_count);
        EdgeId AddEdge(const Edge<Weight>& edge);

        size_t GetVertexCount() const;
        size_t GetEdgeCount() const;
        const Edge<Weight>& GetEdge(EdgeId edge_id) const;
        IncidentEdgesRange GetIncidentEdges(VertexId vertex) const;

      private:
        std::vector<Edge<Weight>> _edges;
        std::vector<IncidenceList> _incidence_lists;
    };

    template<typename Weight>
    DirectedWeightedGraph<Weight>::DirectedWeightedGraph(size_t vertex_count)
            : _incidence_lists(vertex_count) {}

    template<typename Weight>
    EdgeId DirectedWeightedGraph<Weight>::AddEdge(const Edge<Weight>& edge) {
        _edges.push_back(edge);
        const EdgeId id = _edges.size() - 1;
        _incidence_lists[edge.from].push_back(id);
        return id;
    }

    template<typename Weight>
    size_t DirectedWeightedGraph<Weight>::GetVertexCount() const {
        return _incidence_lists.size();
    }

    template<typename Weight>
    size_t DirectedWeightedGraph<Weight>::GetEdgeCount() const {
        return _edges.size();
    }

    template<typename Weight>
    const Edge<Weight>& DirectedWeightedGraph<Weight>::GetEdge(EdgeId edge_id) const {
        return _edges[edge_id];
    }

    template<typename Weight>
    typename DirectedWeightedGraph<Weight>::IncidentEdgesRange
    DirectedWeightedGraph<Weight>::GetIncidentEdges(VertexId vertex) const {
        const auto& edges = _incidence_lists[vertex];
        return {std::begin(edges), std::end(edges)};
    }
}
