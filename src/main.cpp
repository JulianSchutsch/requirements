#include <iostream>

#include "requirements/id.hpp"
#include "requirements/node.hpp"
#include "requirements/textstorage.hpp"

int main(int argc, char** args) {
  requirements::TextStorage storage("/home/alexandrus/testFolder");
  auto node = storage.createNode();
  std::cout<<requirements::id_to_string(node->getId())<<std::endl;
  return 0;
}