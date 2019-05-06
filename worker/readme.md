## Worker

```
$ make
$ MAXTIME=20 CAPACITY=15 PRIORITY_RANGE=2 NAIVE=0 ./worker
```

### Output
- `stdout`: The time [s] and Executing Point
- `stderr`: The priority of the job and the waiting time of the job calculated by `Endtime - Starttime - Sum(tasks)`

### Global Variables
- `MAXTIME`: The worker requests for the server from 0 [sec] to MAXTIME [sec] with 1 [sec] intervals
- `CAPACITY`: The upper limit of Executing Point. When no limit exists, you assign -1 to CAPACITY 
- `PRIORITY_RANGE`: The number of types of priorities. If you want to ignore priorities, you assign 1 to PRIORITY_RANGE
- `NAIVE`: When you execute the worker with this flag, the worker schedules tasks naively (inefficiently). This is a flag used for assignment 2-2
- `active_job_lists[priority]`: The list of jobs being executed with specific priority
- `inactive_job_lists[priority]`: The list of jobs being not executed with specific priority

