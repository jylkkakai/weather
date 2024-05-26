#ifndef PARSEWEATHERQUERY_H
#define PARSEWEATHERQUERY_H
#include "weather.hpp"
#include <curl/curl.h>
#include <iostream>
// #include <pugixml.hpp>
#include <unordered_map>
#include <vector>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string getXml(std::string url) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  return readBuffer;
}

std::string parseDateTime(std::string s) {
  std::string d = s.substr(s.find('-') + 1, s.find('T') - s.find('-') - 1);
  std::string t = s.substr(s.find('T') + 1, s.rfind(':') - s.find('T') - 1);

  return d + "  " + t;
}

std::vector<Weather> getWeatherFromXmlStr(std::string s) {

  std::unordered_map<std::string, int> params = {
      {"t2m", 1},
      {"p_sea", 2},
      {"ws_10min", 3},
  };
  size_t pos = 0;
  size_t tLast = s.rfind("<BsWfs:Time>");
  size_t tPos = s.find("<BsWfs:Time>", pos);
  // std::cout << tPos << " " << tLast << std::endl;
  size_t pNamePos = 0;
  size_t pValuePos = 0;
  std::string tPrev = s.substr(tPos + 12, 20);
  Weather w;
  std::vector<Weather> ws;

  while (tPos < tLast) {
    tPos = s.find("<BsWfs:Time>", pos) + 12;
    pNamePos = s.find("<BsWfs:ParameterName>", pos) + 21;
    pValuePos = s.find("<BsWfs:ParameterValue>", pos) + 22;
    std::string t = s.substr(tPos, 20);
    std::string pName = s.substr(
        pNamePos, s.find("</BsWfs:ParameterName>", pNamePos) - pNamePos);
    std::string pValue = s.substr(
        pValuePos, s.find("</BsWfs:ParameterValue>", pValuePos) - pValuePos);

    if (tPrev != t) {
      w.dateTime = tPrev;
      ws.push_back(w);
      Weather wt;
      w = wt;
      tPrev = t;
      // w.dateTime = tPrev;
    }
    switch (params[pName]) {
    case 1: {
      w.temp = pValue;
      break;
    }
    case 2: {
      w.pres = "Ilmanpaine: " + pValue + " hPa";
      break;
    }
    case 3: {
      w.windS = "Tuulen nopeus: " + pValue + " m/s";
      break;
    }
    }
    // std::cout << t << " " << pName << " " << pValue << std::endl;
    pos = pValuePos;
  }
  return ws;
}

void getForecast() {

  std::string url =
      "http://opendata.fmi.fi/"
      "wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::"
      "forecast::edited::weather::scandinavia::point::simple&place=nokia&";
  // "http://opendata.fmi.fi/"
  // "wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=ecmwf::"
  // "forecast::surface::point::simple&place=nokia&";
  std::string xmlStr = getXml(url);
  std::cout << xmlStr << std::endl;
  // pugi::xml_document doc;
  // pugi::xml_parse_result result = doc.load_string(xmlStr.c_str());
  // pugi::xml_node root = doc.document_element();
  // for (auto node : root) {
  //   std::cout << parseDateTime(node.child("BsWfs:BsWfsElement")
  //                                  .child("BsWfs:Time")
  //                                  .child_value())
  //             << "\t";
  //   std::cout << node.child("BsWfs:BsWfsElement")
  //                    .child("BsWfs:ParameterValue")
  //                    .child_value()
  //             << std::endl;
  // }
}

std::vector<Weather> getCurrentObservation() {

  std::vector<Weather> weatherPoints;
  std::string url =
      "http://opendata.fmi.fi/"
      "wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::"
      "observations::weather::simple&place=nokia&"
      "starttime=2024-05-26T07:40:00Z&";
  std::string xmlStr = getXml(url);
  // std::cout << xmlStr << std::endl;
  weatherPoints = getWeatherFromXmlStr(xmlStr);
  if (weatherPoints.empty()) {
    std::cout << "Jokin meni vikaan. Säätietoja ei saatu." << std::endl;
    exit(1);
  }
  return weatherPoints;
  // for (auto point : weatherPoints) {
  //
  //   std::cout << "Aika: " << point.dateTime << std::endl;
  //   std::cout << point.temp << std::endl;
  //   std::cout << point.pres << std::endl;
  //   std::cout << point.windS << std::endl;
  // }
  // Weather point = weatherPoints[weatherPoints.size() - 1];
  // std::cout << "Aika: " << point.dateTime << std::endl;
  // std::cout << point.temp << std::endl;
  // std::cout << point.pres << std::endl;
  // std::cout << point.windS << std::endl;
  // pugi::xml_document doc;
  // pugi::xml_parse_result result = doc.load_string(xmlStr.c_str());
  // pugi::xml_node root = doc.document_element();
  // for (auto node : root) {
  //   if(
  //   std::cout << parseDateTime(node.child("BsWfs:BsWfsElement")
  //                                  .child("BsWfs:Time")
  //                                  .child_value())
  //             << "\t";
  //   std::cout << node.child("BsWfs:BsWfsElement")
  //                    .child("BsWfs:ParameterValue")
  //                    .child_value()
  //             << std::endl;
  // }
}
#endif // !PARSEWEATHERQUERY_H
