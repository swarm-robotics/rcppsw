/*******************************************************************************
 * Name            : threadable.hpp
 * Project         : paradyn
 * Module          : paradyn
 * Description     : Common threading functionality
 * Creation Date   : Sat Jul 18 14:33:49 2015
 * Original Author : jharwell
 *
 ******************************************************************************/

#ifndef _THREADABLE_HPP_
#define _THREADABLE_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "altypes.h"
#include <thread>


/*******************************************************************************
 * SVN Version
 ******************************************************************************/
static char __unused svnid_threadable_hpp_[] =
  "$Id:$ SwRI";

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
class threadable
{
public:
    /* data members */

    /* member functions */
    threadable(void) : thread_run(false),
                       internal_thread() {}
    virtual ~threadable(void) {}

    virtual void start(
        int core = -1);

    virtual void term(void)
        {
            thread_run = false;
            internal_thread.join();
        } /* phenf::term() */
    bool terminated(void) { return (false == thread_run); }
    virtual void join(void) { internal_thread.join(); }
    virtual void thread_main(void) = 0;
    status_t thread_lock_to_core(
        pthread_t thread,
        int core);

protected:
    pthread_t thread_handle(void) { return internal_thread.native_handle(); }

private:
    /* data members */
    bool thread_run;
    std::thread internal_thread;

    /* non-member functions */
    static void entry_point(void* this_p) { threadable *pt = (threadable*)this_p; pt->thread_main(); }
    /* operators */
};

/*******************************************************************************
 * Operater Definitions
 ******************************************************************************/

#endif /*  _THREADABLE_HPP_  */
