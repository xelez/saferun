See README for basic information on dependences

# Kernel
 * It must be compiled with:
    CONFIG_CGROUPS=y
    CONFIG_CGROUP_DEVICE=y
    CONFIG_CGROUP_CPUACCT=y
    CONFIG_CGROUP_MEM_RES_CTLR=y
    CONFIG_CGROUP_MEM_RES_CTLR_SWAP=y
    CONFIG_CGROUP_MEM_RES_CTLR_SWAP_ENABLED=y
    CONFIG_RESOURCE_COUNTERS=y
    CONFIG_UTS_NS=y
    CONFIG_IPC_NS=y
    CONFIG_PID_NS=y
    CONFIG_NET_NS=y
 * You can see if kernel compiled with these flags
    $ cat /boot/config-`uname -r` | grep _NS
    $ cat /boot/config-`uname -r` | grep CGROUP
    $ cat /boot/config-`uname -r` | grep RESOURCE_COUNTERS
 * Recent Ubuntu and Debian Testing works
 * Debian 6.0 Squeeze compiled without cgroup memory subsystem, but you can use backported kernel
   from http://backports-master.debian.org/

# Cgroups
 * On Ubuntu you can just install cgroup-lite and it would mount it automatically on boot
 * Or you can do the following as root:
    # mkdir /cgroup
    # echo >> /etc/fstab
    # echo 'cgroup /cgroup cgroup defaults 0 0' >> /etc/fstab
    # mount /cgroup
   This will automatically mount cgroup on boot.


