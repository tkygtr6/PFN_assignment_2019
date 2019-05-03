#include <string> 
#include <sstream>
#include <iostream> 
#include <list>
#include "parser.hpp"

#define MAXTIME 100

std::list<Job> job_list;

void calc_exec_point(){
    int exec_point = 0;
    for(const auto& job : job_list){
        exec_point += job.remaining_point;
    }
    std::cout << exec_point << std::endl;
}

void update_jobs(){
    for(auto it = job_list.begin(); it != job_list.end(); ++it){
        auto res = it->update_task();
        if(!res){
            it = job_list.erase(it);
        }
    }
}

void add_job_to_job_list(int t){
    std::list<Job> new_jobs = get_and_parse_json(t);
    for(const auto& new_job : new_jobs){
        job_list.push_back(new_job);
    }
}

int main(){
    for(int i = 0; i <= MAXTIME; i++){
        update_jobs();
        add_job_to_job_list(i);
        calc_exec_point();
    }

    return 0;
}

