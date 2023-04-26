#pragma once

#include <libdocx/document_node.h>

#include <string>
#include <memory>

#include <pugixml.hpp>

namespace libdocx {

class Document;

class Document : virtual DocumentNode<Document> {
public:
    Document();
    Document(const pugi::xml_node &node);

    std::unique_ptr<pugi::xml_node> to_xml() override;

private:
};

}
