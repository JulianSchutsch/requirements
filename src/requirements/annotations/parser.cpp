#include "requirements/annotations/parser.hpp"

#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "util/lineparser.hpp"

#include "requirements/node.hpp"
#include "requirements/nodecollection.hpp"

namespace requirements {
  namespace annotations {
    
    namespace {
      struct Builders {
        MajorPrefix majorPrefix;
        SectionsBuilder sections;
        ErrorsBuilder errors;
        ShortcutsBuilder shortcuts;
        RequirementsBuilder requirements;
        AcceptancesBuilder acceptances;
        ScenesBuilder scenes;
        PhasesBuilder phases;
        NodePtr trashNode;
        
        Builders(ParserResult &result, NodePtr a_trashNode)
          : sections(*result.sections)
          , errors(*result.errors)
          , shortcuts(*result.shortcuts)
          , requirements(*result.requirements, majorPrefix)
          , acceptances(*result.acceptances, majorPrefix)
          , scenes(*result.scenes, majorPrefix)
          , phases(*result.phases)
          , trashNode(a_trashNode) {}
      };
    }

    static bool shortcutSectionHelper(::requirements::NodePtr node, Builders& builders, const std::string& parameters,
                                      std::function<bool(const std::string& title, const std::string& shortcut)> parser) {
      static std::regex sectionTitle(R"(\s*(\w+)\s*(.*))");
      std::smatch matches;
      if(!std::regex_match(parameters, matches, sectionTitle)) {
        builders.errors.set(node->getId(), "A specialized section title must be a shortcut followed by an arbitrary title string");
        return false;
      }
      const std::string& shortcut = matches[1];
      const std::string& title = matches[2];

      builders.shortcuts.set(node->getId(), title);
      return parser(title, shortcut);
    }

    template<bool subParser(::requirements::NodePtr node, ParserResult& result, Builders& builders)>
    static bool iterChildren(::requirements::NodePtr parent, ParserResult& result, Builders& builders) {
      bool success = true;
      for (auto &child: parent->getChildren()) {
        if(child==builders.trashNode) {
          continue;
        }
        if(!subParser(child, result, builders)) {
          success = false;
        }
      }
      return success;
    }

    static bool parseSectionContext(::requirements::NodePtr node, ParserResult &result, Builders &builders);
    
    static bool parseRequirement(::requirements::NodePtr node, ParserResult &result, Builders &builders) {
      RequirementsBuilderScope scope(builders.requirements, node->getId(), node->getContent());
      builders.sections.addElement(node->getId());
      builders.shortcuts.set(node->getId(), scope.getKey());
      return  iterChildren<parseRequirement>(node, result, builders);
    }
    
    static bool parseRequirementSection(::requirements::NodePtr node, ParserResult &result, Builders &builders,
                                        util::LineParser &parser, const std::string &parameters) {
      return shortcutSectionHelper(node, builders, parameters,
        [&](const std::string& title, const std::string& shortcut){
          builders.requirements.setMajorPrefix(shortcut);
          SectionsBuilderScope section(builders.sections, title, parser.consumeAll(), "");
          return iterChildren<parseRequirement>(node, result, builders);
        });
    }
    
    static bool parseAcceptance(::requirements::NodePtr node, ParserResult& result, Builders& builders) {
      ::util::LineParser parser(node->getContent());
      std::stringstream text;
      std::vector<::requirements::Id> accepts;
      for(;;) {
        static std::regex acceptsRegex(R"(accepts:\s*(\S+)\s*)");
        std::smatch matches;
        if(parser.consume(acceptsRegex, matches)) {
          auto idStr = matches[1];
          ::requirements::Id id;
          if(!string_to_id(idStr, id)) {
            builders.errors.set(node->getId(), "Accepts must have valid uuid as parameter (node pointer)");
            return false;
          }
          accepts.emplace_back(id);
          continue;
        }
        std::string line;
        if(!parser.consume(line)) {
          break;
        }
        text<<line;
      }
      AcceptancesBuilderScope scope(builders.acceptances, node->getId(), text.str(), std::move(accepts));
      builders.sections.addElement(node->getId());
      builders.shortcuts.set(node->getId(), scope.getKey());

      return iterChildren<parseAcceptance>(node, result, builders);
    }

    static bool parseAcceptanceSection(::requirements::NodePtr node, ParserResult& result, Builders& builders,
                                       util::LineParser& parser, const std::string& parameters) {
      return shortcutSectionHelper(node, builders, parameters,
        [&](const std::string& title, const std::string& shortcut) {
          builders.acceptances.setMajorPrefix(shortcut);

          SectionsBuilderScope section(builders.sections, title, parser.consumeAll(), "");
          return iterChildren<parseAcceptance>(node, result, builders);
        });
    }

    static bool parseScene(::requirements::NodePtr node, ParserResult& result, Builders& builders) {
      ::util::LineParser parser(node->getContent());
      std::stringstream text;
      std::vector<Scene::Risk> risks;
      bool hasProbability = false;
      float probability;
      for(;;) {
        static std::regex riskRegex(R"(risk:\s*(\S+)\s*(.*))");
        std::smatch matches;
        if(parser.consume(riskRegex, matches)) {
          auto levelStr = matches[1];
          auto description = matches[2];
          static std::map<std::string, Scene::RiskLevel> levels = {
            {"high", Scene::RiskLevel::High},
            {"medium", Scene::RiskLevel::Medium},
            {"low", Scene::RiskLevel::Low}
          };
          auto levelIt = levels.find(levelStr);
          if(levelIt==levels.end()) {
            builders.errors.set(node->getId(), "Risks must have valid risk level (low, medium or high)");
            return false;
          }
          risks.emplace_back(levelIt->second, description);
          continue;
        }
        static std::regex probabilityRegex(R"(probability:\s*(\S+)\s*)");
        if(parser.consume(probabilityRegex, matches)) {
          if(hasProbability) {
            builders.errors.set(node->getId(), "Two probabilities for a single scene element specified.");
          }
          try {
            probability = ::boost::lexical_cast<float>(matches[1]);
          } catch(::boost::bad_lexical_cast& e) {
            builders.errors.set(node->getId(), "Probability requires single float parameter between 0 and 1.");
          }
          if(probability<0 || probability>1) {
            builders.errors.set(node->getId(), "Probability out of range, must be between 0 and 1.");
          }
          hasProbability = true;
        }
        std::string line;
        if(!parser.consume(line)) {
          break;
        }
        text<<line;
      }
      ScenesBuilderScope scope(builders.scenes, node->getId(), text.str(), std::move(risks), hasProbability, probability);
      builders.sections.addElement(node->getId());
      builders.shortcuts.set(node->getId(), scope.getKey());

      return iterChildren<parseScene>(node, result, builders);
    }

    static bool parseSceneSection(::requirements::NodePtr node, ParserResult& result, Builders& builders,
                                       util::LineParser& parser, const std::string& parameters) {
      return shortcutSectionHelper(node, builders, parameters,
        [&](const std::string& title, const std::string& shortcut) {
          builders.scenes.setMajorPrefix(shortcut);
          SectionsBuilderScope section(builders.sections, title, parser.consumeAll(), ""  );
          return iterChildren<parseScene>(node, result, builders);
        });
    }

    static bool parsePhase(::requirements::NodePtr node, ParserResult& result, Builders& builders, util::LineParser& parser, const std::string& parameters) {
      // ShortcutSectionHelper is used here, since the first parameter (which is a phase folder), is identicial to a simple file name
      return shortcutSectionHelper(node, builders, parameters,
        [&](const std::string& title, const std::string& phaseIdentifier) {
          if(!builders.phases.addPhase(node->getId(), phaseIdentifier)) {
            builders.errors.set(node->getId(), "Phase identifier already in use");
            return false;
          }
          SectionsBuilderScope section(builders.sections, title, parser.consumeAll(), phaseIdentifier);
          return iterChildren<parseSectionContext>(node, result, builders);
        });
    }

    static bool parseSection(::requirements::NodePtr node, ParserResult &result, Builders &builders, util::LineParser &parser,
                 const std::string &parameters) {
      auto title = boost::algorithm::trim_copy(parameters);
      builders.shortcuts.set(node->getId(), title);
      SectionsBuilderScope section(builders.sections, title, parser.consumeAll(), "");
      return iterChildren<parseSectionContext>(node, result, builders);
    }
    
    static bool parseSectionContext(::requirements::NodePtr node, ParserResult &result, Builders &builders) {
      util::LineParser parser(node->getContent());
      while (parser.skipEmptyLine()) {}
      std::smatch matches;
      static std::regex sectionRegex(R"((\w+):\s*(.*))");
      if (!parser.consume(sectionRegex, matches)) {
        builders.errors.set(node->getId(), "First non empty line must be a \"tag:\"");
        return false;
      }
      
      static std::map<std::string, bool (*)(::requirements::NodePtr node, ParserResult &result, Builders &builders,
                                            util::LineParser &parser,
                                            const std::string &parameters)> sectionContextTags = {
        {"section",      parseSection},
        {"requirements", parseRequirementSection},
        {"acceptance",  parseAcceptanceSection},
        {"scenes", parseSceneSection},
        {"phase", parsePhase}
      };
      auto tagIt = sectionContextTags.find(matches[1]);
      if (tagIt == sectionContextTags.end()) {
        builders.errors.set(node->getId(), "Unknown tag in section context, " + std::string(matches[1]));
        return false;
      }
      return tagIt->second(node, result, builders, parser, matches[2]);
    }

    static bool recursiveRequirementsAcceptance(::requirements::Id id, ::requirements::NodeCollection& collection, ParserResult& result) {
      bool covered = true;
      auto parent = collection.getNodeById(id);
      for(auto& node: parent->getChildren()) {
        if(!recursiveRequirementsAcceptance(node->getId(), collection, result)) {
          covered = false;
        }
      }
      auto& requirement = result.requirements->access(id);
      covered = covered && requirement.isCoveredByAcceptance();
      requirement.setTreeCoveredByAcceptance(covered);
      return covered;
    }

    static bool requirementsAcceptance(::requirements::NodeCollection& collection, ParserResult& result) {
      for(auto& acceptance: *result.acceptances) {
        for(auto& accepts: acceptance.second.getAccepts()) {
          if(!result.requirements->has(accepts)) {
            result.errors->insert(acceptance.first, "accepts points to non existent node "+::requirements::id_to_string(acceptance.first));
            return false;
          }
          result.requirements->access(accepts).setCoveredByAcceptance(true);
        }
      }
      for(const auto& rootNode: result.requirements->getRootEntries()) {
        recursiveRequirementsAcceptance(rootNode, collection, result);
      }
      return true;
    }
    
    bool parse(::requirements::IStorage &storage, ParserResult &result) {
      result.errors.reset(new Errors());
      result.sections.reset(new Sections());
      result.requirements.reset(new Requirements());
      result.acceptances.reset(new Acceptances());
      result.shortcuts.reset(new Shortcuts());
      result.scenes.reset(new Scenes());
      result.phases.reset(new Phases());
      auto& collection = storage.getNodeCollection();
      Builders builders(result, collection.getTrashNode());
      if(!iterChildren<parseSectionContext>(collection.getRootNode(), result, builders)) {
        return false;
      }
      return requirementsAcceptance(storage.getNodeCollection(), result);
    }
  }
}