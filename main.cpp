#include "parseWeatherQuery.hpp"
#include "printWeather.hpp"
#include "weather.hpp"
#include <iostream>
#include <string>

int main() {

  // std::cout << "Hello, World!" << std::endl;
  // getForecast();
  std::vector<Weather> p = getCurrentObservation();
  printCurrentWeather(p[p.size() - 1]);
  return 0;
}
