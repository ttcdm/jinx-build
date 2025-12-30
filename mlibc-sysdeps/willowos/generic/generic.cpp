//taken from nyaux's
#include "syscall.h"
// #include <../../../sysdeps/willowos/gener/ic/syscall.h>
// #include </home/titus/willowOS/limine-c-template-x86-64/mlibc/sysdeps/willowos/generic/syscall.h>
// #include <asm/ioctls.h>
#include <bits/ensure.h>
// #include <bits/syscall.h>
// #include <dirent.h>
#include <errno.h>
// #include <fcntl.h>
#include <limits.h>
// #include <mlibc/all-sysdeps.hpp>
#include <mlibc/ansi-sysdeps.hpp>
#include <mlibc/debug.hpp>
// #include <syscall.h>
#include <stdint.h>
#include <stdlib.h>
// #include <sys/select.h>
// #include <sys/types.h>


//HERE
namespace mlibc {

int syscall_log(char* fmt, ...) {//15
    int ret;
    size_t num = 15;

    //not sure if we need to cli here
    va_list args;
    va_start(args, fmt);

    struct fmt_args args_struct = {
        .fmt = fmt,
    };

    va_copy(args_struct.args, args);

    asm volatile ("syscall" : "=a"(ret) : "D"(num), "S"(&args_struct) : "memory");
    return ret;
}


int syscall_test() {
}


int syscall_yield() {//6
    int ret;
    uint64_t num = 6;
    asm volatile("syscall" : "=a"(ret): "D"(num) : "memory");
    return ret;
}

int syscall_user_thread_exit() {//9
    int ret;
    uint64_t num = 9;
    asm volatile("syscall" : "=a"(ret): "D"(num) : "memory");
    return ret;
}



int sys_futex_wait(int *pointer, int expected, const struct timespec *time) {//11
    //we ignore time arg for now
    int ret;
    uint64_t num = 11;
    asm volatile("syscall" : "=a"(ret): "D"(num), "S"(pointer), "d"(expected): "memory");
    return ret;
}
int sys_futex_wake(int *pointer) {//12
    int ret;
    uint64_t num = 12;
    asm volatile ("syscall" : "=a"(ret) : "D"(num), "S"(pointer) : "memory");
    return ret;
}



int sys_anon_allocate(size_t size, void **pointer) {//13
    int ret;
    uint64_t num = 13;

    sys_libc_log("HERE sys_anon_allocate called\n");

    asm volatile("syscall" : "=a"(ret): "D"(num), "S"(size), "d"(pointer): "memory");
    return ret;
}
int sys_anon_free(void *pointer, size_t size) {//14
    //i think we can disregard size for now
    int ret;
    uint64_t num = 14;
    asm volatile ("syscall" : "=a"(ret) : "D"(num), "S"(pointer) : "memory");
    return ret;
}


int sys_vm_map(void *hint, size_t size, int prot, int flags, int fd, int64_t offset, void **window) {//7
    int ret;
    uint64_t num = 7;

    syscall_log("\n\nsyscall_log and sys_vm_map called\n\n");
    sys_libc_log("HERE sys_vm_map called\n");

    //we don't have permissions for file access yet so we're just gonna ignore prot

    // int64_t w_flags = (int64_t) flags;//wide flags

    //masking and extracting only a single bit https://stackoverflow.com/questions/2249731/how-do-i-get-bit-by-bit-data-from-an-integer-value-in-c

    //https://pubs.opengroup.org/onlinepubs/9799919799/
    //mmap

    //https://chuck.cranor.org/p/diss.pdf


    //HERE technically this can really only be called when scheduling is started because scheduling is more or less always enabled and we don't call this outright in userspace by just jumping to something without creating a thread
    //make it so that scheduling must be started for this to function
    // if (!scheduling_started) {
    //     // kprintf("error: scheduling not started\n");
    //     return EPERM;
    // }



    uint64_t perm = 0;
    //not sure about PROT_NONE
    //always remember to change every value
    //maybe use a mask instead of shifts
    if (((prot & ( 1 << 0 )) >> 0) == 1) {//PROT_READ
        perm &= 0 << 1;
    }
    else {
        perm |= ~(1 << 0);//PROT_NONE. should be correct
        //move down so we don't need a goto??
    }
    if (((prot & ( 1 << 1 )) >> 1) == 1) {//PROT_WRITE
        perm |= 1 << 1;
    }
    if (((prot & ( 1 << 2 )) >> 2) == 0) {//PROT_EXEC
        perm |= 1ULL << 63;//no execute
    }
    //we have to use 1ULL because 1 defaults to int which is 16 or 32 bits wide i don't remember 
    //HERE remember to cast 1 to a ull and maybe have a constant(s?).h to have macros for it as well


    // if (((flags & ( 1 << 0 )) >> 0) == MAP_SHARED) {
    //     // map_page((uint64_t*) get_current_thread()->cr3, hint, hint, perm);
    // }
    // else if (((flags & ( 1 << 0 )) >> 0) == MAP_PRIVATE) {

    // }


    int map_page_ret;

    struct map_page_bytes_args mmap_args = {//make sure that this persists throughout the actual syscaLL
            // .cr3 = (uint64_t*) get_current_thread()->cr3,//this is set inside the syscall function
            // .phys_address = (uint64_t) region_start,//this is also set inside the syscall function
            .virt_address = (uint64_t) hint,
            .permissions = perm,
            .size = size,
            .error = 0,
            .ret = NULL
    };

    // //the issue is because we're extracting instead of masking
    // kprintf("\n%llx\n", flags);
    // kprintf("\n%llx ", (flags & (0xF << (4 * 1))));//chatgpt'd more or less
    // while (1);

    // kprintf("%llx\n", MAP_ANON);

    // kprintf("%b\n", (flags & (0xF << (4 * 1))) == (int) MAP_ANON);
    
    if ((int) (flags & (0xF << (4 * 1))) == (int) MAP_ANON) {
        ////HERE figure out if we're gonna use hint regardless or our own thing
        //HERE remember to unmap if it's already mapped?? smth about overlaps

        //we wall map_page_bytes behind a syscall here
        //we can just shove all the args into a struct because we don't have enough syscall args for all of the args here

        if ((int) (flags & (0xF << (4 * 0))) == (int) MAP_SHARED) {
            //remember to change hint as well if necessary
            mmap_args.flag = MAP_SHARED;
        }
        else if ((int) (flags & (0xF << (4 * 0))) == (int) MAP_PRIVATE) {
            mmap_args.flag = MAP_PRIVATE;
        }
        //it should be fine passing the struct that's on the stack since the branch hasn't exited yet
        asm volatile("syscall" : "=a"(map_page_ret): "D"(num), "S"(&mmap_args): "memory");
        // assert(mmap_args.error == 0);
        //because we can't have printf here i guess
        if (mmap_args.error != 0) {
            return mmap_args.error;
        }
        // memset(hint, 0, size);

        *window = hint;
        ret = 0;
    }
    else if ((int) (flags & (0xF << (4 * 1))) == (int) MAP_FIXED) {
        
        //HERE remember to unmap if it's already mapped?? smth about overlaps
        if ((int) (flags & (0xF << (4 * 0))) == (int) MAP_SHARED) {
            mmap_args.flag = MAP_SHARED;
        }
        else if ((int) (flags & (0xF << (4 * 0))) == (int) MAP_PRIVATE) {
            mmap_args.flag = MAP_PRIVATE;
        }
        asm volatile("syscall" : "=a"(map_page_ret): "D"(num), "S"(&mmap_args): "memory");
        // assert(mmap_args.error == 0);
        if (mmap_args.error != 0) {
            return mmap_args.error;
        }
        // memset(hint, 0, size);
        *window = hint;
        ret = 0;
    }

    // always use explicit if's to make sure that we're getting the exact value and not using else's cuz we're lazy
    
    // switch ((flags & ( 1 << 0 )) >> 0) {//extracting the 0th bit
    //     case MAP_SHARED:

    //         break;
    //     case MAP_PRIVATE:

    //         break;
    //     case MAP_FIXED:

    //         break;
    //     case MAP_ANON:

    //         break;
    //     // case MAP_ANONYMOUS://MAP_ANONYMOUS is the same as MAP_ANON

    //     //     break;
    // }

    return ret;
}
int sys_vm_unmap(void *pointer, size_t size) {//8
    int ret;
    uint64_t num = 8;
    asm volatile("syscall" : "=a"(ret): "D"(num), "S"(pointer), "d"(size): "memory");
}

//might be a time_t instead of uint64_t thing that was preventing mlibc from building
//HERE remember to fill these in
//also we define time_t as int64_t inside willowOS but here idk if we should
int sys_clock_get(int clock, time_t *secs, long *nanos) {
    int ret;
    //HERE add num as well
}

int sys_tcb_set(void *pointer) {
    int ret;
}

int sys_open(const char *pathname, int flags, mode_t mode, int *fd) {
    sys_libc_log("sys_open called\n");
    return 0;
}

int sys_read(int fd, void *buf, size_t count, ssize_t *bytes_read) {
    return 0;
}

int sys_write(int fd, const void *buf, size_t count, ssize_t *bytes_written) {
    syscall_log((char*) buf);
    return 0;
}

int sys_seek(int fd, off_t offset, int whence, off_t *new_offset) {
    return 0;
}

int sys_close(int fd) {
    return 0;
}

void sys_libc_log(const char *message) {
    syscall_log((char*) message);
}

[[noreturn]] void sys_libc_panic() {
    while (1);
}

//HERE not sure if this is the correct implementation
[[noreturn]] void sys_exit(int status) {
    syscall_user_thread_exit();
    while (1);
}

int get_current_thread_syscall(thread_context** thread) {//16
    int ret;
    uint64_t num = 16;
    asm volatile("syscall" : "=a"(ret): "D"(num), "S"(thread): "memory");
    return ret;
}

int sys_isatty(int fd) {
    while (1);
}

//HERE remember namespace mlibc
}