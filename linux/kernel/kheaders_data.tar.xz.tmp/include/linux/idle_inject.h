/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __IDLE_INJECT_H__
#define __IDLE_INJECT_H__


struct idle_inject_device;

struct idle_inject_device *idle_inject_register(struct cpumask *cpumask);

void idle_inject_unregister(struct idle_inject_device *ii_dev);

int idle_inject_start(struct idle_inject_device *ii_dev);

void idle_inject_stop(struct idle_inject_device *ii_dev);

void idle_inject_set_duration(struct idle_inject_device *ii_dev,
				 unsigned int run_duration_ms,
				 unsigned int idle_duration_ms);

void idle_inject_get_duration(struct idle_inject_device *ii_dev,
				 unsigned int *run_duration_ms,
				 unsigned int *idle_duration_ms);
#endif 
