#pragma once

#include <string>
#include <vector>

#include "requirements/status.hpp"

namespace qreq {
  struct BatchMessage {
    ::requirements::Status::MessageKind kind{};
    std::string message{};
    std::vector<std::string> parameters{};
    BatchMessage(::requirements::Status::MessageKind a_kind, const std::string& a_message, const std::vector<std::string>& a_parameters)
      : kind(a_kind)
      , message(a_message)
      , parameters(a_parameters) {}
    BatchMessage() = default;
  };

}