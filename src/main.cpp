#include <iostream>

#include "requirements/id.hpp"
#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/storage/text_load.hpp"
#include "requirements/storage/text_save.hpp"
#include "requirements/content_text.hpp"

const char chars[] = {
   "\xe2\x94\x80"
   "\xe2\x94\x82"
   "\xe2\x94\x9c"
   "\xe2\x94\x94"
   "\xe2\x94\x8c"
   "\xe2\x94\xBC"
   "\xe2\x94\x80"
   "\0"
};

int main(int argc, char** args) {
  (void)argc;
  (void)args;
  
  const std::string& folder = "/home/alexandrus/testFolder";

  requirements::NodeCollection collection;
  requirements::storage::text_load(collection, folder);
  auto parent = collection.getRootNode();
  auto node1 = collection.createNode(std::make_unique<requirements::Content_Text>());
  auto node2 = collection.createNode(std::make_unique<requirements::Content_Text>());
  node2->setParent(node1);
  for(auto node: collection.getRootNode()->getChildren()) {
    std::cout<<requirements::id_to_string(node->getId())<<" "<<node->getContent().getDescription()<<std::endl;
  }
  requirements::storage::text_save(collection, folder);
  std::cout<<chars<<std::endl;
  return 0;
}