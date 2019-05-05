#include "update_active_jobs.hpp"

extern int CAPACITY;

extern std::vector <std::list<Job>> active_job_lists;
extern std::vector <std::list<Job>> inactive_job_lists;

void update_active_jobs(){
    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(auto it = active_job_lists[i].begin(); it != active_job_lists[i].end();){
            auto res = it->update_task();
            if(res == JOB_FINISHED){
                it = active_job_lists[i].erase(it);
            }else if(res == TASK_FINISHED){
                if (CAPACITY == -1){
                    // if there exists no capacity, the next task continues to run
                    ++it;
                }else{
                    inactive_job_lists[i].push_back(*it);
                    it = active_job_lists[i].erase(it);
                }
            }else{
                ++it;
            }
        }
    }
}

