#include "add_new_jobs.hpp"

extern std::vector <std::list<Job>> active_job_lists;
extern std::vector <std::list<Job>> inactive_job_lists;

void add_new_job_to_inactive_job_lists(){
    std::list<Job> new_jobs = get_and_parse_json(global_time);
    for(const auto& new_job : new_jobs){
        inactive_job_lists[new_job.priority].push_back(new_job);
    }
}

// use for no capacity
void add_new_job_to_active_job_lists(){
    std::list<Job> new_jobs = get_and_parse_json(global_time);
    for(const auto& new_job : new_jobs){
        active_job_lists[new_job.priority].push_back(new_job);
    }
}
