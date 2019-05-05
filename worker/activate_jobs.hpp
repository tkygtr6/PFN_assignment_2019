#pragma once
#include <iostream> 
#include <list>
#include <vector>
#include "types.hpp"

void activate_jobs();

// This is NAIVE implementation of activate_jobs. Tasks are not executed when any higher priority tasks exists.
void activate_jobs_naively(int spare_point);
