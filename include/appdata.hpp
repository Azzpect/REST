#pragma once

#include "nlohmann/json.hpp"
#include <cstdlib>
#include <string>
#include <list>

namespace AppDataNS {

class Request {
public:
  Request() = default;
  Request(const std::string&);
  std::string name, endpoint, method;
};

class Dir {
public:
  Dir() = default;
  Dir(const std::string&);
  std::string name;
  std::list<Request> requests;
  std::list<Dir> dirs;
};

class AppData {
  std::string appdata_dir =
      std::string(std::getenv("HOME")) + "/.local/share/rest";
  std::string appdata_file =
      std::string(std::getenv("HOME")) + "/.local/share/rest/appdata.json";

public:
  AppData();
  Dir appdata;
  void addDir();
};

void to_json(nlohmann::json &, const Request &);
void to_json(nlohmann::json &, const Dir &);
void from_json(const nlohmann::json &, Dir &);
void from_json(const nlohmann::json &, Request &);

} // namespace AppDataNS
