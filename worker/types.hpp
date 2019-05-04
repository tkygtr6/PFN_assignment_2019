#pragma once
#include <iostream>
#include <vector>
#include "json11/json11.hpp"

enum TaskStatus{
    CONTINUE,
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
            priority = job_json["Priority"].int_value();
            for(const auto& task : job_json["Tasks"].array_items()){
                tasks.push_back(task.int_value());
            }
            num_tasks = tasks.size();
            task_no = 1; // begin from 1
            remaining_point = tasks[task_no - 1];
        }

        TaskStatus update_task(){
            if (remaining_point > 1){
                remaining_point--;
            }else{
                if(task_no == num_tasks){
                    return JOB_FINISHED;
                }else{
                    task_no++;
                    remaining_point = tasks[task_no - 1];
                }
            }
            return CONTINUE;
        }
};

