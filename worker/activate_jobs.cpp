#include "activate_jobs.hpp"

extern int MAXTIME;
extern int CAPACITY;
extern int PRIORITY_RANGE;
extern int NAIVE;

extern std::vector <std::list<Job>> active_job_lists;
extern std::vector <std::list<Job>> inactive_job_lists;

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
void activate_jobs_naively(int spare_point){
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
