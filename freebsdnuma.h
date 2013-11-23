/*
 * FreeBSD NUMA project
 * The goal of this project is to develop an interface to allow NUMA aware
 * memory allocation from the FreeBSD user-space (expose NUMA functionality).
 * This will involve implementing syscalls to access NUMA information from
 * userspace and modifying the memory allocator to make allocation decisions
 * based on this information. The indicated APIs should support thread level
 * NUMA domain affinity.
 * 
 * NOTES:
 * Including header files found in dir /stable/10/sys/amd64/include/vmparam.h in 
 * the FreeBSD SVN repository. Since the currently involved stakeholders are
 * using Sandy Bridge E5-2620 which use the 64 bit instruction set, we will use
 * amd64 headers for now. FreeBSD notation seems to be <machine/*.h>. Its
 * probably pointing at an environment variable 'machine' to get to the correct
 * machine directory. http://svnweb.freebsd.org/base/stable/10/
 */


#ifndef __FREE_BSD_NUMA_H__
#define __FREE_BSD_NUMA_H__


/* ----------- INCLUDES ----------- */

#include <sys/proc.h>           /* process include */
#include <sys/pcpu.h>           /* include by <sys/proc.h> if _KERNEL not def */
#include <sys/cpuset.h>
#include </sys/vm/vm_phys.h>
#include <machine/vmparam.h>
#include <malloc.h>             /* Modified version of malloc */


/* ---------- DEFINITIONS --------- */

#define NUMA_POLICY_NEAREST
#define NUMA_POLICY_INTERLEAVE

#define NUMA_TOPOLOGY_CPUS
#define NUMA_TOPOLOGY_WEIGHT


/* --------- ENUMERATIONS --------- */

typedef int numa_policy;
typedef int numa_topology_selection;


/* ------- SYSCALL INTERFACE ------ */

/* Retrieves the memory affinity from the object specified by level, which 
 * and id and returns it as a mask stored in the space provided by mask
 */
int cpuset_get_memory_affinity(cpulevel_t level,
                               cpuwhich_t which,
                               id_t id,
                               size_t setsize,
                               cpuset_t *mask,
                               numa_policy policy);

/* Sets the memory affinity of the object specified by level,which and id to the
 * value stored in mask
 */
int cpuset_set_memory_affinity(cpulevel_t level,
                               cpuwhich_t which,
                               id_t id,
                               size_t setsize,
                               cpuset_t *mask,
                               numa_policy policy);

/* Moves specified pages on specified nodes to new memory nodes. */
long move_pages(int pid,
                unsigned long count,
                void **pages,
                const int *node,
                int *status,
                int flags);

/* Attempts to move all pages of a process in specified nodes to specified new
 * memory nodes.
 */
int migrate_pages(int pid,
                  unsigned long maxnode,
                  const unsigned long *old_nodes,
                  const unsigned long *new_nodes);

/* get_numa_topology fills buff with a data structure indicated by selection.
 * There are 2 modes. NUMA_TOPOLOGY_CPUS fills buff with an array of bitmaps
 * representing the cpus in each NUMA node.  Indexes are NUMA node IDs.
 * NUMA_TOPOLOGY_WEIGHT fills buff with a 2 dimensional array. The indexes are
 * NUMA node IDs. Weight between two NUMA nodes can be found by accessing the
 * value at buff[a][b] where a and b are NUMA node IDs.
 */
int get_numa_topology(char *buff,
                      size_t length,
                      numa_topology_selection selection );


#endif /* __FREE_BSD_NUMA_H__ */
