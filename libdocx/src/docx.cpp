#include <libdocx/content_types.h>
#include <libdocx/libdocx.h>

#include <libzippp.h>
#include <pugixml.hpp>

#include <iostream> // TODO: DEBUG!!!
#include <sstream>

using namespace libdocx;

DocX::DocX(const std::string &path)
{
    libzippp::ZipArchive arc(path);
    auto archiveIsOpen = arc.open(libzippp::ZipArchive::ReadOnly);
    if (!archiveIsOpen) {
        throw std::runtime_error("Unable to open the archive");
    }

    // Parse [Content_Types].xml in order to find the necessary files
    pugi::xml_document contentTypesDocument;
    {
        auto ctFile = arc.getEntry("[Content_Types].xml");
        if (ctFile.isNull()) {
            throw std::runtime_error("Unable to find [Content_Types].xml in the document");
        }

        std::stringstream ctStream;
        auto streamResult = ctFile.readContent(ctStream);
        if (streamResult != LIBZIPPP_OK) {
            std::stringstream sstream;
            sstream << "Unable to open stream (libzippp error " << streamResult << ")";
            throw std::runtime_error(sstream.str());
        }

        auto xmlLoadResult = contentTypesDocument.load(ctStream);
        if (xmlLoadResult.status != pugi::status_ok) {
            throw std::runtime_error(xmlLoadResult.description());
        }
    }
    ContentTypes contentTypes(contentTypesDocument);
    for (auto file : arc.getEntries()) {
        auto fileName = "/" + file.getName(); // QUIRK: pathes in XML start with /
        std::cout << fileName << std::endl;

        if (!file.isFile()) {
            throw std::runtime_error("Entry " + fileName + " is not a file");
        }

        contentTypes.add_file(fileName);
    }

    // Parse the main word document
    {
        auto documentFile = arc.getEntry("word/document.xml");
        if (documentFile.isNull()) {
            throw std::runtime_error("Unable to find the main document XML, even though it's "
                                     "mentioned in Content Types");
        }

        std::stringstream documentStream;
        auto streamResult = documentFile.readContent(documentStream);
        if (streamResult != LIBZIPPP_OK) {
            std::stringstream sstream;
            sstream << "Unable to open stream (libzippp error " << streamResult << ")";
            throw std::runtime_error(sstream.str());
        }

        pugi::xml_document doc;
        auto xmlLoadResult = doc.load(documentStream);
        if (xmlLoadResult.status != pugi::status_ok) {
            throw std::runtime_error(xmlLoadResult.description());
        }

        mainDocument = std::make_shared<Document>(doc);
    }

    if (arc.close() != LIBZIPPP_OK) {
        throw std::runtime_error("Failed to close the archive");
    }
}
