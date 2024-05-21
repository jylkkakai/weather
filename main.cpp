#include <curl/curl.h>
#include <iostream>
#include <pugixml.hpp>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string parseDateTime(std::string s) {
  std::string d = s.substr(s.find('-') + 1, s.find('T') - s.find('-') - 1);
  std::string t = s.substr(s.find('T') + 1, s.rfind(':') - s.find('T') - 1);

  return d + "  " + t;
}
int main() {

  // std::cout << "Hello, World!" << std::endl;
  std::string url =
      // "http://opendata.fmi.fi/"
      // "wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::"
      // "forecast::harmonie::surface::point::multipointcoverage&place=nokia&";
      "http://opendata.fmi.fi/"
      "wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=ecmwf::"
      "forecast::surface::point::simple&place=nokia&parameters=Temperature&";
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
  // std::cout << readBuffer << std::endl;
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_string(readBuffer.c_str());
  pugi::xml_node root = doc.document_element();
  for (auto node : root) {
    std::cout << parseDateTime(node.child("BsWfs:BsWfsElement")
                                   .child("BsWfs:Time")
                                   .child_value())
              << "\t";
    std::cout << node.child("BsWfs:BsWfsElement")
                     .child("BsWfs:ParameterValue")
                     .child_value()
              << std::endl;
  }
  return 0;
}
// struct xml_string_writer : pugi::xml_writer {
//   std::string result;
//
//   virtual void write(const void *data, size_t size) {
//     result.append(static_cast<const char *>(data), size);
//   }
// };
// std::string InnerXML(pugi::xml_node target) {
//   xml_string_writer writer;
//   for (pugi::xml_node child = target.first_child(); child;
//        child = child.next_sibling()) {
//
//     std::cout << child.name() << std::endl;
//     std::cout << child.child_value() << std::endl;
//     // std::cout << child.next_sibling << std::endl;
//     // child.print(writer, "");
//   }
//   return writer.result;
// }
