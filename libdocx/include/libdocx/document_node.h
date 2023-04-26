#pragma once

#include <pugixml.hpp>

#include <memory>

namespace libdocx {

template<class T>
class DocumentNode {
public:
    virtual std::unique_ptr<pugi::xml_node> to_xml() = 0;
};

}
