#include "requirements/generators/latex/blob.hpp"

#include <iostream>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "requirements/istorage.hpp"
#include "requirements/blob.hpp"

namespace requirements {
  namespace generators {
    namespace latex {
      bool translateBlob(::requirements::IStorage &storage, const std::string &description, BlobDescription &result) {
        auto selected = ::requirements::selectBlobs(storage, {boost::algorithm::trim_copy(description)});
        if (selected.size() != 1) {
          std::cout << "More than one blob selected during generation" << std::endl;
          return false;
        }
        auto id = selected[0];
        const std::string filename = storage.getBlobFilename(id);
        const std::string suffix = storage.getBlobSuffix(id);
        const std::string latexFolder = storage.getLatexFolder();
        const std::string intermediateFile = latexFolder + id;
        if (suffix == ".uxf") {
          std::string targetFile = boost::filesystem::change_extension(intermediateFile, ".pdf").native();
          std::string command = "umlet -action=convert -format=pdf -filename=" + filename + " -output=" + targetFile;
          system(command.c_str());
          result.filename = targetFile;
          result.type = BlobType::Image;
          return true;
        }
        return false;
      }
    }
  }
}