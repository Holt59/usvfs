#pragma once

#include <boost/interprocess//sync/named_recursive_mutex.hpp>

class RecursiveSemaphore
{
public:
  RecursiveSemaphore(const char *name);
  ~RecursiveSemaphore();

  bool lock();
  void unlock();

private:
  boost::interprocess::named_recursive_mutex *m_Mutex;
  const char *m_Name;

};
