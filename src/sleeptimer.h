// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SLEEPTIMER_H
#define SLEEPTIMER_H

#include <QCoreApplication>
#include <QElapsedTimer>

#include <cmath>

class SleepTimer final
{
public:
    explicit SleepTimer(int timeoutMs)
        : m_timeoutMs(timeoutMs)
    {
        m_timer.start();
    }

    bool sleep()
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
        return m_timer.elapsed() < m_timeoutMs;
    }

    int remaining() const
    {
        const auto remaining = static_cast<int>(m_timeoutMs - m_timer.elapsed());
        return std::max(0, remaining);
    }

private:
    QElapsedTimer m_timer;
    int m_timeoutMs;
};

inline void waitFor(int ms)
{
    SleepTimer t(ms);
    while (t.sleep()) {}
}

#endif // SLEEPTIMER_H

