#include <string> 
#include <sstream>
#include <iostream> 
#include <list>
#include "parser.hpp"

#define MAXTIME 20
#define CAPACITY 10
#define PRIORITY_RANGE 2

std::list<Job> active_job_lists[PRIORITY_RANGE];
std::list<Job> inactive_job_lists[PRIORITY_RANGE];

int calc_exec_point(){
    int exec_point = 0;
    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(const auto& job : active_job_lists[i]){
            exec_point += job.remaining_point;
        }
    }
    return exec_point;
}

void activate_jobs(int spare_point){
    for(int i = PRIORITY_RANGE - 1; i >= 0; i--){
        for(auto it = inactive_job_lists[i].begin(); it != inactive_job_lists[i].end(); ++it){
            if(it->remaining_point <= spare_point){
                spare_point -= it->remaining_point;
                active_job_lists[i].push_back(*it);
                it = inactive_job_lists[i].erase(it);
            }
        }
    }
}

void update_jobs(){
    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(auto it = active_job_lists[i].begin(); it != active_job_lists[i].end();){
            auto res = it->update_task();
            if(res == TASK_FINISHED){
                inactive_job_lists[i].push_back(*it);
                it = active_job_lists[i].erase(it);
            }
            else if(res == JOB_FINISHED){
                it = active_job_lists[i].erase(it);
            }else{
                ++it;
            }
        }
    }
}

void add_job_to_inactive_job_list(int t){
    std::list<Job> new_jobs = get_and_parse_json(t);
    for(const auto& new_job : new_jobs){
        inactive_job_lists[new_job.priority].push_back(new_job);
    }
}

void print_job_list(){
    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(auto& j : active_job_lists[i]){
            std::cout << "active" << i << j.remaining_point << std::endl;
        }
    }
    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(auto& j : inactive_job_lists[i]){
            std::cout << "inactive" << i << j.remaining_point << std::endl;
        }
    }
}

int main(){
    for(int i = 0; i <= MAXTIME; i++){
        update_jobs();
        add_job_to_inactive_job_list(i);
        activate_jobs(CAPACITY - calc_exec_point());
        std::cout << calc_exec_point() << std::endl;
        //print_job_list();
    }

    return 0;
}

