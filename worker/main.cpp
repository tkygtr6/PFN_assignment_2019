#include <string> 
#include <sstream>
#include <iostream> 
#include <list>
#include "parser.hpp"

#define MAXTIME 20
#define CAPACITY 10

std::list<Job> active_job_list;
std::list<Job> inactive_job_list;

int calc_exec_point(){
    int exec_point = 0;
    for(const auto& job : active_job_list){
        exec_point += job.remaining_point;
    }
    return exec_point;
}

void activate_jobs(int spare_point){
    for(auto it = inactive_job_list.begin(); it != inactive_job_list.end(); ++it){
        if(it->remaining_point <= spare_point){
            spare_point -= it->remaining_point;
            active_job_list.push_back(*it);
            it = inactive_job_list.erase(it);
        }
    }
}

void update_jobs(){
    for(auto it = active_job_list.begin(); it != active_job_list.end();){
        auto res = it->update_task();
        if(res == TASK_FINISHED){
            inactive_job_list.push_back(*it);
            it = active_job_list.erase(it);
        }
        else if(res == JOB_FINISHED){
            it = active_job_list.erase(it);
        }else{
            ++it;
        }
    }
}

void add_job_to_inactive_job_list(int t){
    std::list<Job> new_jobs = get_and_parse_json(t);
    for(const auto& new_job : new_jobs){
        inactive_job_list.push_back(new_job);
    }
}

void print_job_list(){
    for(auto& j : active_job_list){
        std::cout << "active" << j.remaining_point << std::endl;
    }
    for(auto& j : inactive_job_list){
        std::cout << "inactive" << j.remaining_point << std::endl;
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

