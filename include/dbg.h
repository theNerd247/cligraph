// Zed Shaw's Debug macros. Pretty fancy, that.
// Comments by Ben Davenport-Ray.
#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

//--THREADS ------------------------------
#define lock(a) pthread_mutex_lock(&a)
#define unlock(a) pthread_mutex_unlock(&a)
//--END THREADS ---------------------------

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG (%s:%s:%d): " M "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#endif

//added by Noah Harvey
#ifdef NLOG

#define __DO_VOID__ (void)(0)
#define clean_errno()           __DO_VOID__
#define log_err(M, ...)         __DO_VOID__
#define log_warn(M, ...)        __DO_VOID__
#define log_info(M, ...)        __DO_VOID__
#define log_attempt(M, ...)     __DO_VOID__
#define log_success(M, ...)     __DO_VOID__
#define log_failure(M, ...)     __DO_VOID__

#else /* not NLOG */

// Say something if errno is flagged. 
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// Send an error to STDERR.
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
// Send a warning to STDERR.
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
// Send information to STDERR.
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

//added by Noah Harvey
// Send attempt info to STDERR ...
#define log_attempt(M, ...) fprintf(stderr, "[ATTEMPT] " M "...", ##__VA_ARGS__) 
// ...success! 
#define log_success(M, ...) fprintf(stderr,"success " M "\n",##__VA_ARGS__);
// ...failure! 
#define log_failure(M, ...) { fprintf(stderr,"failure: \n"); log_err(M, ##__VA_ARGS__); }

#endif 

// Check if the expression A is true. If not, say something and go to the error label.
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

//added by Noah Harvey
// Shorthand for check
#define check_expr(A, V, M, ...) check(((A) == (V)),M, ##__VA_ARGS__)

// Place the sentinel somewhere that the program should not go.
#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// Check if memory allocation was good.
#define check_mem(A) check((A), "Out of memory.")

//added by Noah Harvey
// Run something if an error occurs
#define error_run(A,C) if(!(A)) { C; goto error;}

#endif
