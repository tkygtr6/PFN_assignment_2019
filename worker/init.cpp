#include <cstdlib>
#include <string>
#include <list>
#include "init.hpp"
#include "types.hpp"

extern int MAXTIME;
extern int CAPACITY;
extern int PRIORITY_RANGE;
extern int NAIVE;

extern std::vector <std::list<Job>> active_job_lists;
extern std::vector <std::list<Job>> inactive_job_lists;

void env_init(){
    MAXTIME = MAXTIME_DEFAULT;
    if(const char* env_p = std::getenv("MAXTIME")){
        int i_value = atoi(env_p);
        if (i_value){
            MAXTIME = atoi(env_p);
        }
    }

    CAPACITY = CAPACITY_DEFAULT;
    if(const char* env_p = std::getenv("CAPACITY")){
        int i_value = atoi(env_p);
        if (i_value){
            CAPACITY = atoi(env_p);
        }
    }

    PRIORITY_RANGE = PRIORITY_RANGE_DEFAULT;
    if(const char* env_p = std::getenv("PRIORITY_RANGE")){
        int i_value = atoi(env_p);
        if (i_value){
            PRIORITY_RANGE = atoi(env_p);
        }
    }

    active_job_lists.resize(PRIORITY_RANGE);
    inactive_job_lists.resize(PRIORITY_RANGE);

    NAIVE = NAIVE_DEFAULT;
    if(const char* env_p = std::getenv("NAIVE")){
        int i_value = atoi(env_p);
        if (i_value){
            NAIVE = atoi(env_p);
        }
    }
}

