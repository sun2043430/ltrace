#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/ptrace.h>
#include <asm/ptrace.h>

#if (!defined(PTRACE_PEEKUSER) && defined(PTRACE_PEEKUSR))
# define PTRACE_PEEKUSER PTRACE_PEEKUSR
#endif

#if (!defined(PTRACE_POKEUSER) && defined(PTRACE_POKEUSR))
# define PTRACE_POKEUSER PTRACE_POKEUSR
#endif

void *
get_instruction_pointer(pid_t pid) {
	return (void *)ptrace(PTRACE_PEEKUSER, pid, 4*PT_NIP, 0);
}

void
set_instruction_pointer(pid_t pid, long addr) {
	ptrace(PTRACE_POKEUSER, pid, 4*PT_NIP, addr);
}

void *
get_stack_pointer(pid_t pid) {
	return (void *)ptrace(PTRACE_PEEKUSER, pid, 4*PT_R1, 0);
}

void *
get_return_addr(pid_t pid, void * stack_pointer) {
	return (void *)ptrace(PTRACE_PEEKTEXT, pid, 4*PT_LNK, 0);
}