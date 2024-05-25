#ifndef PARSEWEATHERQUERY_H
#define PARSEWEATHERQUERY_H
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

void getCurrentObservation();
#endif // !PARSEWEATHERQUERY_H
