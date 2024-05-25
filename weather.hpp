#ifndef WEATHER_HPP
#define WEATHER_HPP
#include <string>

struct Weather {
  std::string dateTime;
  std::string temp;
  std::string pres;
  std::string windS;
  std::string windD;
};

#endif // !WEATHER_HPP
