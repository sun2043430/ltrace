#include "config.h"

#if defined(HAVE_LIBUNWIND)
#include <libunwind.h>
#include <libunwind-ptrace.h>
#endif /* defined(HAVE_LIBUNWIND) */

#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#include "common.h"

Process *
open_program(char *filename, pid_t pid, int enable) {
	Process *proc;
	assert(pid != 0);
	proc = calloc(sizeof(Process), 1);
	if (!proc) {
		perror("malloc");
		exit(1);
	}
	proc->filename = strdup(filename);
	proc->breakpoints_enabled = -1;
	proc->pid = pid;
#if defined(HAVE_LIBUNWIND)
	proc->unwind_priv = _UPT_create(pid);
	proc->unwind_as = unw_create_addr_space(&_UPT_accessors, 0);
#endif /* defined(HAVE_LIBUNWIND) */


	proc->next = list_of_processes;
	list_of_processes = proc;
	breakpoints_init(proc, enable);

	return proc;
}

void
open_pid(pid_t pid) {
	Process *proc;
	char *filename;

	if (trace_pid(pid) < 0) {
		fprintf(stderr, "Cannot attach to pid %u: %s\n", pid,
			strerror(errno));
		return;
	}

	filename = pid2name(pid);

	if (!filename) {
		fprintf(stderr, "Cannot trace pid %u: %s\n", pid,
				strerror(errno));
		return;
	}

	proc = open_program(filename, pid, 1);
	continue_process(pid);
	proc->breakpoints_enabled = 1;
}

Process *
pid2proc(pid_t pid) {
	Process *tmp;

	tmp = list_of_processes;
	while (tmp) {
		if (pid == tmp->pid) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}
