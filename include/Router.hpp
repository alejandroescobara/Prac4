#pragma once

#include <cstddef>
#include <vector>
#include <map>

struct RouteInfo {
  size_t cost;
  std::vector<size_t> path;
};

class Router {
  private:
    size_t id;
    std::map <size_t, RouteInfo> routeTable;

  public:
    explicit Router(size_t givenId) : id(givenId) {}
    
    size_t getId() const {
      return id;
    }

    void setId(size_t newId) {
      id = newId;
    }

    const std::map<size_t, RouteInfo>& getRouteTable() const {
      return routeTable;
    }

    const RouteInfo* getRoute(size_t destination) const{
      auto it = routeTable.find(destination);
      return (it != routeTable.end()) ? &(it->second) : nullptr;
    }
    
    void updateTable(const std::vector<std::vector<unsigned int>>& adjacencyMatrix);
};
