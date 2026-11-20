#include "Network.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

void Network::syncRouters() {
  for (auto& router : routers) {
    router.updateTable(adjacencyMatrix);
  }
}

void Network::updateLink(const std::string& source, const std::string& destination, unsigned int cost) {
  size_t u = getIndex(source);
  size_t v = getIndex(destination);

  if (u == std::numeric_limits<size_t>::max() || v == std::numeric_limits<size_t>::max()) {
    return;
  }

  if (u == v) return;

  if (adjacencyMatrix[u][v] == cost) return;

  adjacencyMatrix[u][v] = cost;
  adjacencyMatrix[v][u] = cost;

  syncRouters();
}

Network& Network::operator+=(const std::string& routerName) {
  if (nameToIndex.find(routerName) != nameToIndex.end()) {
    return *this;
  }

  size_t newIndex = indexToName.size();

  indexToName.push_back(routerName);
  nameToIndex[routerName] = newIndex;

  routers.emplace_back(newIndex);

  for (auto& row : adjacencyMatrix) {
    row.push_back(0);
  }

  adjacencyMatrix.emplace_back(newIndex+1, 0);

  return *this;
}


Network& Network::operator-=(const std::string& routerName) {
  size_t targetIdx = getIndex(routerName);

  if (targetIdx == std::numeric_limits<size_t>::max()) {
    return *this;
  }

  adjacencyMatrix.erase(adjacencyMatrix.begin() + targetIdx);

  for (auto& row : adjacencyMatrix) {
    row.erase(row.begin() + targetIdx);
  }

  routers.erase(routers.begin() + targetIdx);
  indexToName.erase(indexToName.begin() + targetIdx);

  nameToIndex.clear();
  for (size_t i = 0; i < indexToName.size(); ++i) {
    nameToIndex[indexToName[i]] = i;
    routers[i].setId(i);
  }

  syncRouters();

  return *this;
}

const RouteInfo* Network::getRoute(const std::string& source, const std::string& destination) const {
  size_t u = getIndex(source);
  size_t v = getIndex(destination);


  if (u == std::numeric_limits<size_t>::max() || v == std::numeric_limits<size_t>::max()) {
    return nullptr;
  }

  return routers[u].getRoute(v);
}

void Network::printRoute(const std::string& source, const std::string& destination) const {
  const RouteInfo* route = getRoute(source, destination);

  if (!route) {
    std::cout << "No existe conexion entre " << source << " y " << destination << "\n\n";
    return;
  }

  std::cout << "| " << source << " -> " << destination << " |\n" <<
               "Ruta optima: ";
  for (size_t i = 0; i < route->path.size(); ++i) {
    std::cout << indexToName[route->path[i]];
    if (i+1 < route->path.size()) {
      std::cout << " -> ";
    }
  }
  std::cout << " [ Costo: " << route->cost << " ]\n";
}

void Network::printRouters() const {
  std::cout << "\n";
  if (routers.empty()) {
    std::cout << "La red esta vacía\n\n";
    return;
  }
  std::cout << "Routers disponibles ( " << routers.size() << "en total):\n\n";
  for (size_t i = 0; i < indexToName.size(); ++i) {
    std::cout << "\n" << i+1 << ". " << indexToName[i];
  }
  std::cout << "\n";
}

void Network::printRouterTable(const std::string& routerName) const {
  size_t u = getIndex(routerName);
  if (u == std::numeric_limits<size_t>::max()) {
    std::cout << "Router " << routerName << " no encontrado\n";
    return;
  }

  std::cout << "\n| TABLA DE ENRUTAMIENTO " << routerName << " |\n";
  const auto& table = routers[u].getRouteTable();
  for (size_t j = 0; j < routers.size(); ++j) {
    if (u == j) continue;

    const std::string& otherRouterName = indexToName[j];
    auto it = table.find(j);

    if (it != table.end()) {
      std::cout << routerName << "->" << otherRouterName << ":\n"
                << "  Ruta: [ ";
      for (size_t k = 0; k < it->second.path.size(); ++k) {
        std::cout << indexToName[it->second.path[k]];
        if (k + 1 < it->second.path.size()) {
          std::cout << " -> ";
        }
      }
      std::cout << " ]\n" 
                << "  Costo: " << it->second.cost << "\n\n";
    }
    else {
      std::cout << routerName << " -> " << otherRouterName << ":\n"
                << "  Ruta: [ N/A ]\n"
                << "  Costo: INF\n\n";
    }
  }
}

bool Network::loadNetwork(std::string_view filepath) {
  std::ifstream file{std::string{filepath}};
  if (!file.is_open()) {
    return false;
  }

  routers.clear();
  adjacencyMatrix.clear();
  indexToName.clear();
  nameToIndex.clear();

  std::string src, dst;
  unsigned int cost;
  std::string line;

  while (std::getline(file, line)) {
    if (line.empty()) continue;

    std::istringstream iss(line);
    if (iss >> src >> dst >> cost) {
      *this += src;
      *this += dst;
      
      size_t u = getIndex(src);
      size_t v = getIndex(dst);

      if (u == std::numeric_limits<size_t>::max() || v == std::numeric_limits<size_t>::max()) {
        continue;
      }

      if (u == v) continue;

      adjacencyMatrix[u][v] = cost;
      adjacencyMatrix[v][u] = cost;
    }
  }

  syncRouters();
  return true;
}

bool Network::saveNetwork(std::string_view filepath) const {
  std::ofstream file{std::string{filepath}};
  if (!file.is_open()) {
    return false;
  }

  for (size_t i = 0; i < adjacencyMatrix.size(); ++i) {
    for (size_t j = i+1; j < adjacencyMatrix[i].size(); ++j) {
      unsigned int cost = adjacencyMatrix[i][j];
      if (cost > 0) {
        file << indexToName[i] << " " << indexToName[j] << " " << cost << "\n";
      }
    }
  }
  return true;
}

void Network::generateRandNetwork() {
  constexpr size_t MIN_NODES = 4;
  constexpr size_t MAX_NODES = 8;
  constexpr unsigned int MIN_COST = 1;
  constexpr unsigned int MAX_COST = 20;
  constexpr double PROBABILIDAD_ENLACE = 0.45; // 45% de probabilidad de tener conexión directa

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> distNodos(MIN_NODES, MAX_NODES);
  std::uniform_int_distribution<unsigned int> distCosto(MIN_COST, MAX_COST);
  std::bernoulli_distribution distMoneda(PROBABILIDAD_ENLACE);

  size_t totalNodos = distNodos(gen);

  std::ofstream outFile{std::string{DEFAULT_NETWORK_PATH}};
  if (!outFile.is_open()) {
    std::cerr << "Error: No se pudo crear el archivo para la red aleatoria.\n";
    return;
  }

  for (size_t i = 1; i < totalNodos; ++i) {
    std::uniform_int_distribution<size_t> distPrevio(0, i - 1);
    size_t vecino = distPrevio(gen);

    char src = static_cast<char>('A' + vecino);
    char dst = static_cast<char>('A' + i);
    outFile << src << " " << dst << " " << distCosto(gen) << "\n";
  }

  for (size_t i = 0; i < totalNodos; ++i) {
    for (size_t j = i + 1; j < totalNodos; ++j) {
      if (distMoneda(gen)) {
        char src = static_cast<char>('A' + i);
        char dst = static_cast<char>('A' + j);
        outFile << src << " " << dst << " " << distCosto(gen) << "\n";
      }
    }
  }

  outFile.close();

  loadNetwork(DEFAULT_NETWORK_PATH);
}
