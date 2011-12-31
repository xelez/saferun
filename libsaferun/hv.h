/*
 *
 */

#ifndef _HYPERVISOR_H
#define _HYPERVISOR_H

#include "saferun.h"
void hypervisor(const saferun_inst *inst, pid_t pid, const saferun_limits * limits,
                saferun_stat * stat);

#endif /*_HYPERVISOR_H */
