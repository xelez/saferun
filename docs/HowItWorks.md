#How it works
 1. Creates new cgroup. Sets memory limit in it.
 2. It creates new process(called P futher in this file) in new namespace with clone().
 3. P sets up such things as chroot, makes a chdir, changes user and drops privelegies.
 4. Main process adds P to cgroup.
 5. Sets start time to measure real time used by P
 6. P execs something you need
 7. Main process starts hypervisor, that checks state of P with wait() every HYPERVISOR\_DELAY milliseconds
 8. Checking of memory and system+user time usage is done via memory and cpuaact cgroup subsystems
 9. If something goes bad, then hypervisor will kill P

#What to improve
 * waiting for the proccess.. if there is function something for waiting process for some time, use it

# Documentation for used things:
 * man 2 clone
 * <kernel-src>/Documentation/cgroups
 * <kernel-src>/Documentation/namespaces

