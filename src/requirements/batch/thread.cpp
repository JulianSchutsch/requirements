#include "requirements/batch/thread.hpp"

namespace requirements {
   namespace batch {
     
     Thread::Thread(Queue::ResponseFunction a_responseFunction, const std::string &a_statusFilename)
       : queue(a_responseFunction, a_statusFilename)
       , thread(&Thread::mainloop, this) {}
     
     Thread::~Thread() {
       terminated = true;
       queue.notify();
       thread.join();
     }
     
     void Thread::mainloop() {
       while(!terminated) {
         queue.wait();
       }
       queue.finish();
     }
     
   }
}