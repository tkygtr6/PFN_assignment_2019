#include "parser.hpp"

std::string int2ts(int i){
    std::ostringstream ts;
    ts << std::setfill('0') << std::right << std::setw(2) << i / 3600;
    ts << ":";
    ts << std::setfill('0') << std::right << std::setw(2) << (i / 60) % 60;
    ts << ":";
    ts << std::setfill('0') << std::right << std::setw(2) << i % 60;
    return ts.str();
}

size_t callbackWrite(char *ptr, size_t size, size_t nmemb, std::string *stream){
    int dataLength = size * nmemb;
    stream->append(ptr, dataLength);
    return dataLength;
}

std::vector<Job> get_and_parse_json(int sec){
    CURL *curl;
    CURLcode ret;
    std::string url = "http://localhost:5000/jobs/" + int2ts(sec);
    std::string res;

    curl = curl_easy_init();
    if (curl == NULL) {
        std::cerr << "curl_easy_init() failed" << std::endl;
        std::vector<Job> jobs;
        return jobs;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackWrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
    ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (ret != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed." << std::endl;
        std::vector<Job> jobs;
        return jobs;
    }

    std::string json_err; // todo
    auto jobs_json = json11::Json::parse(res, json_err); 
    jobs_json = jobs_json["Jobs"];

    std::vector<Job> jobs;
    for(const auto& job_json : jobs_json.array_items()){
        Job job(job_json);
        jobs.push_back(job);
    }
    return jobs;
}
