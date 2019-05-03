#include <string> 
#include <sstream>
#include <iostream> 
#include <vector>
#include "parser.hpp"

std::vector<Job> job_list;

void calc_exec_point(){
    int exec_point = 0;
    for(const auto& job : job_list){
        exec_point += job.remaining_point;
    }
    std::cout << exec_point << std::endl;
}

void remove_job(int job_id){
    for(auto it = job_list.begin(); it != job_list.end(); ++it){
        if(it->job_id == job_id){
            job_list.erase(it);
        }
        return;
    }
}

void update_jobs(){
    std::vector<int> finish_job_ids;
    for(auto& job : job_list){
        auto res = job.update_task();
        if(res.first){
            finish_job_ids.push_back(res.second);
        }
    }
    for(auto& finish_job_id : finish_job_ids){
        remove_job(finish_job_id);
    }
}

int main(){
    for(int i = 0; i <= 10000; i++){
        std::vector<Job> new_jobs = get_and_parse_json(i);
        for(const auto& new_job : new_jobs){
            job_list.push_back(new_job);
        }
        update_jobs();
        calc_exec_point();
    }

    return 0;
}


