#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include "json11/json11.hpp"

extern int PRIORITY_RANGE;
extern int global_time;

enum TaskStatus{
    CONTINUE,
    TASK_FINISHED,
    JOB_FINISHED
};

class Job{
    public:
        int created;
        int job_id;
        int priority;
        std::vector<int> tasks;
        int num_tasks;
        int task_no;
        int remaining_point;

        Job(json11::Json job_json){
            created = job_json["Created"].int_value();
            job_id = job_json["JobID"].int_value();
            if(PRIORITY_RANGE == 1){
                priority = 0; // ignore priority
            }else{
                priority = job_json["Priority"].int_value();
            }
            for(const auto& task : job_json["Tasks"].array_items()){
                tasks.push_back(task.int_value());
            }
            num_tasks = tasks.size();
            task_no = 1; // ATTENTION: task_no begins from 1 not from 0
            remaining_point = tasks[task_no - 1];
        }

        TaskStatus update_task(){
            if (remaining_point > 1){
                remaining_point--;
            }else{
                if(task_no == num_tasks){
                    std::cerr << priority << " " << global_time - created - std::accumulate(tasks.begin(), tasks.end(), 0) << std::endl;
                    return JOB_FINISHED;
                }else{
                    task_no++;
                    remaining_point = tasks[task_no - 1];
                    return TASK_FINISHED;
                }
            }
            return CONTINUE;
        }
};

