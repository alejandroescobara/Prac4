#include <iostream>
#include <limits>

#include "lib.hpp"

bool invalid_input(size_t& variable) {
  if (!(std::cin >> variable && std::cin.peek() == '\n')) {
    std::cerr << "\n\nERROR: INVALID INPUT";
    std::cin.clear();
    wait_enter();  
    return true;
  }
  return false;
}

void clear_console() {
  #if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
  #else
    std::system("clear");
  #endif
}


void wait_enter() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cin.get();
}


void printMenu() {
  std::cout << "<| SISTEMA DE GESTION DE RED |>\n\nOPCIONES:\n"
            << "  1. Cargar red desde archivo\n"
            << "  2. Generar red virgen\n"       
            << "  3. Generar red aleatoria\n" 
            << "  4. Salir\n\n"
            << "Escribe tu eleccion (1-4): ";
}


void printSubMenu() {
  std::cout << "\n<| OPERACIONES DE RED |>\n\n"
            << "  1. Consultar ruta optima\n"
            << "  2. Ver tabla de enrutamiento de un router\n"
            << "  3. Ver routers disponibles\n"
            << "  4. Gestionar enlace\n"
            << "  5. Agregar un router\n"
            << "  6. Eliminar un router\n"
            << "  7. Guardar cambios\n"
            << "  8. Volver al menu de inicio\n\n"
            << "Escribe tu eleccion (1-8): ";
}

void networkSession(Network& net) {
  size_t opt = 0;

  while (true) {
    clear_console();
    printSubMenu();

    if (invalid_input(opt)) continue;

    if (opt == 8) break;

    switch (opt) {
      case 1: { // Consultar costo y camino
        clear_console();
        net.printRouters();
        std::string src, dst;
        std::cout << "\nIngrese el router origen: ";
        std::cin >> src;
        std::cout << "Ingrese el router destino: ";
        std::cin >> dst;

        std::cout << "\n";
        net.printRoute(src, dst);
        wait_enter();
        break;
      }

      case 2: { // Ver tabla de enrutamiento
        clear_console();
        net.printRouters();
        std::string routerName;
        std::cout << "\nIngrese el nombre del router a consultar: ";
        std::cin >> routerName;

        net.printRouterTable(routerName);
        wait_enter();
        break;
      }

      case 3: {
        clear_console();
        net.printRouters();
        wait_enter();
        break;
      }

      case 4: {
        clear_console();
        net.printRouters();
        std::string src, dst;
        size_t cost = 0;

        std::cout << "\nIngrese el primer router: ";
        std::cin >> src;
        std::cout << "Ingrese el segundo router: ";
        std::cin >> dst;

        std::cout << "Ingrese el costo del enlace (entero positivo): ";
        if (invalid_input(cost) || cost == 0) {
          std::cerr << "\n[!] El costo debe ser mayor a 0.\n";
          wait_enter();
          break;
        }

        net.updateLink(src, dst, static_cast<unsigned int>(cost));
        std::cout << "\n[OK] Enlace (" << src << " <-> " << dst 
                  << ") actualizado con costo " << cost << ".\n";
        wait_enter();
        break;
      }

      case 5: { // Agregar router
        clear_console();
        std::string routerName;
        std::cout << "Ingrese el nombre del nuevo router: ";
        std::cin >> routerName;

        net += routerName;
        std::cout << "\n[OK] Router [" << routerName << "] registrado en la red.\n";
        wait_enter();
        break;
      }

      case 6: { // Eliminar router
        clear_console();
        net.printRouters();
        std::string routerName;
        std::cout << "\nIngrese el router a eliminar: ";
        std::cin >> routerName;

        net.printRouterTable(routerName);

        std::cout << "¿Seguro que desea eliminar el router " << routerName << "? (s/n): ";
        char confirm;
        std::cin >> confirm;
        if (confirm == 's' || confirm == 'S') {
          net -= routerName;
          std::cout << "\n[OK] Router [" << routerName << "] eliminado y rutas recalculadas.\n";
        } else {
          std::cout << "\nOperacion cancelada.\n";
        }
        wait_enter();
        break;
      }

      case 7: { //guardar cambios
        clear_console();
        std::string filepath;
        std::cout << "Ingrese la ruta del archivo (o presione ENTER para usar la ruta por defecto): ";
        std::cin.ignore();
        std::getline(std::cin, filepath);

        bool success = filepath.empty() ? net.saveNetwork() : net.saveNetwork(filepath);
        if (success) {
          std::cout << "\nRed guardada exitosamente.\n";
        } else {
          std::cerr << "\nError al escribir en el archivo.\n";
        }
        wait_enter();
        break;
      }
    }
  }
}
