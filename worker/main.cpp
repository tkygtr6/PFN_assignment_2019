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

std::vector <std::list<Job>> active_job_lists;
std::vector <std::list<Job>> inactive_job_lists;

void update_active_jobs(){
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

void add_job_to_inactive_job_lists(int t){
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

int find_executable_begin_time(std::vector<int>& spare_points_list, int value){
    std::vector<int> executable_points_list(spare_points_list.size());
    std::copy(spare_points_list.begin(), spare_points_list.end(), executable_points_list.begin());

    for(int i = executable_points_list.size() - 1; 0 <= i; i--){
        executable_points_list[i] = std::min(executable_points_list[i], executable_points_list[i + 1] + 1);
    }

    int begin_time = -1;
    for(int i = 0; i < executable_points_list.size(); i++){
        if(value <= executable_points_list[i]){
            begin_time = i;
            break;
        }
    }
    return begin_time;
}

void update_spare_points_list(std::vector<int>& spare_points_list, int begin_time, int point){
    for(int i = 0; begin_time + i < spare_points_list.size(); i++){
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

void choose_and_activate_jobs(){
    int predict_size = CAPACITY + 1;
    std::vector<int> spare_points_list(predict_size);
    update_spare_points_list_by_all_active_jobs(spare_points_list);

    for(int j = PRIORITY_RANGE - 1; 0 <= j; j--){
        for(auto it = inactive_job_lists[j].begin(); it != inactive_job_lists[j].end(); ){
            int begin_time = find_executable_begin_time(spare_points_list, it->remaining_point);
            if(begin_time == -1){
                ++it;
                continue;
            }
            update_spare_points_list(spare_points_list, begin_time, it->remaining_point);

            if(begin_time == 0){
                active_job_lists[j].push_back(*it);
                it = active_job_lists[j].erase(it);
            }else{
                ++it;
            }

            if(spare_points_list[0] == 0){
                return;
            }
        }
    }
    return;
}


int main(){
    env_init();
    
    for(int t = 0; t <= MAXTIME; t++){
        update_active_jobs();
        add_job_to_inactive_job_lists(t);
        choose_and_activate_jobs();
        print_exec_point(t);
    }

    /*
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
    */

    return 0;
}

