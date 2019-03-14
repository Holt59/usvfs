#include "semaphore.h"

#include <boost/interprocess/sync/named_recursive_mutex.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <spdlog.h>

using namespace boost::interprocess;

RecursiveSemaphore::RecursiveSemaphore(const char *name)
  : m_Mutex(nullptr)
  , m_Name(name)
{
  try {
    m_Mutex = new named_recursive_mutex(open_or_create, m_Name);
  } catch (interprocess_exception &e) {
    spdlog::get("usvfs")
      ->error("unable to open or create mutex {}", m_Name);
  }
}

RecursiveSemaphore::~RecursiveSemaphore()
{
  if (m_Mutex != nullptr)
    delete m_Mutex;
}

bool RecursiveSemaphore::lock()
{
  if (m_Mutex == nullptr)
    return false;

  bool res = false;
  try {
    boost::posix_time::ptime wait_time = microsec_clock::universal_time() + boost::posix_time::milliseconds(1000);
    if (m_Mutex->timed_lock(wait_time)) {
      res = true;
    } else {
      spdlog::get("usvfs")
        ->error("unable to lock mutex {}: timeout", m_Name);
    }
  } catch (interprocess_exception &e) {
    spdlog::get("usvfs")
      ->error("unable to lock mutex {}: {}", m_Name, e.what());
  }
  return res;
}

void RecursiveSemaphore::unlock()
{
  if (m_Mutex == nullptr)
    return;

  try {
    m_Mutex->unlock();

  } catch (interprocess_exception &e) {
    spdlog::get("usvfs")
      ->error("unable to unlock mutex {}: {}", m_Name, e.what());
  }

}

