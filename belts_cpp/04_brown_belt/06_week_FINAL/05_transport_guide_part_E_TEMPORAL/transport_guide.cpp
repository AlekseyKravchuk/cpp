#include "bus.h"
#include "transport_manager.h"

#include "json_parser.h"
#include "stop_manager.h"
#include "transport_manager_command.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace std;

void HandleInputCommand(TransportManager &manager, const InCommand *command) {
  if (command->Type() == InCommandType::NEW_STOP) {
    auto new_stop_command = dynamic_cast<const NewStopCommand *>(command);
    manager.AddStop(new_stop_command->Name(), new_stop_command->Latitude(),
                    new_stop_command->Longitude(),
                    new_stop_command->Distances());
  } else if (command->Type() == InCommandType::NEW_BUS) {
    auto new_bus_command = dynamic_cast<const NewBusCommand *>(command);
    manager.AddBus(new_bus_command->Name(), new_bus_command->Stops(),
                   new_bus_command->IsCyclic());
  } else {
    throw std::invalid_argument("Unsupported command");
  }
}

void HandleOutputCommand(TransportManager &manager, const OutCommand *command, vector<StopInfo>& stop_info_data, vector<BusInfo>& bus_info_data, vector<RouteInfo>& route_data) {
  if (command->Type() == OutCommandType::STOP_DESCRIPTION) {
    auto stop_command = dynamic_cast<const StopDescriptionCommand *>(command);
    auto stop_info = manager.GetStopInfo(stop_command->Name(), stop_command->RequestId());
    stop_info_data.push_back(std::move(stop_info));
  } else if (command->Type() == OutCommandType::BUS_DESCRIPTION) {
    auto bus_command = dynamic_cast<const BusDescriptionCommand *>(command);
    auto bus_info = manager.GetBusInfo(bus_command->Name(), bus_command->RequestId());
    bus_info_data.push_back(std::move(bus_info));
  } else if (command->Type() == OutCommandType::ROUTE) {
    auto route_command = dynamic_cast<const RouteCommand*>(command);
    auto route_info = manager.GetRouteInfo(route_command->From(), route_command->To(), route_command->RequestId());
    route_data.push_back(std::move(route_info));
  } else {
    throw std::invalid_argument("Unsupported command");
  }
}

int main() {
    string input_json_file_name = "./00_inputs_and_outputs/input_1.json";
//    string input_json_file_name = "./00_inputs_and_outputs/input_4.json";
    ifstream ifs{input_json_file_name};
    if (!ifs) {
        throw std::runtime_error("File \"" + input_json_file_name + "\" is not opened.");
    }

    TransportManagerCommands commands = JsonArgs::ReadCommands(ifs);

//  TransportManagerCommands commands = JsonArgs::ReadCommands(cin);

  TransportManager manager{
    RoutingSettings{commands.routing_settings.bus_wait_time, commands.routing_settings.bus_velocity}
  };

  for (const auto& command : commands.input_commands) {
    HandleInputCommand(manager, command.get());
  }

  manager.CreateRoutes();

  vector<StopInfo> stop_info_data;
  vector<BusInfo> bus_info_data;
  vector<RouteInfo> route_data;

  for (const auto& command : commands.output_commands) {
    HandleOutputCommand(manager, command.get(), stop_info_data, bus_info_data, route_data);
  }

//  JsonArgs::PrintResults(stop_info_data, bus_info_data, route_data, cout);
    string output_file_name = "./00_inputs_and_outputs/output_1.json";
//    string output_file_name = "./00_inputs_and_outputs/output_4.json";
    ofstream ofs(output_file_name);
    if (!ofs) {
        throw std::runtime_error("File \"" + output_file_name + "\" is not opened.");
    }

//    JsonArgs::PrintResults(stop_info_data, bus_info_data, route_data, cout);
    JsonArgs::PrintResults(stop_info_data, bus_info_data, route_data, ofs);
}
