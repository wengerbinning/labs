POSIX Thread




## APIs

* `pthread.h`

### Thread managerment APIs

* pthread_t

```c
#include <bits/pthreadtypes.h>

/* Thread identifiers.  The structure of the attribute type is not
   exposed on purpose.  */
typedef unsigned long int pthread_t;

/* Keys for thread-specific data */
typedef unsigned int pthread_key_t;
```


* pthread_create

```c
#include <pthread.h>
/* Create a new thread, starting with execution of START-ROUTINE
   getting passed ARG.  Creation attributed come from ATTR.  The new
   handle is stored in *THREAD.  */
int pthread_create (pthread_t *__thread, const pthread_attr_t *__attr,
    void *(*__start_routine) (void *), void *__arg);
```

* pthread_join

```c
/* Make calling thread wait for termination of the thread TH.  The
   exit status of the thread is stored in *THREAD_RETURN, if THREAD_RETURN
   is not NULL.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
int pthread_join (pthread_t __th, void **__thread_return)
```

* pthread_detach

```c
/* Indicate that the thread TH is never to be joined with PTHREAD_JOIN.
   The resources of TH will therefore be freed immediately when it
   terminates, instead of waiting for another thread to perform PTHREAD_JOIN
   on it.  */
int pthread_detach (pthread_t __th)
```

* pthread_equal

```c
/* Compare two thread identifiers.  */
int pthread_equal (pthread_t __thread1, pthread_t __thread2)
```

* pthread_cancel

```c
/* Cancel THREAD immediately or at the next possibility.  */
int pthread_cancel (pthread_t __th);
```

### Thread Self APIs

* pthread_exit

```c
int pthread_yield (void)
```

* pthread_exit

```c
/* Terminate calling thread.
   The registered cleanup handlers are called via exception handling
   so we cannot mark this function with __THROW.*/
void pthread_exit (void *__retval)
```

```c
extern pthread_t pthread_self
```




### Thread mutex APIs

```c
#include <pthread.h>
```

```c
int pthread_mutex_init (pthread_mutex_t *__mutex,
			       const pthread_mutexattr_t *__mutexattr)
```

```c
int pthread_mutex_destroy (pthread_mutex_t *__mutex)
```

```c
int pthread_mutex_trylock (pthread_mutex_t *__mutex)
```

```c
int pthread_mutex_lock (pthread_mutex_t *__mutex)
```


```c
int pthread_mutex_unlock (pthread_mutex_t *__mutex)
```

### Thread rwlock APIs

```c
int pthread_rwlock_init (pthread_rwlock_t *__restrict __rwlock,
				const pthread_rwlockattr_t *__restrict
				__attr)
```

```c
int pthread_rwlock_destroy (pthread_rwlock_t *__rwlock)
```

```c
int pthread_rwlock_rdlock (pthread_rwlock_t *__rwlock)
```

```c
int pthread_rwlock_tryrdlock (pthread_rwlock_t *__rwlock)
```

### Thread spin-lock APIs

### Thread barrier APIs


线程同步




## mutex

pthread_mutex_init
pthread_mutex_lock
pthread_mutex_trylock
pthread_mutex_unlock
pthread_mutex_destroy

## cond


pthread_cond_init
pthread_cond_wait
pthread_cond_timewait
pthread_cond_signal
pthread_cond_broadcast
pthread_cond_destroy

## semaphore


sem_init
sem_post
sem_wait
sem_destroy


## 读写锁

pthread_rwlock_init
pthread_rwlock_rdlock
pthread_rwlock_tryrdlock
pthread_rwlock_wrlock
pthread_rwlock_trywrlock
pthread_rwlock_unlock
pthread_rwlock_timedrdlock
pthread_rwlock_timedwrlock
pthread_rwlock_destroy
























## Linux 任务调度器

Linux支持任务调度器

* stop scheduler
* deadline scheduler
* rt scheduler
* cfs scheduler
* idle scheduler


* 调度类
* 调度周期
* 运行时间
* 虚拟运行时间(vruntime):
* 实际运行时间(walltime):按照权重分配
* 调度延迟

## 任务优先级

* 实时进程优先级：0-99，值越大优先级越大
* 普通进程优先级：100-139，值越小优先级越大
* 权重(weight): 根据权重来分配CPU时间
* nice:[-20,19], 数值越小优先级越大




#### stop scheduler

停机调度器，
可以抢占其他所有进程，不能被其他进程抢断，用于紧急任务。
每一个CPU只有一个stop任务

* softlockup
* cpu hotplug
* rcu

#### deadline scheduler

期限调度器
使用红黑树将进程按照绝对截至日期进行排序，
任务存在三个调度参数：运行周期、运行时间、截至日期；

#### runtime scheduler

实时调度器
存在0～99的优先等级，数字越大优先级越高
每一个优先级存在一个队列
调度策略： SCHED_RR， SCHED_FIFO

#### CFS scheduler

完全公平调度器，完全公平调度算法




调度策略： SCHED_NORMAL, SCHED_BATCH

#### IDLE scheduler

空闲调度器
每一个CPU存在一个idle线程， 没有其他线程时，调度该线程
优先级最低的线程

调度策略： SCHED_IDLE


## Linux 任务调度策略

* SCHED_RR时间片轮转调度策略：进程用完时间片后添加到当前优先级队列尾部
* SCHED_FIFO: 先进先出调度，无时间片，无更高优先级只能等待任务主动让出CPU.
* SCHED_NORMAL：s
* SCHED_BATCH: 批量处理，期望减少调度次数，每次调度执行时间长点
* SCHED_IDLE: 优先级特别低的任务


## 就绪队列(cpu runqueue)

每一个CPU都有一个队列

* cfs_rq
* rt_rq
* dl_rq



* <https://zhuanlan.zhihu.com/p/533319895>