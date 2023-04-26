#pragma once

#include <pugixml.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace libdocx {

class ContentTypes
{
public:
    ContentTypes(const pugi::xml_document &ctXml);
    void add_file(const std::string &fileName);

private:
    void add_file_to_map(const std::string &mime, const std::string &fileName);

    std::unordered_map<std::string, std::string> extensionToMime;
    std::unordered_map<std::string, std::string> mimeOverrides;

    std::unordered_map<std::string, std::vector<std::string>> mimeToFile;
    std::unordered_map<std::string, std::string> fileToMime;
};

} // namespace libdocx
