#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <limits>

#include "Router.hpp"

inline constexpr char DEFAULT_NETWORK_PATH[] = "./network_data.txt";

class Network {
  private:
    std::vector<Router> routers;
    std::vector<std::vector<unsigned int>> adjacencyMatrix;

    std::vector<std::string> indexToName;
    std::unordered_map<std::string, size_t> nameToIndex;

    void syncRouters();

    size_t getIndex(const std::string& name) const {
      auto it = nameToIndex.find(name);
      return (it != nameToIndex.end()) ? it->second : std::numeric_limits<size_t>::max();
    }

  public:
    Network() = default;

    explicit Network(std::string_view filepath) {
      loadNetwork(filepath);
    }

    //Manipulacion de topologia
    void updateLink(const std::string& source, const std::string& destination, unsigned int cost);

    Network& operator +=(const std::string& routerName);

    Network& operator -=(const std::string& routerName);

    //consulta y visualizacion
    const RouteInfo* getRoute(const std::string& source, const std::string& destination) const;
    inline const std::string& getRouterName(size_t index) const {
      return indexToName[index];
    }
    void printRoute(const std::string& source, const std::string& destination) const;

    void printRouters() const;

    void printRouterTable(const std::string& routerName) const;

    //manejo de fronteras
    bool loadNetwork(std::string_view filepath = DEFAULT_NETWORK_PATH);

    bool saveNetwork(std::string_view filepath = DEFAULT_NETWORK_PATH) const;

    void generateRandNetwork();
};
