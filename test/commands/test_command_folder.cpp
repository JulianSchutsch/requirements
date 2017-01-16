#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include "test/folder.hpp"

#include "requirements/commands/command.hpp"
#include "requirements/commands/folder.hpp"
#include "requirements/commands/null.hpp"
#include "requirements/batch/thread.hpp"
#include "requirements/batch/response.hpp"

using namespace requirements;
 
TEST(Commands, Command_Folder) {
  ::test::UniqueFolder folder;
  std::string statusFile = folder.getName()+"_status.xml";
  {
    std::string batchFolder;
    volatile bool batchFolderSet = false;
    std::mutex responseMutex;
    std::condition_variable responseCondition;
    batch::Thread batchThread(
      [&batchFolder, &batchFolderSet, &responseMutex, &responseCondition]
        (batch::Response &&response) {
        std::lock_guard<std::mutex> guard(responseMutex);
        batchFolder = response.status->folder;
        batchFolderSet = true;
        responseCondition.notify_all();
      },
      [](ICommand::MessageKind, const std::string&){
        FAIL();
      }, statusFile);
    batchThread.enqueue(std::make_unique<commands::Folder>(folder.getName()));
    std::unique_lock<std::mutex> guard(responseMutex);
    while (!batchFolderSet) {
      responseCondition.wait(guard);
    }
    ASSERT_EQ(batchFolder, folder.getName());
  }
  {
    std::string batchFolder;
    volatile bool batchFolderSet = false;
    std::mutex responseMutex;
    std::condition_variable responseCondition;
    batch::Thread batchThread(
      [&batchFolder, &batchFolderSet, &responseMutex, &responseCondition]
        (batch::Response &&response) {
        std::lock_guard<std::mutex> guard(responseMutex);
        batchFolder = response.status->folder;
        batchFolderSet = true;
        responseCondition.notify_all();
      },
      [](ICommand::MessageKind, const std::string&){
        FAIL();
      }, statusFile);
    batchThread.enqueue(std::make_unique<commands::Null>());
    std::unique_lock<std::mutex> guard(responseMutex);
    while (!batchFolderSet) {
      responseCondition.wait(guard);
    }
    ASSERT_EQ(batchFolder, folder.getName());
  }
  boost::filesystem::remove(statusFile);
}

TEST(Commands, Command_Folder_Console) {
  ::test::UniqueFolder folder;
  std::string statusFile = folder.getName()+"_status.xml";
  {
    std::string batchFolder;
    volatile bool batchFolderSet = false;
    std::mutex responseMutex;
    std::condition_variable responseCondition;
    batch::Thread batchThread(
      [&batchFolder, &batchFolderSet, &responseMutex, &responseCondition]
        (batch::Response &&response) {
        std::lock_guard<std::mutex> guard(responseMutex);
        batchFolder = response.status->folder;
        batchFolderSet = true;
        responseCondition.notify_all();
      },
      [](ICommand::MessageKind, const std::string&){
        FAIL();
      },statusFile);
    batchThread.enqueue(commands::assembleFromString("folder "+folder.getName()));
    std::unique_lock<std::mutex> guard(responseMutex);
    while (!batchFolderSet) {
      responseCondition.wait(guard);
    }
    ASSERT_EQ(batchFolder, folder.getName());
  }
  {
    std::string batchFolder;
    volatile bool batchFolderSet = false;
    std::mutex responseMutex;
    std::condition_variable responseCondition;
    batch::Thread batchThread(
      [&batchFolder, &batchFolderSet, &responseMutex, &responseCondition]
        (batch::Response &&response) {
        std::lock_guard<std::mutex> guard(responseMutex);
        batchFolder = response.status->folder;
        batchFolderSet = true;
        responseCondition.notify_all();
      },
      [](ICommand::MessageKind, const std::string&) {
        FAIL();
      }, statusFile);
    batchThread.enqueue(std::make_unique<commands::Null>());
    std::unique_lock<std::mutex> guard(responseMutex);
    while (!batchFolderSet) {
      responseCondition.wait(guard);
    }
    ASSERT_EQ(batchFolder, folder.getName());
  }
  boost::filesystem::remove(statusFile);
}