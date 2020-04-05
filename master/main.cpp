#include "../library/server.h"
#include "lib/server.h"

#include <iostream>

int main() {
  try {
      std::cout << "Starting...\n";
      std::string connection = "host=localhost port=5432 dbname=jaunt_db user=runtime_jaunt password=rtjdb2020";
      TMasterServer server("127.0.0.1", "8080", connection);
  } catch (std::exception& e) {
      std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
