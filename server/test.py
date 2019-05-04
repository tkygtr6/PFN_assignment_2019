import unittest
import requests
import subprocess
import random
from server import ts2int, int2ts

URL_base = "http://localhost:5000/jobs/"
DATA_DIR = "./data"

class TestServer(unittest.TestCase):
    def test_samples(self):
        res = requests.get(URL_base + "00:32:24").json()
        self.assertEqual(len(res["Jobs"]), 1)
        self.assertEqual(res["Jobs"][0]["Created"], ts2int("00:32:24"))
        self.assertEqual(res["Jobs"][0]["JobID"], 499)
        self.assertEqual(res["Jobs"][0]["Priority"], 0)
        self.assertEqual(len(res["Jobs"][0]["Tasks"]), 3)
        self.assertEqual(res["Jobs"][0]["Tasks"][0], 1)
        self.assertEqual(res["Jobs"][0]["Tasks"][1], 4)
        self.assertEqual(res["Jobs"][0]["Tasks"][2], 3)

        res = requests.get(URL_base + "00:38:45").json()
        self.assertEqual(len(res["Jobs"]), 1)
        self.assertEqual(res["Jobs"][0]["Created"], ts2int("00:38:45"))
        self.assertEqual(res["Jobs"][0]["JobID"], 599)
        self.assertEqual(res["Jobs"][0]["Priority"], 0)
        self.assertEqual(len(res["Jobs"][0]["Tasks"]), 2)
        self.assertEqual(res["Jobs"][0]["Tasks"][0], 3)
        self.assertEqual(res["Jobs"][0]["Tasks"][1], 4)

        res = requests.get(URL_base + "00:45:10").json()
        self.assertEqual(len(res["Jobs"]), 1)
        self.assertEqual(res["Jobs"][0]["Created"], ts2int("00:45:10"))
        self.assertEqual(res["Jobs"][0]["JobID"], 699)
        self.assertEqual(res["Jobs"][0]["Priority"], 1)
        self.assertEqual(len(res["Jobs"][0]["Tasks"]), 4)
        self.assertEqual(res["Jobs"][0]["Tasks"][0], 3)
        self.assertEqual(res["Jobs"][0]["Tasks"][1], 5)
        self.assertEqual(res["Jobs"][0]["Tasks"][2], 5)
        self.assertEqual(res["Jobs"][0]["Tasks"][3], 4)

    def test_num_jobs(self):
        for _ in range(50):
            t = random.randint(0,1000)
            res = requests.get(URL_base + int2ts(t)).json()
            num_jobs = len(res["Jobs"])
            num_jobs_ans = int(subprocess.check_output("grep -r {} {} | wc -l".format(int2ts(t), DATA_DIR), shell=True))
            self.assertEqual(num_jobs, num_jobs_ans)

if __name__ == "__main__":
    unittest.main()
