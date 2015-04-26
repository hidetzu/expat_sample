#ifndef __COMMON_CAPI_H__
#define __COMMON_CAPI_H__

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

#ifndef UNITTEST_ENABLE
#define common_malloc malloc
#define common_free   free
#define common_memcpy memcpy
#define common_memset memset
#define common_fopen fopen
#define common_fread fread
#define common_fclose fclose
#define common_strcmp strcmp
#define common_feof feof
#define common_strlen strlen
#define common_pthread_mutex_init pthread_mutex_init 
#define common_pthread_mutex_destroy pthread_mutex_destroy 
#define common_pthread_cond_init pthread_cond_init
#define common_pthread_cond_destroy pthread_cond_destroy
#define common_pthread_mutex_lock pthread_mutex_lock
#define common_pthread_mutex_unlock pthread_mutex_unlock
#define common_pthread_cond_signal pthread_cond_signal
#define common_pthread_cond_wait pthread_cond_wait
#define common_pthread_create pthread_create
#define common_pthread_join pthread_join
#define common_atoi atoi

#else
void* unittest_malloc(size_t size);
void unittest_free(void* ptr);
int unittest_pthread_mutex_init(pthread_mutex_t* mutex,
        const pthread_mutexattr_t* mutexattr);
int unittest_pthread_mutex_destroy(pthread_mutex_t* mutex);
int unittest_pthread_cond_init(pthread_cond_t *cond,
    const pthread_condattr_t *attr);
int unittest_pthread_cond_destroy(pthread_cond_t *cond);
int unittest_pthread_mutex_lock(pthread_mutex_t *mutex);
int unittest_pthread_mutex_unlock(pthread_mutex_t *mutex);
int unittest_pthread_cond_signal(pthread_cond_t *cond);
int unittest_pthread_cond_wait(pthread_cond_t *cond,
    pthread_mutex_t *mutex);
int unittest_pthread_create(pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine)(void *),
    void *arg);
int unittest_pthread_join(pthread_t thread,
        void **value_ptr);


#define common_malloc unittest_malloc
#define common_free   unittest_free
#define common_pthread_mutex_init unittest_pthread_mutex_init 
#define common_pthread_mutex_destroy unittest_pthread_mutex_destroy 
#define common_pthread_cond_init unittest_pthread_cond_init
#define common_pthread_cond_destroy unittest_pthread_cond_destroy
#define common_pthread_mutex_lock unittest_pthread_mutex_lock
#define common_pthread_mutex_unlock unittest_pthread_mutex_unlock
#define common_pthread_cond_signal unittest_pthread_cond_signal
#define common_pthread_cond_wait unittest_pthread_cond_wait
#define common_pthread_create unittest_pthread_create
#define common_pthread_join unittest_pthread_join

#endif

#endif  /*__COMMON_CAPI_H__*/
