#include "printWeather.hpp"
#include "weather.hpp"
#include <iostream>

void printCurrentWeather(Weather w) {

  std::vector<std::string> lines(ASCII_IMG_LINES);
  for (size_t i = 0; i < w.temp.length(); i++) {
    for (size_t j = 0; j < ASCII_IMG_LINES; j++) {
      // std::cout << i << " " << j << std::endl;
      lines[j] += asciiImgMap.at(w.temp[i])[j];
    }
  }
  std::cout << std::endl;
  for (size_t i = 0; i < lines.size(); i++) {
    std::cout << lines[i] << std::endl;
  }
  std::cout << std::endl;
}
