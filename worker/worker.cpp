#include <string> 
#include <sstream>
#include <iostream> 
#include <iomanip>
#include <vector>
#include <curl/curl.h> 
#include "json11/json11.hpp"

std::string int2ts(int i){
    std::ostringstream ts;
    ts << std::setfill('0') << std::right << std::setw(2) << i / 3600;
    ts << ":";
    ts << std::setfill('0') << std::right << std::setw(2) << (i / 60) % 60;
    ts << ":";
    ts << std::setfill('0') << std::right << std::setw(2) << i % 60;
    return ts.str();
}

size_t callbackWrite(char *ptr, size_t size, size_t nmemb, std::string *stream){
    int dataLength = size * nmemb;
    stream->append(ptr, dataLength);
    return dataLength;
}

class Job{
    public:
        int created;
        int job_id;
        int priority;
        std::vector<int> tasks;
        int num_tasks;
        int task_no;
        int remaining_point;

        Job(json11::Json job_json){
            created = job_json["Created"].int_value();
            job_id = job_json["JobID"].int_value();
            priority = job_json["Priority"].int_value();
            for(const auto& task : job_json["Tasks"].array_items()){
                tasks.push_back(task.int_value());
            }
            num_tasks = tasks.size();
            task_no = 1; // begin from 1
            remaining_point = tasks[task_no - 1];
        }

        void update(){
            if (remaining_point > 1){
                remaining_point--;
            }else{
                if(task_no == num_tasks){
                    remaining_point = 0;
                    // remove
                }else{
                    task_no++;
                    remaining_point = tasks[task_no - 1];
                }
            }
        }
};

std::vector<Job> get_and_parse_json(int sec){
    CURL *curl;
    CURLcode ret;
    std::string url = "http://localhost:5000/jobs/" + int2ts(sec);
    std::string res;

    curl = curl_easy_init();
    if (curl == NULL) {
        std::cerr << "curl_easy_init() failed" << std::endl;
        std::vector<Job> jobs;
        return jobs;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackWrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
    ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (ret != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed." << std::endl;
        std::vector<Job> jobs;
        return jobs;
    }

    std::string json_err; // todo
    auto jobs_json = json11::Json::parse(res, json_err); 
    jobs_json = jobs_json["Jobs"];

    std::vector<Job> jobs;
    for(const auto& job_json : jobs_json.array_items()){
        Job job(job_json);
        jobs.push_back(job);
    }
    return jobs;
}

std::vector<Job> job_list;

void calc_exec_point(){
    int exec_point = 0;
    for(const auto& job : job_list){
        exec_point += job.remaining_point;
    }
    std::cout << exec_point << std::endl;
}

void update_jobs(){
    for(auto& job : job_list){
        job.update();
    }
}

int main(){
    for(int i = 0; i <= 10000; i++){
        std::vector<Job> new_jobs = get_and_parse_json(i);
        for(const auto& new_job : new_jobs){
            job_list.push_back(new_job);
        }
        calc_exec_point();
        update_jobs();
    }

    return 0;
}


