#include <iostream>
#include <fstream>
#include <string>
#include "treemap.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0]
              << " <donations_file.dat> <command> [<args>]"
              << std::endl;
    exit(1);
  }

  std::ifstream don_list;
  don_list.open(argv[1]);

  if (!don_list.good()) {
    std::cerr << "Error: cannot open file "<< argv[1] << std::endl;
    exit(1);
  }

  std::string opera;
  opera = argv[2];

  if (opera != "who" && opera != "cheap" && opera != "rich" && opera != "all") {
    std::cerr << "Command '" << argv[2]<< "' is invalid\n"
              << "Possible commands are: all|cheap|rich|who"
              << std::endl;
    exit(1);
  }

  if (opera == "who") {
    if (argc < 4) {
      std::cerr << "Command 'who' expects another argument: [+/-]amount"
                << std::endl;
      exit(1);
    }
  }

  Treemap<int, std::string> don_map;

  std::string name;
  int amount;
  std::string one_line;

  while (!don_list.eof()) {
    std::getline(don_list, name, ',');
    if (name.size() > 1) {
      don_list >> amount;
      don_list.get();
      don_map.Insert(amount, name);
    }
  }

  if (opera == "all") {
    while (don_map.Size() > 0) {
      std::cout << don_map.Get(don_map.MinKey())
                << " (" << don_map.MinKey() << ")"
                << '\n';
      don_map.Remove(don_map.MinKey());
    }
  }

  if (opera == "rich") {
    int richest_amount = don_map.MaxKey();
    std::cout << don_map.Get(richest_amount)
              << " (" << richest_amount << ")"
              << '\n';
  }

  if (opera == "cheap") {
    int poorest_amount = don_map.MinKey();
    std::cout << don_map.Get(poorest_amount)
              << " (" << poorest_amount << ")"
              << '\n';
  }

  if (opera == "who") {
    std::string that_name = "none";

    int floor_ceil = 0;
    int that_key = 0;
    if (argv[3][0] == '+') {
      floor_ceil = atoi(argv[3]) + 1;
      if (floor_ceil > don_map.MaxKey()) {
        std::cout << "No match" << '\n';
        return 0;
      }
      that_key = don_map.CeilKey(floor_ceil);
      that_name = don_map.Get(that_key);
    } else if (argv[3][0] == '-') {
      floor_ceil = 0 - atoi(argv[3]) - 1;
      if (floor_ceil < don_map.MinKey()) {
        std::cout << "No match" << '\n';
        return 0;
      }
      that_key = don_map.FloorKey(floor_ceil);
      that_name = don_map.Get(that_key);
    } else {
      that_key = atoi(argv[3]);
      if (don_map.ContainsKey(that_key)) {
        that_name = don_map.Get(that_key);
      } else {
        std::cout << "No match" << '\n';
        return 0;
      }
    }

    std::cout << that_name << " (" << that_key << ")";
  }

  return 0;
}