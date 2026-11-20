#pragma once

#include <cstddef>

#include "Network.hpp"

bool invalid_input(size_t& variable);

void clear_console();

void wait_enter();

void printMenu();

void printSubMenu();

void networkSession(Network& net);
