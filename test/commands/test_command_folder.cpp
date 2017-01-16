#include <gtest/gtest.h>

#include "test/folder.hpp"

#include "commands/command.hpp"
#include "commands/command_folder.hpp"
#include "commands/batchthread.hpp"
#include "commands/batchresponse.hpp"

namespace commands {
  
  TEST(Commands, Command_Folder) {
    ::test::UniqueFolder folder;
    std::string batchFolder;
    volatile bool batchFolderSet = false;
    std::mutex responseMutex;
    std::condition_variable responseCondition;
    ::commands::BatchThread batchThread([&batchFolder, &batchFolderSet, &responseMutex, &responseCondition](::commands::BatchResponse&& response) {
      std::lock_guard<std::mutex> guard(responseMutex);
      batchFolder = response.status->folder;
      batchFolderSet = true;
      responseCondition.notify_all();
    });
    batchThread.enqueue(std::make_unique<::commands::Command_Folder>(folder.getName()));
    std::unique_lock<std::mutex> guard(responseMutex);
    while(!batchFolderSet) {
      responseCondition.wait(guard);
    }
    ASSERT_EQ(batchFolder, folder.getName());
  }
  
}