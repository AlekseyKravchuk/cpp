#pragma once

#include <algorithm>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>
#include <optional>
#include <memory>
#include <variant>

enum class InCommandType {
  NEW_STOP,
  NEW_BUS,

  NUM_COMMANDS,
};

enum class OutCommandType {
  STOP_DESCRIPTION,
  BUS_DESCRIPTION,
  ROUTE,

  NUM_COMMANDS,
};

struct InCommand {
public:
  explicit InCommand(InCommandType type) : _type(type) {}
  virtual InCommandType Type() const { return _type; }

private:
  InCommandType _type{InCommandType::NUM_COMMANDS};
};

struct OutCommand {
public:
  explicit OutCommand(OutCommandType type) : _type(type) {}
  virtual OutCommandType Type() const { return _type; }

private:
  OutCommandType _type{OutCommandType::NUM_COMMANDS};
};

struct RoutingSettingsCommand {
  unsigned int bus_wait_time;
  double bus_velocity;
};

struct TransportManagerCommands {
  std::vector<std::unique_ptr<InCommand>> input_commands;
  std::vector<std::unique_ptr<OutCommand>> output_commands;
  RoutingSettingsCommand routing_settings;
};

struct NewStopCommand : public InCommand {
public: 
  NewStopCommand(std::string name, double latitude, double longitude, std::unordered_map<std::string, unsigned int> distances)
    : InCommand(InCommandType::NEW_STOP)
    , _name(std::move(name))
    , _latitude(latitude)
    , _longitude(longitude)
    , _distances(std::move(distances)) { }

  std::string Name() const { return _name; }
  double Latitude() const { return _latitude; }
  double Longitude() const { return _longitude; }
  const auto& Distances() const { return _distances; }

private:
  std::string _name;
  double _latitude;
  double _longitude;
  std::unordered_map<std::string, unsigned int> _distances;
};

struct NewBusCommand : public InCommand {
public: 
  NewBusCommand(std::string name, std::vector<std::string> stops, bool is_roundtrip)
    : InCommand(InCommandType::NEW_BUS)
    , _name(std::move(name))
    , _stops(std::move(stops))
    , _is_roundtrip(is_roundtrip) { }

  std::string Name() const { return _name; }
  std::vector<std::string> Stops() const { return _stops; }
  bool IsCyclic() const { return _is_roundtrip; }

private:
  std::string _name;
  std::vector<std::string> _stops;
  bool _is_roundtrip;
};

struct StopDescriptionCommand : public OutCommand {
public: 
  StopDescriptionCommand(std::string name)
    : OutCommand(OutCommandType::STOP_DESCRIPTION)
    , _name(std::move(name))
  {
  }

  StopDescriptionCommand(std::string name, size_t request_id)
    : OutCommand(OutCommandType::STOP_DESCRIPTION)
    , _name(std::move(name))
    , _request_id(request_id) { }

  std::string Name() const { return _name; }
  size_t RequestId() const { return _request_id; }

private:
  std::string _name;
  size_t _request_id{std::numeric_limits<size_t>::max()};
};

struct BusDescriptionCommand : public OutCommand {
public: 
  explicit BusDescriptionCommand(std::string name)
    : OutCommand(OutCommandType::BUS_DESCRIPTION)
    , _name(std::move(name)) { }

  BusDescriptionCommand(std::string name, size_t request_id)
    : OutCommand(OutCommandType::BUS_DESCRIPTION)
    , _name(std::move(name))
    , _request_id(request_id) { }

  std::string Name() const { return _name; }
  size_t RequestId() const { return _request_id; }

private:
  std::string _name;
  size_t _request_id{std::numeric_limits<size_t>::max()};
};

struct RouteCommand : public OutCommand {
public: 
  RouteCommand(std::string from, std::string to)
    : OutCommand(OutCommandType::ROUTE)
    , _from(std::move(from))
    , _to(std::move(to))
  {
  }

  RouteCommand(std::string from, std::string to, size_t request_id)
    : OutCommand(OutCommandType::ROUTE)
    , _from(std::move(from))
    , _to(std::move(to))
    , _request_id(request_id)
  {
  }

  std::string From() const { return _from; }
  std::string To() const { return _to; }
  size_t RequestId() const { return _request_id; }

private:
  std::string _from;
  std::string _to;
  size_t _request_id{std::numeric_limits<size_t>::max()};
};

struct StopInfo {
  std::vector<std::string> buses;
  size_t request_id;
  std::optional<std::string> error_message;
};

struct BusInfo {
  size_t route_length{};
  size_t request_id{};
  double curvature{};
  size_t stop_count{};
  size_t unique_stop_count{};
  std::optional<std::string> error_message;
};

struct WaitActivity {
  std::string type;
  unsigned int time;
  std::string stop_name;
};

struct BusActivity {
  std::string type;
  double time;
  std::string bus;
  unsigned int span_count;
};

struct RouteInfo {
  size_t request_id;
  double total_time;
  std::vector<std::variant<WaitActivity, BusActivity>> items;
  std::optional<std::string> error_message;
};
