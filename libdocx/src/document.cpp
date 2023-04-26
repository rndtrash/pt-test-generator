#include <libdocx/libdocx.h>

#include <libzippp.h>

#include <fstream>
#include <stdexcept>
#include <sstream>

#include <iostream>

using namespace libdocx;

Document::Document()
{}

Document::Document(const pugi::xml_node &node)
{
    auto documentRootNode = node.first_child();
    documentRootNode.print(std::cout);
    std::cout.flush();
}

std::unique_ptr<pugi::xml_node> Document::to_xml()
{
    return std::make_unique<pugi::xml_node>(); // TODO: stub
}
