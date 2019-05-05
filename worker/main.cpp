#include <string> 
#include <sstream>
#include <iostream> 
#include <list>
#include <vector>
#include <array>
#include "parser.hpp"
#include "init.hpp"

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

int find_time_to_begin_task(const std::vector<int>& spare_points_list, int task){
    std::vector<int> executable_points_list(spare_points_list.size());
    std::copy(spare_points_list.begin(), spare_points_list.end(), executable_points_list.begin());

    for(int i = executable_points_list.size() - 2; 0 <= i; i--){
        executable_points_list[i] = std::min(executable_points_list[i], executable_points_list[i + 1] + 1);
    }

    int begin_time = -1;
    for(int i = 0; i < executable_points_list.size(); i++){
        if(task <= executable_points_list[i]){
            begin_time = i;
            break;
        }
    }
    return begin_time;
}

void update_spare_points_list(std::vector<int>& spare_points_list, int begin_time, int point){
    for(int i = 0; begin_time + i < spare_points_list.size() && i < point; i++){
        spare_points_list[begin_time + i] -= point - i;
    }
}

void update_spare_points_list_by_all_active_jobs(std::vector<int>& spare_points_list){
    for(int i = 0; i < spare_points_list.size(); i++){
        spare_points_list[i] = CAPACITY;
    }

    for(int i = 0; i < PRIORITY_RANGE; i++){
        for(const auto& job : active_job_lists[i]){
            update_spare_points_list(spare_points_list, 0, job.remaining_point);
        }
    }
}


void activate_jobs(){
    int predict_size = CAPACITY + 1;
    std::vector<int> spare_points_list(predict_size);
    update_spare_points_list_by_all_active_jobs(spare_points_list);

    for(int j = PRIORITY_RANGE - 1; 0 <= j; j--){
        for(auto it = inactive_job_lists[j].begin(); it != inactive_job_lists[j].end(); ){
            int begin_time = find_time_to_begin_task(spare_points_list, it->remaining_point);
            if(begin_time == -1){
                ++it;
                continue;
            }
            update_spare_points_list(spare_points_list, begin_time, it->remaining_point);

            if(begin_time == 0){
                active_job_lists[j].push_back(*it);
                it = inactive_job_lists[j].erase(it);
            }else{
                ++it;
            }

            if(spare_points_list[0] == 0){
                goto FIN;
            }
        }
    }

FIN:
    return;
}

// This is NAIVE implementation of activate_jobs. Tasks are not executed when any higher priority tasks exists.
void activate_jobs_naively(){
    int spare_point = CAPACITY - calc_exec_point();
    for(int j = PRIORITY_RANGE - 1; j >= 0; j--){
        for(auto it = inactive_job_lists[j].begin(); it != inactive_job_lists[j].end(); ++it){
            if(it->remaining_point <= spare_point){
                spare_point -= it->remaining_point;
                active_job_lists[j].push_back(*it);
                it = inactive_job_lists[j].erase(it);
            }
        }
        if(inactive_job_lists[j].size()){
            return;
        }
    }
    return;
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
                activate_jobs_naively();
            }else{
                activate_jobs();
            }
            print_exec_point();
        }
    }

    return 0;
}

