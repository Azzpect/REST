#include "appdata.hpp"
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

AppDataNS::Request::Request(const std::string& n) {
  this->name = n;
  this->method = "GET";
  this->endpoint = "";
} 
AppDataNS::Dir::Dir(const std::string& n) {
  this->name = n;
  this->dirs = {};
  this->requests = {};
} 

AppDataNS::AppData::AppData() {
  if (!(std::filesystem::exists(this->appdata_dir) &&
        std::filesystem::is_directory(this->appdata_dir))) {
    std::filesystem::create_directory(this->appdata_dir);

    std::ofstream file(this->appdata_file);
    if (!file.is_open()) {
      std::cout << "Failed to create appdata file!!!" << std::endl;
      exit(1);
    }
    this->appdata = AppDataNS::Dir("root");
    nlohmann::json j = appdata;
    std::string data = j.dump();
    file << data;
    file.close();
  } else if (!std::filesystem::exists(this->appdata_file)) {
    std::ofstream file(this->appdata_file);
    if (!file.is_open()) {
      std::cout << "Failed to create appdata file!!!" << std::endl;
      exit(1);
    }
    this->appdata = AppDataNS::Dir("root");
    nlohmann::json j = appdata;
    std::string data = j.dump();
    file << data;
    file.close();
  } else {
    std::ifstream file(this->appdata_file);
    nlohmann::json j = nlohmann::json::parse(file);
    this->appdata = j.get<AppDataNS::Dir>();
    file.close();
  }
}

void AppDataNS::to_json(nlohmann::json &j, const AppDataNS::Request &r) {
  j = nlohmann::json{
      {"name", r.name}, {"endpoint", r.endpoint}, {"method", r.method}};
}

void AppDataNS::to_json(nlohmann::json &j, const AppDataNS::Dir &d) {
  j = nlohmann::json{
      {"name", d.name}, {"dirs", d.dirs}, {"requests", d.requests}};
}

void AppDataNS::from_json(const nlohmann::json &j, AppDataNS::Request &r) {
  j.at("name").get_to(r.name);
  j.at("endpoint").get_to(r.endpoint);
  j.at("method").get_to(r.method);
}

void AppDataNS::from_json(const nlohmann::json &j, AppDataNS::Dir &d) {
  j.at("name").get_to(d.name);
  j.at("dirs").get_to(d.dirs);
  j.at("requests").get_to(d.requests);
}
