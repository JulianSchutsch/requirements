#include "generators/latex/blob.hpp"

#include <iostream>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "requirements/istorage.hpp"
#include "requirements/id.hpp"

namespace generators {
  namespace latex {
    bool translateBlob(::requirements::IStorage& storage, const std::string& description, BlobDescription& result) {
      std::cout<<"Desc:"<<description<<std::endl;
      std::string id = boost::algorithm::trim_copy(description);
      const std::string filename = storage.getBlobFilename(id);
      const std::string suffix = storage.getBlobSuffix(id);
      std::cout<<"suf:"<<suffix<<":"<<std::endl;
      std::cout<<"Filename:"<<filename<<std::endl;
      const std::string latexFolder = storage.getLatexFolder();
      const std::string intermediateFile = latexFolder+id;
      if(suffix==".uxf") {
        std::string targetFile = boost::filesystem::change_extension(intermediateFile, ".pdf").native();
        std::cout<<"covert "<<filename<<" -> "<<targetFile<<std::endl;
        std::string command = "umlet -action=convert -format=pdf -filename="+filename+" -output="+targetFile;
        system(command.c_str());
        result.filename = targetFile;
        result.type = BlobType::Image;
        return true;
      }
      return false;
    }
  }
}