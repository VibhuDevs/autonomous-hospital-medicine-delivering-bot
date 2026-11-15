#ifndef ANP_AUTONOMY_LOCATION_UTILS_HPP_
#define ANP_AUTONOMY_LOCATION_UTILS_HPP_

#include <yaml-cpp/yaml.h>

#include <map>
#include <stdexcept>
#include <string>

struct Location
{
  double x;
  double y;
  double theta;
};

inline std::map<std::string, Location> loadLocations(const std::string & path)
{
  const auto document = YAML::LoadFile(path);
  std::map<std::string, Location> locations;
  for (const auto & entry : document) {
    if (!entry.second.IsSequence() || entry.second.size() != 3) {
      throw std::runtime_error("Each map location must be [x, y, theta]: " + entry.first.as<std::string>());
    }
    locations.emplace(entry.first.as<std::string>(), Location{
      entry.second[0].as<double>(), entry.second[1].as<double>(), entry.second[2].as<double>()});
  }
  if (locations.empty()) {
    throw std::runtime_error("The map locations file contains no locations: " + path);
  }
  return locations;
}

#endif
