#include <string> 
#include <sstream>
#include <iostream> 
#include <list>
#include "parser.hpp"
#include "init.hpp"

int MAXTIME;
int CAPACITY;
int PRIORITY_RANGE;

std::vector <std::list<Job>> active_job_lists;
std::vector <std::list<Job>> inactive_job_lists;

void update_jobs_with_no_capacity(){
    // use for no capacity
    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(auto it = active_job_lists[i].begin(); it != active_job_lists[i].end();){
            auto res = it->update_task();
            if(res == JOB_FINISHED){
                it = active_job_lists[i].erase(it);
            }else{
                ++it;
            }
        }
    }
}

void add_job_to_active_job_list(int t){
    // use for no capacity
    std::list<Job> new_jobs = get_and_parse_json(t);
    for(const auto& new_job : new_jobs){
        active_job_lists[new_job.priority].push_back(new_job);
    }
}

void update_jobs(){
    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(auto it = active_job_lists[i].begin(); it != active_job_lists[i].end();){
            auto res = it->update_task();
            if(res == JOB_FINISHED){
                it = active_job_lists[i].erase(it);
            }else if(res == TASK_FINISHED){
                inactive_job_lists[i].push_back(*it);
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

int calc_exec_point(){
    int exec_point = 0;
    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(const auto& job : active_job_lists[i]){
            exec_point += job.remaining_point;
        }
    }
    return exec_point;
}

void print_exec_point(int t){
    std::cout << t << "\t" << calc_exec_point() << std::endl;
}

int main(){
    env_init();

    if(CAPACITY == -1){
        // no capacity, only use active_job_list
        for(int t = 0; t <= MAXTIME; t++){
            update_jobs_with_no_capacity();
            add_job_to_active_job_list(t);
            print_exec_point(t);
        }
    }else{
        for(int t = 0; t <= MAXTIME; t++){
            update_jobs();
            add_job_to_inactive_job_list(t);
            activate_jobs(CAPACITY - calc_exec_point());
            print_exec_point(t);
        }
    }

    return 0;
}

