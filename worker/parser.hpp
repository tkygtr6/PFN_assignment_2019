#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <curl/curl.h>
#include "json11/json11.hpp"
#include "types.hpp"

std::vector<Job> get_and_parse_json(int sec);
