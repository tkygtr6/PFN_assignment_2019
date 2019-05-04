from flask import Flask, jsonify
import os
import bisect

DEFAULT_DATA_DIR="./data"
DATA_DIR =  os.environ.get("DATA_DIR")
if DATA_DIR is None:
    DATA_DIR = DEFAULT_DATA_DIR

jobs = []
jobs_created = []

def ts2int(ts):
    ts = ts.split(":")
    return int(ts[0])*3600+int(ts[1])*60+int(ts[2])

def int2ts(i):
    return '{0:02d}'.format(int(i/3600)) + ":" + '{0:02d}'.format(((int(i/60))%60)) + ":" + '{0:02d}'.format(i%60)

def parse_job(file_path):
    try:
        with open(file_path) as f:
            lines = f.readlines()
            job = {}
            job["JobID"] = int(lines[1])
            job["Created"] = ts2int(lines[4].rstrip("\n"))
            job["Priority"] = 0 if str(lines[7].rstrip("\n")) == "Low" else 1
            job["Tasks"] = list(map(lambda s: int(s), lines[10:]))
    except:
        job = None
    return job

def read_data():
    files = os.listdir(DATA_DIR)
    for file in files:
        if not file.endswith(".job"):
            continue
        job = parse_job(os.path.join(DATA_DIR, file))
        if job:
            jobs.append(job)
            jobs_created.append(job["Created"])
    jobs.sort(key=lambda job:job["Created"])
    jobs_created.sort()

def search_job(ts):
    created = ts2int(ts)
    min_index = bisect.bisect_left(jobs_created, created)
    max_index = bisect.bisect_right(jobs_created, created)
    target_jobs = jobs[min_index:max_index]
    return target_jobs

app = Flask(__name__)
read_data()

@app.route("/jobs/<ts>", methods=['GET'])
def get_jobs(ts):
    return jsonify({
        "Jobs": search_job(ts)
    })

