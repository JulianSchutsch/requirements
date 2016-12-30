#include "requirements/storage/text.hpp"

#include <fstream>

#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/filesystem.hpp>
#include <requirements/storage/exception.hpp>

#include "util/path.hpp"

#include "requirements/storage/text_common.hpp"
#include "requirements/storage/text_load.hpp"
#include "requirements/storage/text_save.hpp"

namespace requirements {
  namespace storage {

    std::map<std::string, std::string> Text::getBlobAliases() {
      return blobAliases;
    }
    
    std::vector<std::string> Text::getBlobs() {
      std::vector<std::string> result;
      for(auto it=boost::filesystem::directory_iterator(folder+text_blobFolder);it!=boost::filesystem::directory_iterator();++it) {
        boost::filesystem::path path(*it);
        auto id = path.filename().native();
        result.emplace_back(std::move(id));
      }
      return std::move(result);
    }
    
    std::string Text::createBlob(const std::string& suffix) {
      return id_to_string(generateRandomId()) + "." + suffix;
    }
    
    std::string Text::getBlobFilename(const std::string& id) {
      return folder + text_blobFolder+id;
    }

    std::string Text::getBlobSuffix(const std::string& id) {
      boost::filesystem::path path(id);
      return path.extension().native();
    }

    std::string Text::getLatexFolder() {
      return folder + text_latexFolder;
    }

    void Text::loadBlobAliases() {
      {
        std::string blobAliasFilename = folder + text_blobaliasFile;
        if (!boost::filesystem::exists(blobAliasFilename)) {
          return;
        }
        std::fstream file(blobAliasFilename, std::fstream::in);
        std::string line;
        while (std::getline(file, line)) {
          auto idSeparator = line.find(' ');
          auto id = (idSeparator == std::string::npos)?line:std::string(line, 0, idSeparator);
          blobAliases[id] = line;
        }
      }
      // Ensure every blob has an alias, even if just a trivial one
      {
        auto blobs = getBlobs();
        for(auto& blob: blobs) {
          auto it = blobAliases.find(blob);
          if(it==blobAliases.end()) {
            blobAliases[blob] = blob;
          }
        }
      }
    }

    void Text::saveBlobAliases() {
      std::string blobAliasFilename = folder + text_blobaliasFile;
      std::fstream file(blobAliasFilename, std::fstream::out);
      for(auto& pair: blobAliases) {
        file<<pair.second<<std::endl;
      }
    }

    void Text::setBlobAliases(const std::string& blob, const std::string& alias) {
      auto it = blobAliases.find(blob);
      if(it==blobAliases.end()) {
        throw Exception(Exception::Reason::BlobNotFound);
      }
      it->second = it->first + " " + alias;
    }

    Text::Text(const std::string& a_folder, bool a_autosave)
      : folder(util::ensureTrailingSlash(a_folder))
      , autosave(a_autosave) {

      text_ensureFolder(folder);

      std::string lockFilename = folder+"lock";
      { std::fstream s(lockFilename, std::fstream::out); }
      fileLock.reset(new boost::interprocess::file_lock(lockFilename.c_str()));

      text_load(collection, folder);
      loadBlobAliases();
    }

    void Text::save(const std::string& a_folder) {
      folder = a_folder;
      save();
    }

    void Text::save() {
      text_save(collection, folder);
      saveBlobAliases();
    }

    Text::~Text() {
      if(autosave) {
        save();
      }
    }

  }
}