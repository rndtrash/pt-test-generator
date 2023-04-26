#include <libdocx/content_types.h>

//#include <filesystem> // TODO: filesystem doesn't recognise extensions likle /_rels/.rels
#include <iostream> // DEBUG!!
#include <regex>
#include <sstream>

using namespace libdocx;

ContentTypes::ContentTypes(const pugi::xml_document &ctXml)
{
    ctXml.document_element().print(std::cout);
    std::cout.flush();

    auto documentElement = ctXml.document_element();
    if (documentElement.empty() || std::string(documentElement.name()) != "Types") {
        throw std::runtime_error("Invalid [Content_Types].xml file");
    }

    {
        auto specAttribute = documentElement.attribute("xmlns");
        if (specAttribute.empty()) {
            throw std::runtime_error("Spec is not specified");
        }

        std::string specString(specAttribute.as_string());
        if (specString != "http://schemas.openxmlformats.org/package/2006/content-types") {
            throw std::runtime_error("This library does not support spec \"" + specString + "\"");
        }
    }

    {
        auto defaultNodes = documentElement.select_nodes("Default");
        for (auto defaultExtension : defaultNodes) {
            auto node = defaultExtension.node();

            auto extension = node.attribute("Extension");
            if (extension.empty()) {
                std::stringstream sstream;
                sstream << "No Extension in node ";
                node.print(sstream);
                throw std::runtime_error(sstream.str());
            }

            auto contentType = node.attribute("ContentType");
            if (contentType.empty()) {
                std::stringstream sstream;
                sstream << "No ContentType in node ";
                node.print(sstream);
                throw std::runtime_error(sstream.str());
            }

            extensionToMime[extension.as_string()] = contentType.as_string();
        }
    }

    {
        auto overrideNodes = documentElement.select_nodes("Override");
        for (auto overrideMime : overrideNodes) {
            auto node = overrideMime.node();

            auto partName = node.attribute("PartName");
            if (partName.empty()) {
                std::stringstream sstream;
                sstream << "No PartName in node ";
                node.print(sstream);
                throw std::runtime_error(sstream.str());
            }

            auto contentType = node.attribute("ContentType");
            if (contentType.empty()) {
                std::stringstream sstream;
                sstream << "No ContentType in node ";
                node.print(sstream);
                throw std::runtime_error(sstream.str());
            }

            mimeOverrides[partName.as_string()] = contentType.as_string();
        }
    }
}

void ContentTypes::add_file(const std::string &fileName)
{
    if (mimeOverrides.contains(fileName)) {
        auto mime = mimeOverrides[fileName];
        add_file_to_map(mime, fileName);
        return;
    }

    static const std::regex extensionRegex("\\.([^\\/\\\\\\.]+)$");
    std::smatch extensionMatch;
    auto hasExtension = std::regex_search(fileName, extensionMatch, extensionRegex);
    if (hasExtension) {
        auto extension = extensionMatch[1].str();
        if (!extensionToMime.contains(extension)) {
            throw std::runtime_error("Unhandled extension ." + extension);
        }

        auto mime = extensionToMime[extension];
        add_file_to_map(mime, fileName);
        return;
    }

    throw std::runtime_error("Unable to get the content type for a file " + fileName);
}

void ContentTypes::add_file_to_map(const std::string &mime, const std::string &fileName)
{
    if (fileToMime.contains(fileName)) {
        throw std::runtime_error("File " + fileName + " is already registered!");
    }

    fileToMime[fileName] = mime;

    if (!mimeToFile.contains(mime)) {
        mimeToFile[mime] = {};
    }
    mimeToFile[mime].push_back(fileName);
}
