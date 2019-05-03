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

    // convert Json to Job
    std::vector<Job> jobs;
    for(const auto& job_json : jobs_json.array_items()){
        Job job;
        job.created = job_json["Created"].int_value();
        job.job_id = job_json["JobID"].int_value();
        job.priority = job_json["Priority"].int_value();
        for(const auto& task : job_json["Tasks"].array_items()){
            job.tasks.push_back(task.int_value());
        }
        jobs.push_back(job);
    }
    return jobs;
}

/*
class task{
    public:
        int task_point;
        int priority;
        int job_id;
        int job_r;
        int task_no;
};

std::vector<json11::Json> job_list;
std::vector<task> ready_queue;
*/

/*
void append_new_job(Job new_job){
    job_list.push_back(new_job);

    new_task = new_job[]
    ready_queue.push_back();
}
*/

int main(){
    for(int i = 0; i <= 1000; i++){
        std::vector<Job> new_jobs = get_and_parse_json(i);
        for(const auto& new_job : new_jobs){
            std::cout << i << " " << new_job.job_id << std::endl;
            //append_new_job(new_job);
        }
       
        /*
        update_exec_list();
        execute_task();
        */
    }

    return 0;
}


