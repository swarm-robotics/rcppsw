/*******************************************************************************
 * Name            : forkable.cpp
 * Project         : paradyn
 * Module          : paradyn
 * Description     : Common fork()/exec() wrappers
 * Creation Date   : Sun Aug 16 10:30:07 2015
 * Original Author : jharwell
 *
 ******************************************************************************/

#ifndef _FORKABLE_CPP_
#define _FORKABLE_CPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "utils.h"
#include "dbg.h"
#include "forkable.hpp"

#include <vector>
#include <string>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*******************************************************************************
 * SVN Version
 ******************************************************************************/
static char __unused svnid_forkable_cpp_[] =
  "$Id:$ SwRI";

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
using namespace paradyn;

/*******************************************************************************
 * Constant Definitions
 ******************************************************************************/

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

/*******************************************************************************
 * Forward Declarations
 ******************************************************************************/

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
/**
 * forkable::start() - Start a process
 *
 * RETURN:
 *     pid_t - PID of child process
 *
 **/
pid_t forkable::start(
    int core)
{
    proc_run = true;
    _pid = fork();
    if (_pid == 0) {
        if (-1 != core) {
            proc_lock_to_core(core);
        }
        entry_point(this);
    }
    return _pid;
} /* forkable::start() */

/**
 * forkable::start() - Change cwd and Start a process
 *
 * RETURN:
 *     pid_t - PID of child process
 *
 **/
pid_t forkable::start(
    const std::string& new_wd,
    int core)
{
    proc_run = true;
    _pid = fork();
    if (_pid == 0) {
        if (0 != chdir(new_wd.c_str())) {

            return -1;
        }
        if (-1 != core) {
            proc_lock_to_core(core);
        }
        entry_point(this);
    }
    return _pid;
} /* forkable::start() */

/*******************************************************************************
 * Non-Member Functions
 ******************************************************************************/
/**
 * proc_lock_to_core() - Lock a process to a core
 *
 * RETURN:
 *     OK if successful, ERROR otherwise
 *
 **/
status_t proc_lock_to_core(
    int core)
{
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(core,&cpuset);

    check(0 == sched_setaffinity(0,sizeof(cpu_set_t),&cpuset));
    return OK;

error:
    return ERROR;
} /* proc_lock_to_core() */

/**
 * fork_exec() - Fork(), exec(), returning pid of child in parent
 *
 * RETURN:
 *     pid_t - pid of child
 *
 **/
pid_t fork_exec(
    const std::vector<std::string>& cmd,
    const std::string& new_wd,
    bool stdout_sup,
    int* pipefd)
{
    pid_t pid = fork();
    if (0 == pid) {
        /* change to the working directory before exec()ing if requested */
        if ("" != new_wd) {
            if (0 != chdir(new_wd.c_str())) {
                return ERROR;
            }
        }

        /* suppress stdout */
        if (stdout_sup) {
            int fd = open("/dev/null",O_WRONLY);
            dup2(fd,1);
        }

        /* the child will read data on stdin from the parent */
        if (NULL != pipefd) {
            dup2(pipefd[0],STDIN_FILENO);
        }
        char *argv[cmd.size() + 1];
        memset(argv,0,sizeof(argv));
        unsigned i;
        for (i = 0; i < cmd.size(); ++i) {
            argv[i] = const_cast<char*>(cmd[i].c_str());
        } /* for() */
        argv[i] = NULL;
        execv(argv[0],argv);
        perror(NULL);
        assert(0); /* execv() should never return */
    } else {
        return pid;
    }
} /* fork_exec() */

#endif /*  _FORKABLE_CPP_  */
