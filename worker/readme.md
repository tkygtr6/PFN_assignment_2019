## Worker

```
make
MAXTIME=20 CAPACITY=15 PRIORITY_RANGE=2 NAIVE=0 ./worker
```

- `stdout`: The time [s] and Executing Point
- `stderr`: The priority of the job and the waiting time of the job calculated by `Endtime - Starttime - Sum(tasks)`
