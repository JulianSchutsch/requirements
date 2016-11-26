#include <iostream>

#include "requirements/id.hpp"
#include "requirements/node.hpp"
#include "requirements/textstorage.hpp"
#include "requirements/content_text.hpp"

int main(int argc, char** args) {
  (void)argc;
  (void)args;
  requirements::TextStorage storage("/home/alexandrus/testFolder");
  auto parent = storage.getRootNode();
  auto node1 = storage.createNode(std::make_unique<requirements::Content_Text>());
  auto node2 = storage.createNode(std::make_unique<requirements::Content_Text>());
  for(auto node: storage.getRootNode()->getChildren()) {
    std::cout<<requirements::id_to_string(node->getId())<<" "<<node->getContent().getDescription()<<std::endl;
  }
  storage.save();
  return 0;
}