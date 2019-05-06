## Server

```
$ DATA_DIR=../test/data_test1 FLASK_APP=server.py flask run &
$ curl http://localhost:5000/jobs/00:00:03

{"Jobs":[{"Created":3,"JobID":2,"Priority":1,"Tasks":[3,5]}]}
```

### Unit Test

```
$ DATA_DIR=./data FLASK_APP=server.py flask run &
python test.py
```
