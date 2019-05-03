from flask import Flask, jsonify
import os

DATA_DIR="./data"
jobs = []

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
            #job["Priority"] = 0 if str(lines[7].rstrip("\n")) == "Low" else 1
            job["Priority"] = lines[7].rstrip("\n")
            job["Tasks"] = list(map(lambda s: int(s), lines[10:]))
    except:
        job = None
    return job

def read_data():
    files = os.listdir(DATA_DIR)
    for file in files:
        if not file[-4:] == ".job":
            continue
        job = parse_job(os.path.join(DATA_DIR, file))
        if job:
            jobs.append(job)
    jobs.sort(key=lambda job:job["Created"])

def search_job(ts):
    # todo bin search
    created = ts2int(ts)
    target_jobs = []
    for job in jobs:
        if job["Created"] == created:
            target_jobs.append(job)
    return target_jobs

app = Flask(__name__)
read_data()

@app.route("/jobs/<ts>", methods=['GET'])
def get_jobs(ts):
    return jsonify({
        "Jobs": search_job(ts)
    })

