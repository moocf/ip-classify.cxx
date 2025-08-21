#include <array>
#include <string>
#include <iostream>
#include <exception>
#include <cstdio>
#include <cstdint>

using namespace std;


// Define a byte type for clarity.
typedef array<uint8_t, 4> IP;


// Parse and IP address.
inline IP parseIP(const string& ipx) {
  size_t d1 = ipx.find('.');
  size_t d2 = ipx.find('.', d1+1);
  size_t d3 = ipx.find('.', d2+1);
  uint8_t o1 = stoi(ipx.substr(0, d1));
  uint8_t o2 = stoi(ipx.substr(d1+1, d2));
  uint8_t o3 = stoi(ipx.substr(d2+1, d3));
  uint8_t o4 = stoi(ipx.substr(d3+1));
  return {o1, o2, o3, o4};
}


// Stringify IP address.
inline string stringifyIP(IP ip) {
  return to_string(ip[0]) + '.' + to_string(ip[1]) + '.' +
         to_string(ip[2]) + '.' + to_string(ip[3]);
}


// Validate an IP address.
inline bool validIP(const string& ipx) {
  size_t d1 = ipx.find('.');
  size_t d2 = ipx.find('.', d1+1);
  size_t d3 = ipx.find('.', d2+1);
  int o1 = stoi(ipx.substr(0, d1));
  int o2 = stoi(ipx.substr(d1+1, d2));
  int o3 = stoi(ipx.substr(d2+1, d3));
  int o4 = stoi(ipx.substr(d3+1));
  return o1>=0 && o1<=255 && o2>=0 && o2<=255 && o3>=0 && o3<=255 && o4>=0 && o4<=255;
}


// Get class of IP address.
inline char ipClass(IP ip) {
  if (ip[0] < 128) return 'A';
  if (ip[0] < 192) return 'B';
  if (ip[0] < 224) return 'C';
  if (ip[0] < 240) return 'D';
  return 'E';
}


// Get network address.
inline IP networkAddress(IP ip, IP mask) {
  IP net;
  for (uint32_t i=0; i<4; ++i)
    net[i] = ip[i] & mask[i];
  return net;
}


// Get broadcast address.
inline IP broadcastAddress(IP net, IP mask) {
  IP broad;
  for (uint32_t i=0; i<4; ++i)
    broad[i] = net[i] | ~mask[i];
  return broad;
}


// Get first IP address.
inline IP firstIPAddress(IP net, IP mask) {
  return {net[0], net[1], net[2], uint8_t(net[3] + 1)};
}


// Get last IP address.
inline IP lastIPAddress(IP net, IP mask) {
  IP broad = broadcastAddress(net, mask);
  return {broad[0], broad[1], broad[2], uint8_t(broad[3] - 1)};
}


// Check if two IP addresses are in the same network.
inline bool sameNetwork(IP ip1, IP ip2, IP mask) {
  return networkAddress(ip1, mask) == networkAddress(ip2, mask);
}


// Check if an IP address belongs to private range.
inline bool privateIP(IP ip) {
  return (ip[0] == 10) ||
         (ip[0] == 172 && ip[1] >= 16 && ip[1] <= 31) ||
         (ip[0] == 192 && ip[1] == 168);
}


// Main function.
int main(int argc, char **argv) {
  string ip1x, ip2x, maskx;
  cout << "Enter IP address: ";
  cin >> ip1x;
  if (!validIP(ip1x)) { cout << "ERROR: Invalid IP address!\n"; return 1; }
  IP ip1 = parseIP(ip1x);
  cout << "IP Address: " << stringifyIP(ip1) << '\n';
  cout << "Class: " << ipClass(ip1) << '\n';
  cout << "Enter subnet mask: ";
  cin >> maskx;
  if (!validIP(maskx)) { cout << "ERROR: Invalid subnet mask!\n"; return 1; }
  IP mask = parseIP(maskx);
  IP net1 = networkAddress(ip1, mask);
  cout << "Subnet Mask: " << stringifyIP(mask) << '\n';
  cout << "Network Address:   " << stringifyIP(networkAddress(ip1, mask))   << '\n';
  cout << "Broadcast Address: " << stringifyIP(broadcastAddress(ip1, mask)) << '\n';
  cout << "First IP Address:  " << stringifyIP(firstIPAddress(net1, mask))   << '\n';
  cout << "Last IP Address:   " << stringifyIP(lastIPAddress(net1, mask))    << '\n';
  cout << "Enter second IP address: ";
  cin >> ip2x;
  if (!validIP(ip2x)) { cout << "ERROR: Invalid second IP address!\n"; return 1; }
  IP ip2 = parseIP(ip2x);
  cout << "Second IP Address: " << stringifyIP(ip2) << '\n';
  if (sameNetwork(ip1, ip2, mask)) cout << "Both IP addresses are in the same network.\n";
  else cout << "IP addresses are in different networks.\n";
  cout << stringifyIP(ip1) << " is " << (privateIP(ip1) ? "private" : "public") << ".\n";
  cout << stringifyIP(ip2) << " is " << (privateIP(ip2) ? "private" : "public") << ".\n";
  return 0;
}
