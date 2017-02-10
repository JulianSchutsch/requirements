#include "requirements/annotations/scenes.hpp"

namespace requirements {
  namespace annotations {
    Scene::Scene(const std::string& a_key, const std::string& a_text, const std::vector<Risk>& a_risks, bool a_hasProbability, float a_probability)
      : key(a_key)
      , text(a_text)
      , risks(a_risks)
      , hasProbability(a_hasProbability)
      , probability(a_probability) {}

  }
}
