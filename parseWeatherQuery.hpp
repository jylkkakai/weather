#ifndef PARSEWEATHERQUERY_HPP
#define PARSEWEATHERQUERY_HPP
#include "weather.hpp"
#include <string>
// #include <curl/curl.h>
// #include <iostream>
// #include <pugixml.hpp>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp);

std::string getXml(std::string url);

std::string parseDateTime(std::string s);

void getForecast();

std::vector<Weather> getCurrentObservation();
#endif // !PARSEWEATHERQUERY_HPP
