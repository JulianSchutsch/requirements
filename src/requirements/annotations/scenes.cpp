#include "requirements/annotations/scenes.hpp"

namespace requirements {
  namespace annotations {
    Scene::Scene(const std::string& a_key, const std::string& a_text, const std::vector<Risk>& a_risks)
      : key(a_key)
      , text(a_text)
      , risks(a_risks) {}

  }
}
