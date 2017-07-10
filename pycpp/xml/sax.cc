//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/xml/sax.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


static xml_attr_t parse_attributes(const xmlChar** attrs)
{
    xml_attr_t attrib;

    // parse the attrs
    if (attrs != nullptr) {
        // find start/end
        const xmlChar** first = attrs;
        const xmlChar** last = first;
        while (*last != nullptr) {
            ++last;
        }

        for (; first < last; ) {
            std::string key((char*) *first++);
            std::string value((char*) *first++);
            attrib[std::move(key)] = std::move(value);
        }
    }

    return attrib;
}


static int stream_read(void* ctx, char* buffer, int length)
{
    auto* stream = (std::istream*) ctx;
    stream->read(buffer, length);
    return stream->gcount();
}



static int stream_close(void*)
{
    return 0;
}


static void internal_subset_handler(void* data, const xmlChar* name,
                                    const xmlChar* external_id,
                                    const xmlChar* system_id)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    // TODO: implement.
}


static void start_document_handler(void* data)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->start_document();
}


static void end_document_handler(void* data)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->end_document();
}


static void start_element_handler(void* data, const xmlChar* name, const xmlChar** attrs)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->start_element(string_view((char*) name), parse_attributes(attrs));
}


static void end_element_handler(void* data, const xmlChar* name)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->end_element(string_view((char*) name));
}


static void characters_handler(void* data, const xmlChar* ch, int len)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->characters(string_view((char*) ch, len));
}


static void reference_handler(void* data, const xmlChar* name)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
//    handler->characters(string_view((char*) ch, len));
}


static void ignorable_whitespace_handler(void* data, const xmlChar* ch, int len)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->ignorable_whitespace(string_view((char*) ch, len));
}


static void processing_instruction_handler(void* data, const xmlChar* target, const xmlChar* value)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->processing_instruction(string_view((char*) target), string_view((char*) value));
}


static void skipped_entity_handler(void* data, const xmlChar* name,
                                   const xmlChar* public_id,
                                   const xmlChar* system_id,
                                   const xmlChar* notation_name)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->skipped_entity(string_view((char*) name));
}


void start_element_ns_handler(void* data, const xmlChar* localname,
                              const xmlChar* prefix, const xmlChar* uri,
                              int nb_namespaces, const xmlChar** namespaces,
                              int nb_attributes, int nb_defaulted,
                              const xmlChar** attrs)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->start_element_ns(string_view((char*) uri), string_view((char*) prefix), string_view((char*) localname), parse_attributes(attrs));
}


void end_element_ns_handler(void* data, const xmlChar* localname,
                            const xmlChar* prefix, const xmlChar* uri)
{
    xml_sax_handler* handler = (xml_sax_handler*) data;
    handler->end_element_ns(string_view((char*) uri), string_view((char*) prefix), string_view((char*) localname));
}


static void comment_handler(void* data, const xmlChar* msg)
{}


static void warning_handler(void* data, const char* msg, ...)
{}


static void error_handler(void* data, const char* msg, ...)
{
    throw std::runtime_error(msg);
}


static void fatal_error_handler(void* data, const char* msg, ...)
{
    throw std::runtime_error(msg);
}


/**
 *  \brief Transform libxml2 API to public SAX handler.
 */
struct handler_wrapper
{
public:
    handler_wrapper(xml_sax_handler&);

    xml_sax_handler* mapper();
    xmlSAXHandler* handler();

private:
    xml_sax_handler* public_ = nullptr;
    xmlSAXHandler private_;
};


handler_wrapper::handler_wrapper(xml_sax_handler& h):
    public_(&h)
{
    memset(&private_, 0, sizeof(private_));

    private_.internalSubset = internal_subset_handler;
    private_.unparsedEntityDecl = skipped_entity_handler;
    private_.startDocument = start_document_handler;
    private_.endDocument = end_document_handler;
    private_.startElement = start_element_handler;
    private_.endElement = end_element_handler;
    private_.reference = reference_handler;
    private_.characters = characters_handler;
    private_.ignorableWhitespace = ignorable_whitespace_handler;
    private_.processingInstruction = processing_instruction_handler;
    private_.comment = comment_handler;
    private_.warning = warning_handler;
    private_.error = error_handler;
    private_.fatalError = fatal_error_handler;
    private_.initialized = XML_SAX2_MAGIC;

    if (public_->use_namespaces()) {
        private_.startElementNs = start_element_ns_handler;
        private_.endElementNs = end_element_ns_handler;
    }
}


xml_sax_handler* handler_wrapper::mapper()
{
    return public_;
}


xmlSAXHandler* handler_wrapper::handler()
{
    return &private_;
}


struct reader_impl
{
    void parse(std::istream&, handler_wrapper&);
};


void reader_impl::parse(std::istream& stream, handler_wrapper& wrapper)
{
    // create our context
    xmlParserCtxtPtr ctx = xmlCreateIOParserCtxt(wrapper.handler(),
        wrapper.mapper(),
        stream_read,
        stream_close,
        &stream,
        XML_CHAR_ENCODING_NONE
    );

    // process the context
    if (ctx == nullptr) {
        throw std::runtime_error("Unable to make SAX context.");
    }

    if (ctx->sax != (xmlSAXHandlerPtr) &xmlDefaultSAXHandler) {
        xmlFree(ctx->sax);
    }
    ctx->sax = wrapper.handler();

    // parse data
    xmlParseDocument(ctx);
    int error = ctx->wellFormed ? 0 : ctx->errNo;

    // free our context
    if (wrapper.handler() != nullptr) {
        ctx->sax = nullptr;
    }
    if (ctx->myDoc != nullptr) {
        xmlFreeDoc(ctx->myDoc);
        ctx->myDoc = nullptr;
    }
    xmlFreeParserCtxt(ctx);

    // handle errors
    if (error != 0) {
        // TODO: create a real message
        throw std::runtime_error("Unknown runtime error during SAX2.");
    }
}

// OBJECTS
// -------


xml_sax_handler::xml_sax_handler(bool use_namespaces):
    use_namespaces_(use_namespaces)
{}


void xml_sax_handler::start_document()
{}


void xml_sax_handler::end_document()
{}


void xml_sax_handler::start_element(const string_view& content, xml_attr_t&& attrs)
{}


void xml_sax_handler::end_element(const string_view& content)
{}


void xml_sax_handler::characters(const string_view& content)
{}


void xml_sax_handler::start_element_ns(const string_view& uri, const string_view& prefix, const string_view& localname, xml_attr_t&& attrs)
{}


void xml_sax_handler::end_element_ns(const string_view& uri, const string_view& prefix, const string_view& localname)
{}


void xml_sax_handler::ignorable_whitespace(const string_view& whitespace)
{}


void xml_sax_handler::processing_instruction(const string_view& target, const string_view& data)
{}


void xml_sax_handler::skipped_entity(const string_view& name)
{}


bool xml_sax_handler::use_namespaces() const
{
    return use_namespaces_;
}


xml_stream_reader::xml_stream_reader()
{}


void xml_stream_reader::parse(std::istream& s)
{
    stream_ = &s;
    if (!handler_) {
        throw std::runtime_error("Must assign handler prior parsing.");
    }

    // parse stream
    handler_wrapper wrapper(*handler_);
    reader_impl reader;
    reader.parse(*stream_, wrapper);
}


void xml_stream_reader::set_handler(xml_sax_handler& h)
{
    handler_ = &h;
}


xml_file_reader::xml_file_reader()
{}


xml_file_reader::xml_file_reader(const std::string &name)
{
    open(name);
}


void xml_file_reader::open(const std::string &name)
{
    file_.open(name, std::ios_base::binary);
}


void xml_file_reader::parse(const std::string &name)
{
    open(name);
    parse();
}


#if defined(PYCPP_HAVE_WFOPEN)


xml_file_reader::xml_file_reader(const std::wstring &name)
{
    open(name);
}


void xml_file_reader::open(const std::wstring &name)
{
    file_.open(name, std::ios_base::binary);
}


void xml_file_reader::parse(const std::wstring &name)
{
    open(name);
    parse();
}

#endif


void xml_file_reader::parse()
{
    if (!file_.is_open()) {
        throw std::runtime_error("Must open file handle prior to parsing.");
    }
    xml_stream_reader::parse(file_);
}

PYCPP_END_NAMESPACE
