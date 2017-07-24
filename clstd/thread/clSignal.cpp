﻿#include "clstd.h"
#include "Thread/clSignal.h"

namespace clstd
{

#if defined(_CPLUSPLUS_11_THREAD) || (__cplusplus >= 201103L) || (_MSC_VER >= 1900)
  namespace cxx11
  {
    const i32 Signal::eTimeOut;

    Signal::Signal()
    {
    }

    Signal::~Signal()
    {
    }

    i32 Signal::Set()
    {
      m_cond.notify_all();
      return 1;
    }

    i32 Signal::Wait()
    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock);
      return 0;
    }

    i32 Signal::WaitTimeOut(u32 dwMilliSec)
    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      return static_cast<i32>(m_cond.wait_for(_lock, std::chrono::microseconds(dwMilliSec)));
    }
  } // namespace cxx11
#endif // #if defined(_CPLUSPLUS_11_THREAD) || (__cplusplus >= 201103L) || (_MSC_VER >= 1900)

#if defined(POSIX_THREAD)
  namespace _posix
  {

    Signal::Signal()
      : m_cond(NULL)
      , m_mutex(NULL)
      , m_bSignaled(FALSE)
    {
      pthread_cond_init(&m_cond, NULL);
      pthread_mutex_init(&m_mutex, NULL);
    }

    Signal::~Signal()
    {
      pthread_cond_destroy(&m_cond);
      pthread_mutex_destroy(&m_mutex);
    }

    i32 Signal::Set()
    {
      pthread_mutex_lock(&m_mutex);
      m_bSignaled = TRUE;
      pthread_mutex_unlock(&m_mutex);
      return pthread_cond_signal(&m_cond);
    }

    i32 Signal::Wait()
    {
      i32 ret = 0;
      pthread_mutex_lock(&m_mutex);
      while( ! m_bSignaled)
      {
        ret = pthread_cond_wait(&m_cond, &m_mutex);
      }
      m_bSignaled = FALSE;
      pthread_mutex_unlock(&m_mutex);
      return ret;
    }

    i32 Signal::WaitTimeOut(u32 dwMilliSec)
    {
      if(dwMilliSec == TimeOut_Infinite)
      {
        return Wait();
      }

      timespec timeout;

#if 1
      // 这个算法不精确!
      // TODO: windows版下这个tv_nsec时间貌似是无效的, 所以用了近似值来代替, 注意要实现其他平台的版本
      timeout.tv_sec = time(0) + (dwMilliSec + 999) / 1000;
      timeout.tv_nsec = 0;
#else
      //timespec timeout;
      timeval now;
      gettimeofday(&now, NULL);
      int nsec = now.tv_usec * 1000 + (dwMilliSec % 1000) * 1000000;
      timeout.tv_nsec = nsec % 1000000000;
      timeout.tv_sec = now.tv_sec + nsec / 1000000000 + dwMilliSec / 1000;
#endif

      const int ret = pthread_cond_timedwait(&m_cond, &m_mutex, &timeout);
      return ret;
  }

    //i32 Signal::Reset()
    //{
    //  STATIC_ASSERT(0);
    //}

}
#endif

#ifdef _WIN32
  namespace _win32
  {
    Signal::Signal()
      : m_hEvent(NULL)
    {
       m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    Signal::~Signal()
    {
      CloseHandle(m_hEvent);
    }

    i32 Signal::Set()
    {
      return SetEvent(m_hEvent);
    }

    i32 Signal::Wait()
    {
      return WaitForSingleObject(m_hEvent, -1);
    }

    i32 Signal::WaitTimeOut(u32 dwMilliSec)
    {
      return WaitForSingleObject(m_hEvent, dwMilliSec);
    }

    //i32 Signal::Reset()
    //{
    //  return (i32)ResetEvent(m_hEvent);
    //}
  } // namespace _win32
#endif // #ifdef _WIN32
} // namespace clstd
