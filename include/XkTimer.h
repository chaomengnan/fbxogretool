#ifndef XK_TIMER_H
#define XK_TIMER_H

#include "XkConfig.h"
#include <Poco/Stopwatch.h>
#include <Poco/Timestamp.h>

namespace Xk {

class Timer
{
public:
	Timer()
	{
		m_sw.start();
	}

	~Timer() {}

	bool IsUp(int nSec)
	{
		Poco::Timestamp::TimeDiff diff = m_sw.elapsed();
		unsigned long ulSec = static_cast<unsigned long>(diff/(1000*1000));
		return ulSec >= nSec;
	}

	bool IsUpM(int nMs)
	{
		Poco::Timestamp::TimeDiff diff =m_sw.elapsed();
		unsigned long ulMs = static_cast<unsigned long>(diff/1000);
		return ulMs >= nMs;
	}

	void Restart()
	{
		m_sw.restart();
	}

	unsigned long Elapsed()
	{
		Poco::Timestamp::TimeDiff diff = m_sw.elapsed();
		unsigned long ulMs = static_cast<unsigned long>(diff/(1000));

		return ulMs;
	}

    float elapsedSecond()
    {
        Poco::Timestamp::TimeDiff diff = m_sw.elapsed();
		float fSec = diff * 0.001f * 0.001f;

		return fSec;
    }

private:
	Poco::Stopwatch m_sw;
};

class TimerProfile
{
public:
    TimerProfile(const char* func, const char* file, unsigned int line);
    ~TimerProfile();

private:
    Timer m_t;
    const char* szFunc;
    const char* szFile;
    unsigned int nLine;
};

//一个作用域内只允许出现一次此宏
#if XK_ENABLE_TIMER_PROFILE == 1
#define XK_TIMER_PROFILE Xk::TimerProfile timer_profile_______(__FUNCTION__, __FILE__, __LINE__);
#else
#define XK_TIMER_PROFILE /\
/
#endif

}//namespace Xk
#endif