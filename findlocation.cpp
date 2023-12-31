#include "Parser.hpp"

Parser * findLocation(const std::map<std::string, Parser *>& locations, const std::string& path) 
{
    auto it = locations.begin();
    
    while (it != locations.end()) {
        std::string locationPath = it->first; or it->second->getslocation();

        // If locationPath contains a slash at the end, remove it
        if (!locationPath.empty() && locationPath.back() == '/') {
            locationPath.pop_back();
        }

        if (locationPath == path)
        {
            return it->second; // Match found
        }

        ++it;
    }
    return locations.end()->second; // No match found
}