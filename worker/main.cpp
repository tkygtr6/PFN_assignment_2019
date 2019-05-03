#include <string> 
#include <sstream>
#include <iostream> 
#include <list>
#include "parser.hpp"

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

int main(){
    for(int i = 0; i <= 10000; i++){
        update_jobs();
        std::list<Job> new_jobs = get_and_parse_json(i);
        for(const auto& new_job : new_jobs){
            job_list.push_back(new_job);
        }
        calc_exec_point();
    }

    return 0;
}

