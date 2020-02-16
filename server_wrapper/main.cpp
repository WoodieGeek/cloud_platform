#include "../library/server.h"
#include "lib/server.h"

#include <iostream>

int main() {
  try {
      std::cout << "Starting...\n";
      TServerWrapper server("127.0.0.1", "8080");
  } catch (std::exception& e) {
      std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
