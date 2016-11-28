#include <iostream>

#include "requirements/id.hpp"
#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/storage/text_load.hpp"
#include "requirements/storage/text_save.hpp"

#include "req/console/printtree.hpp"

int main(int argc, char** args) {
  (void)argc;
  (void)args;
  
  const std::string& folder = "/home/alexandrus/testFolder";

  requirements::NodeCollection collection;
  requirements::storage::text_load(collection, folder);
  auto parent = collection.getRootNode();
  auto node1 = collection.createNode("req 1");
  auto node2 = collection.createNode("req 2");
  auto node3 = collection.createNode("req 3");
  node2->setParent(node1);
  node3->setParent(node1);
  requirements::storage::text_save(collection, folder);
  
  req::console::printTree(std::cout, *collection.getRootNode());
  
  return 0;
}