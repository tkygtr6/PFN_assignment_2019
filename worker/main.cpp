#include <string> 
#include <sstream>
#include <iostream> 
#include <list>
#include <vector>
#include <array>
#include "init.hpp"
#include "parser.hpp"
#include "update_active_jobs.hpp"
#include "add_new_jobs.hpp"
#include "activate_jobs.hpp"

int MAXTIME;
int CAPACITY;
int PRIORITY_RANGE;
int NAIVE;

std::vector <std::list<Job>> active_job_lists;
std::vector <std::list<Job>> inactive_job_lists;

int global_time;

int calc_exec_point(){
    int exec_point = 0;
    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(const auto& job : active_job_lists[i]){
            exec_point += job.remaining_point;
        }
    }
    return exec_point;
}

void print_exec_point(){
    std::cout << global_time << "\t" << calc_exec_point() << std::endl;
}

int main(){
    env_init();

    if(CAPACITY == -1){
    // In no capacity situation, inactive_job_lists is not used. Only active_job_lists is used.
        for(global_time = 0; global_time <= MAXTIME; global_time++){
            update_active_jobs();
            add_new_job_to_active_job_lists();
            print_exec_point();
        }
    }else{
        for(global_time = 0; global_time <= MAXTIME; global_time++){
            update_active_jobs();
            add_new_job_to_inactive_job_lists();
            if (NAIVE){
                activate_jobs_naively(CAPACITY - calc_exec_point());
            }else{
                activate_jobs();
            }
            print_exec_point();
        }
    }

    return 0;
}

