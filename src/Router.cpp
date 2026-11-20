#include "Router.hpp"

#include <queue>
#include <limits>

void Router::updateTable(const std::vector<std::vector<unsigned int>>& adjacencyMatrix) {
  routeTable.clear();
  
  size_t totalNodes = adjacencyMatrix.size();
  if (totalNodes == 0 || id >= totalNodes) {
    return;
  }

  const size_t INF = std::numeric_limits<size_t>::max();
  std::vector<size_t> minCosts(totalNodes, INF);
  std::vector<size_t> previous(totalNodes, INF);

  using Pair = std::pair<size_t, size_t>;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;

  minCosts[id] = 0;
  pq.push({0, id});

  while (!pq.empty()) {
    auto [currentCost, u] = pq.top();
    pq.pop();

    if (currentCost > minCosts[u]) continue;
    
    for (size_t v = 0; v < totalNodes; ++v) {
      unsigned int linkCost = adjacencyMatrix[u][v];

      if (linkCost > 0) {
        size_t newCost = minCosts[u] + linkCost;
        if (newCost < minCosts[v]) {
          minCosts[v] = newCost;
          previous[v] = u;
          pq.push({newCost, v});
        }
      }
    }
  }

  for (size_t destination = 0; destination < totalNodes; ++destination) {
    if (minCosts[destination] == INF) continue;

    std::vector<size_t> path;
    for (size_t at = destination; at != INF; at = previous[at]) {
      path.insert(path.begin(), at);
      if (at == id) break;
    }
    routeTable[destination] = {minCosts[destination], path};
  }
}
