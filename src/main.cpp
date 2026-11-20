
#include <iostream>

#include "lib.hpp"

int main() {
  Network net;
  size_t opt = 0;

  while (true) {
    clear_console();
    printMenu();

    if (invalid_input(opt)) continue;
    

    if (opt == 4) break;

    switch (opt) {
      case 1: { // Cargar desde archivo
        clear_console();
        std::string path;
        std::cout << "Ingrese el nombre/ruta del archivo (vacio para default): ";
        std::cin.ignore();
        std::getline(std::cin, path);

        bool loaded = path.empty() ? net.loadNetwork() : net.loadNetwork(path);
        if (loaded) {
          std::cout << "\n[OK] Red cargada y sincronizada correctamente.\n";
          wait_enter();
          networkSession(net);
        } else {
          std::cerr << "\n[!] No se pudo abrir el archivo especificado.\n";
          wait_enter();
        }
        break;
      }

      case 2: { // Red virgen / vacia
        clear_console();
        net = Network{}; // Reinicia a una red limpia
        std::cout << "[OK] Red virgen inicializada (sin routers ni enlaces).\n";
        wait_enter();
        networkSession(net);
        break;
      }

      case 3: { // Generar aleatoria
        clear_console();
        std::cout << "Generando topologia aleatoria conexa...\n";
        net.generateRandNetwork();
        std::cout << "[OK] Red aleatoria generada y cargada.\n";
        wait_enter();
        networkSession(net);
        break;
      }

      default:
        std::cerr << "\n[!] Opcion fuera de rango (1-4).\n";
        wait_enter();
        break;
    }
  }

  return 0;
}
