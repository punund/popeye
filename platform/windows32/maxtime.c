#include "platform/maxtime_impl.h"
#include "windows.h"
#include "utilities/boolean.h"
#include "pymsg.h"

#include "debugging/assert.h"
#include <limits.h>

/* Implementation of option maxtime using Windows Multimedia timers
 */

/* Singular value indicating that an MMRESULT object does not hold an
 * actual timer resource.
 * The docs
 * (http://msdn.microsoft.com/en-us/library/ms713423(VS.85).aspx) say
 * that timeSetEvent() returns NULL if it fails; but MMRESULT is not a
 * pointer type ...
 */
static MMRESULT const no_timer = 0;

/* id of current Multimedia Timer
 */
static MMRESULT current_timer;

/* The Windows APIs use all kinds of types with strange Hungarian
 * names, but it all boils down to:
 */
typedef unsigned long timer_period_type;

/* Actual timer resolution; initialized with intended resolution
 */
static timer_period_type timer_resolutionMS = 1000;

/* Singular value that we use to indicate that the Multimedia Timers
 * machinery is not available.
 */
static timer_period_type const MM_timers_not_available = 0;

/* Calibrate timer resolution based on intended resolution and
 * capabilities indicated by Windows.
 */
static void calibrate_timer_resolution(void)
{
  TIMECAPS tc;
  if (timeGetDevCaps(&tc, sizeof tc) == TIMERR_NOERROR)
  {
    if (timer_resolutionMS<tc.wPeriodMin)
      timer_resolutionMS = tc.wPeriodMin;
    else if (timer_resolutionMS>tc.wPeriodMax)
      timer_resolutionMS = tc.wPeriodMax;
  }
  else
    timer_resolutionMS = MM_timers_not_available;
}

/* Callback function for Multimedia Timer.
 * Called back by Windows every actual_timer_delayMS milliseconds
 */
static void CALLBACK tick(unsigned int timer_id,
                          unsigned int reserved1,
                          unsigned long seconds_elapsed,
                          unsigned long reserved2,
                          unsigned long reserved3)
{
  /* assert(timer_id==current_timer);
   * is probably not a good idea in (something similar to) a signal
   * handler ...
   */

  ++periods_counter;
}

/* Attempt to set up a new timer for timing out solving after a number
 * of seconds.
 * @param seconds number of seconds after which solving is aborted
 */
static void setupNewTimer(maxtime_type seconds)
{
  unsigned int const intended_period_lengthMS = 1000;
  unsigned int const period_lengthMS = (intended_period_lengthMS
                                        <timer_resolutionMS
                                        ? timer_resolutionMS
                                        : intended_period_lengthMS);

  unsigned int const milliseconds = 1000*seconds;
  nr_periods = milliseconds/period_lengthMS;

  current_timer = timeSetEvent(period_lengthMS,timer_resolutionMS,
                               &tick,0,
                               TIME_PERIODIC|TIME_CALLBACK_FUNCTION);
  if (current_timer==no_timer)
  {
    VerifieMsg(NoMaxTime);
    nr_periods = 0;
    periods_counter = 1;
  }
  else
    periods_counter = 0;
}

/* Kill previously used timer resource (if any)
 */
static void killPreviousTimer(void)
{
  if (current_timer!=no_timer)
  {
    MMRESULT const kill_result = timeKillEvent(current_timer);
    assert(kill_result==TIMERR_NOERROR);
    current_timer = no_timer;
  }
}

void initMaxtime(void)
{
  /* Make sure that the variables we update in (something similar to)
   * a signal are large enough to hold timer period values.
   */
  assert(sizeof(timer_period_type)<=sizeof(sig_atomic_t));
  assert(sizeof(maxtime_type)<=sizeof(sig_atomic_t));

  calibrate_timer_resolution();
  current_timer = no_timer;

  /* we support up to UINT_MAX milliseconds (i.e. ~1 month)
   */
  maxtime_maximum_seconds = UINT_MAX/1000;
}

void setMaxtime(maxtime_type seconds)
{
  killPreviousTimer();

  if (seconds==no_time_set)
  {
    nr_periods = 1;
    periods_counter = 0;
  }
  else if (timer_resolutionMS==MM_timers_not_available)
  {
    VerifieMsg(NoMaxTime);
    nr_periods = 0;
    periods_counter = 1;
  }
  else
    setupNewTimer(seconds);
}
