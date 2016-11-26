#include <iostream>

#include "requirements/id.hpp"
#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/storage/text_load.hpp"
#include "requirements/storage/text_save.hpp"
#include "requirements/content_text.hpp"

int main(int argc, char** args) {
  (void)argc;
  (void)args;
//  requirements::TextStorage storage("/home/alexandrus/testFolder");
  requirements::NodeCollection collection;
  auto parent = collection.getRootNode();
  auto node1 = collection.createNode(std::make_unique<requirements::Content_Text>());
  auto node2 = collection.createNode(std::make_unique<requirements::Content_Text>());
  for(auto node: collection.getRootNode()->getChildren()) {
    std::cout<<requirements::id_to_string(node->getId())<<" "<<node->getContent().getDescription()<<std::endl;
  }
  return 0;
}