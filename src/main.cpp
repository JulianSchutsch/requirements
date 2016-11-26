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
  
  const std::string& folder = "/home/alexandrus/testFolder";

  requirements::NodeCollection collection;
  requirements::storage::text_load(collection, folder);
  auto parent = collection.getRootNode();
  auto node1 = collection.createNode(std::make_unique<requirements::Content_Text>());
  auto node2 = collection.createNode(std::make_unique<requirements::Content_Text>());
  for(auto node: collection.getRootNode()->getChildren()) {
    std::cout<<requirements::id_to_string(node->getId())<<" "<<node->getContent().getDescription()<<std::endl;
  }
  requirements::storage::text_save(collection, folder);
  return 0;
}