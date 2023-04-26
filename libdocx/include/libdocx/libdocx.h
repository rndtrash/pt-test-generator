#pragma once

#include <libdocx/document.h>

#include <memory>

namespace libdocx {

class DocX {
public:
    DocX();
    DocX(const std::string &filePath);

    std::shared_ptr<const Document> get_main_document();

private:
    std::shared_ptr<Document> mainDocument;
};

}
