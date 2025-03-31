#include <thread>

#include "./Base.h++"
#include "./container/Vector.h++"

namespace Senpai {

// wrapper for std::thread
struct Thread {
  private:
   UniquePtr<std::thread> thread = nullptr;
   bool finished = false;
   bool success = false;

  public:
   inline Thread() = default;
   inline ~Thread() {
      this->join();
   }

   template <typename Fn, typename... Args>
   void execute(Fn&& f, Args&&... args) {
      // Bind the function with its arguments, preserving reference qualifiers.
      auto boundFunc =
          std::bind(std::forward<Fn>(f), std::forward<Args>(args)...);
      thread =
          UniquePtr<std::thread>(new std::thread([boundFunc, this]() mutable {
             try {
                boundFunc();
                success = true;
             } catch (Exception const& e) {
                cout << e.what() << endl;
                success = false;
             }
             finished = true;
          }));
   }

   // returns true if the thread is running
   inline bool is_running() { return thread != nullptr; }

   // returns true if the running thread has finished execution
   inline bool is_finished() { return finished; }

   // joins the thread and resets it, returns true if no Exceptions accured
   inline bool join() {
      if (!thread) {
         return false;
      }
      thread->join();
      thread.reset();
      finished = false;
      if (success) {
         success = false;
         return true;
      } else {
         return false;
      }
   }
};

}  // namespace Senpai