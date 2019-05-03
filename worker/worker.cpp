#include <string> 
#include <sstream>
#include <iostream> 
#include <iomanip>
#include <curl/curl.h> 
#include "json11/json11.hpp"

size_t callbackWrite(char *ptr, size_t size, size_t nmemb, std::string *stream)
{
    int dataLength = size * nmemb;
    stream->append(ptr, dataLength);
    return dataLength;
}

std::string int2ts(int i){
    std::ostringstream ts;
    ts << std::setfill('0') << std::right << std::setw(2) << i / 3600;
    ts << ":";
    ts << std::setfill('0') << std::right << std::setw(2) << (i / 60) % 60;
    ts << ":";
    ts << std::setfill('0') << std::right << std::setw(2) << i % 60;
    return ts.str();
}

int main()
{
    CURL *curl;
    CURLcode ret;
    
    for(int i = 0; i <= 1000; i++){
        std::string url = "http://localhost:5000/jobs/" + int2ts(i);
        std::string res;

        curl = curl_easy_init();
        if (curl == NULL) {
            std::cerr << "curl_easy_init() failed" << std::endl;
            return 1;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackWrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
        ret = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (ret != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed." << std::endl;
            return 1;
        }

        std::cout << res << std::endl;

        std::string json_err;
        auto json_sample = json11::Json::parse(res, json_err); 

        std::cout << json_sample["Jobs"][0]["Tasks"].dump() << std::endl;
    }

    return 0;
}
