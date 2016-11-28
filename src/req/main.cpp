#include <iostream>

#include "requirements/id.hpp"
#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"
#include "requirements/storage/text_load.hpp"
#include "requirements/storage/text_save.hpp"

#include "req/console/printtree.hpp"

#include "req/config.hpp"

int main(int argc, char** args) {
  (void)argc;
  (void)args;
  
  req::Config config;
  config.load();
  const std::string& folder = "/home/alexandrus/testFolder";

  requirements::NodeCollection collection;
  requirements::storage::text_load(collection, folder);
  requirements::storage::text_save(collection, folder);
  
  req::console::printTree(std::cout, *collection.getRootNode());
  config.save();
  
  return 0;
}