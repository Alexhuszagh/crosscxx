//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/stl/sstream.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/string/string.h>
#include <pycpp/xml/dom.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

static void dump_impl(const xml_node_t& node, xml_stream_writer& writer)
{
    writer.start_element(node.get_tag());
    for (const auto& pair: node.get_attrs()) {
        writer.write_attribute(pair.first, pair.second);
    }
    writer.write_text(node.get_text());

    for (const xml_node_t& child: node.get_children()) {
        dump_impl(child, writer);
    }

    writer.end_element();
}

// OBJECTS
// -------


xml_string_t xml_node_t::tostring() const
{
    ostringstream stream;
    {
        xml_stream_writer writer(stream, ' ', 0);
        dump_impl(*this, writer);
    }

    // remove the XML declaration
    xml_string_t str = stream.str();
    size_t end_decl = str.find("?>\n");
    if (end_decl == str.npos) {
        return str;
    }
    return str.substr(end_decl + 3);
}

// HANDLER

xml_dom_handler::xml_dom_handler(xml_dom_handler&& rhs)
{
    swap(rhs);
}


xml_dom_handler& xml_dom_handler::operator=(xml_dom_handler&& rhs)
{
    swap(rhs);
    return *this;
}


xml_dom_handler::xml_dom_handler(xml_node_t& root):
    root_(&root)
{}


void xml_dom_handler::start_document()
{
    levels_.emplace_back(root_);
}


void xml_dom_handler::end_document()
{
    levels_.pop_back();
}


void xml_dom_handler::start_element(const string_wrapper& name, xml_attr_t&& attrs)
{
    xml_node_t* parent = levels_.back();
    xml_node_list_t& list = parent->get_children();

    // create and append child
    xml_node_t child;
    child.set_tag(xml_string_t(name));
    child.set_attrs(forward<xml_attr_t>(attrs));
    list.push_back(move(child));

    levels_.emplace_back(&*list.rbegin());
}


void xml_dom_handler::end_element(const string_wrapper& name)
{
    levels_.pop_back();
}


void xml_dom_handler::characters(const string_wrapper& content)
{
    xml_node_t* current = levels_.back();
    current->set_text(current->get_text() + xml_string_t(content));
}


void xml_dom_handler::swap(xml_dom_handler& rhs)
{
    using PYCPP_NAMESPACE::swap;

    swap(root_, rhs.root_);
    swap(levels_, rhs.levels_);
}

// DOCUMENT

xml_document_t::xml_document_t(xml_document_t&& rhs):
    xml_node_t(move(rhs))
{}


xml_document_t& xml_document_t::operator=(xml_document_t&& rhs)
{
    xml_node_t::operator=(move(rhs));
    return *this;
}


void xml_document_t::loads(const string_wrapper& data)
{
    istringstream stream = istringstream(string(data));
    load(stream);
}


void xml_document_t::load(istream& stream)
{
    xml_stream_reader reader;
    xml_dom_handler handler(*this);
    reader.set_handler(handler);
    reader.open(stream);
}


void xml_document_t::load(const string_view& path)
{
    ifstream stream(path);
    load(stream);
}


#if defined(HAVE_WFOPEN)                        // WINDOWS

void xml_document_t::load(const wstring_view& path)
{
    ifstream stream(path);
    load(stream);
}


void xml_document_t::load(const u16string_view& path)
{
    ifstream stream(path);
    load(stream);
}

#endif                                          // WINDOWS


xml_string_t xml_document_t::dumps(char c, int width)
{
    ostringstream stream;
    dump(stream, c, width);
    return stream.str();
}


void xml_document_t::dump(ostream& stream, char c, int width)
{
    xml_stream_writer writer(stream, c, width);
    dump_impl(*this, writer);
}


void xml_document_t::dump(const string_view& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}


#if defined(HAVE_WFOPEN)                        // WINDOWS

void xml_document_t::dump(const wstring_view& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}


void xml_document_t::dump(const u16string_view& path, char c, int width)
{
    ofstream stream(path);
    dump(stream, c, width);
}

#endif                                          // WINDOWS

PYCPP_END_NAMESPACE
