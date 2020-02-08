#include "../library/server.h"

#include <bits/stdc++.h>

int main() {
  try
  {
      TServer server("127.0.0.1", "8080");
      std::cout << "Starting...\n";
      server.Run();
  }
  catch (std::exception& e)
  {
      std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
