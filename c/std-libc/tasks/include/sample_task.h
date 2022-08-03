/****************************************************************************
* *
* * FILENAME:        $RCSfile: sample_task.h,v $
* *
* * LAST REVISION:   $Revision: 1.0 $
* * LAST MODIFIED:   $Date: 2024/11/22
* *
* * DESCRIPTION:     Sample Task API
* *
* *
* * Copyright (c) 2024 by Grandstream Networks, Inc.
* * All rights reserved.
* *
* * This material is proprietary to Grandstream Networks, Inc. and,
* * in addition to the above mentioned Copyright, may be
* * subject to protection under other intellectual property
* * regimes, including patents, trade secrets, designs and/or
* *
* * Any use of this material for any purpose, except with an
* * express license from Grandstream Networks, Inc. is strictly
* * prohibited.
* *
* ***************************************************************************/
#ifndef __SAMPLE_TASK_H__
#define __SAMPLE_TASK_H__

// ================
//  Includes
// ================

#include <time.h>
#include <string.h>

// ================
//  Typedefs
// ================

struct sample_task;

enum {
    TASK_UNKNOWN,
    TASK_RUNNING,
    TASK_EXIT,
    TASK_STATE_MAX
};

struct sample_task_ops {
    int (*start)  (struct sample_task *task, void *priv);
    int (*check)  (struct sample_task *task);
    void (*kill)  (struct sample_task *task);
    int (*finish) (struct sample_task *task, void *arg);
};

struct sample_task {
    int pid;
    time_t timestamp;
    int state;

    void *priv;
    struct sample_task_ops *ops;
    int ret;
};


// ================
//  Functions
// ================

static inline int sample_task_init (struct sample_task *task,
    struct sample_task_ops *ops)
{
    memset(task, 0, sizeof(struct sample_task));
    task->pid = -1;
    task->ops = ops;
    return 0;
}

static inline int sample_task_start (struct sample_task *task, void *priv) {
    if (task && task->ops && task->ops->start ) {
        return task->ops->start(task, priv);
    }
    return TASK_UNKNOWN;
}

static inline int sample_task_check (struct sample_task *task) {
    if (task && task->ops && task->ops->check ) {
        return task->ops->check(task);
    }
    return TASK_UNKNOWN;
}

static inline void sample_task_kill (struct sample_task *task) {
    if (task && task->ops && task->ops->kill ) {
        task->ops->kill(task);
    }
}

static inline int sample_task_finish (struct sample_task *task) {
    if (task && task->ops && task->ops->finish ) {
        return task->ops->finish(task);
    }

    return 0;
}



#endif /* __SAMPLE_TASK_H__ */