#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <queue>

#include "exception.hh"
#include "network_interface.hh"





// \brief A router that has multiple network interfaces and
// performs longest-prefix-match routing between them.
class Router
{
public:
  // Add an interface to the router
  // \param[in] interface an already-constructed network interface
  // \returns The index of the interface after it has been added to the router
  size_t add_interface( std::shared_ptr<NetworkInterface> interface )
  {
    _interfaces.push_back( notnull( "add_interface", std::move( interface ) ) );
    return _interfaces.size() - 1;
  }

  // Access an interface by index
  std::shared_ptr<NetworkInterface> interface( const size_t N ) { return _interfaces.at( N ); }

  // Add a route (a forwarding rule)
  void add_route( uint32_t route_prefix,
                  uint8_t prefix_length,
                  std::optional<Address> next_hop,
                  size_t interface_num );

  // Route packets between the interfaces
  void route();
  uint32_t get_mask(uint8_t prefix_length) {
    if (prefix_length > 32) {
      throw std::out_of_range("Prefix length must be between 0 and 32");
    }
    return prefix_length ? 0xffffffff << (32 - prefix_length) : 0;
  }

private:
  // The router's collection of network interfaces
  std::vector<std::shared_ptr<NetworkInterface>> _interfaces {};
  struct RouteItem {
    size_t prefix_length{0};
    std::optional<Address> next_hop {std::nullopt};
    size_t interface_num {0};
  };
  std::unordered_map<uint32_t, RouteItem> _routing_table{};
};