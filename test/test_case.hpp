#ifndef _TEST_CASE_HPP_
#define _TEST_CASE_HPP_


#include <vector>
#include <map>

#include "./libyaml.hpp"

#include <ryml/ryml.hpp>
#include <gtest/gtest.h>

namespace c4 {
namespace yml {

struct Case;
struct CaseNode;
struct CaseData;

Case const* get_case(cspan name);
CaseData* get_data(cspan name);

void check_invariants(Tree const& t);
void check_invariants(NodeRef const& n);

void show_children(NodeRef const& p);

void print_node(NodeRef const& p, int level=0, bool print_children=false);
void print_tree(NodeRef const& p, int level=0);
void print_tree(Tree const& t);

void print_node(CaseNode const& t, int level = 0);
void print_tree(CaseNode const& p, int level = 0);


void print_path(NodeRef const& p);

struct TaggedScalar
{
    cspan tag;
    cspan scalar;
    template< size_t N, size_t M > TaggedScalar(const char (&t)[N], const char (&s)[M]) : tag(t), scalar(s) {}
};

/** a node class against which ryml structures are tested. Uses initializer
 * lists to facilitate minimal specification. */
struct CaseNode
{
public:

    using  seqmap = std::vector< CaseNode >;
    using iseqmap = std::initializer_list< CaseNode >;

    struct TaggedList
    {
        cspan tag;
        iseqmap ilist;
        template< size_t N > TaggedList(const char (&t)[N], iseqmap l) : tag(t), ilist(l) {}
    };

public:

    NodeType_e type;
    cspan      key, key_tag;
    cspan      val, val_tag;
    seqmap     children;
    CaseNode * parent;

public:

    CaseNode() : CaseNode(NOTYPE) {}
    CaseNode(NodeType_e t) : type(t), key(), key_tag(), val(), val_tag(), children(), parent(nullptr) { _set_parent(); }


    template< size_t N > explicit CaseNode(const char (&v)[N]   ) : type(             VAL        ), key(), key_tag(), val(v       ), val_tag(     ), children(), parent(nullptr) { _set_parent(); }
                         explicit CaseNode(TaggedScalar const& v) : type((NodeType_e)(VAL|VALTAG)), key(), key_tag(), val(v.scalar), val_tag(v.tag), children(), parent(nullptr) { _set_parent(); }

    template< size_t N > explicit CaseNode(NodeType_e t, const char (&v)[N]   ) : type((NodeType_e)(VAL|t     )), key(), key_tag(), val(v       ), val_tag(     ), children(), parent(nullptr) { _set_parent(); }
                         explicit CaseNode(NodeType_e t, TaggedScalar const& v) : type((NodeType_e)(VAL|VALTAG)), key(), key_tag(), val(v.scalar), val_tag(v.tag), children(), parent(nullptr) { _set_parent(); }


    template< size_t N, size_t M > explicit CaseNode(const char (&k)[N]   , const char (&v)[M]   ) : type((NodeType_e)(KEYVAL              )), key(k       ), key_tag(     ), val(v       ), val_tag(     ), children(), parent(nullptr) { _set_parent(); }
    template< size_t N >           explicit CaseNode(const char (&k)[N]   , TaggedScalar const& v) : type((NodeType_e)(KEYVAL|VALTAG       )), key(k       ), key_tag(     ), val(v.scalar), val_tag(v.tag), children(), parent(nullptr) { _set_parent(); }
    template< size_t M >           explicit CaseNode(TaggedScalar const& k, const char (&v)[M]   ) : type((NodeType_e)(KEYVAL|KEYTAG       )), key(k.scalar), key_tag(k.tag), val(v       ), val_tag(     ), children(), parent(nullptr) { _set_parent(); }
                                   explicit CaseNode(TaggedScalar const& k, TaggedScalar const& v) : type((NodeType_e)(KEYVAL|KEYTAG|VALTAG)), key(k.scalar), key_tag(k.tag), val(v.scalar), val_tag(v.tag), children(), parent(nullptr) { _set_parent(); }

    template< size_t N, size_t M > explicit CaseNode(NodeType_e t, const char (&k)[N]   , const char (&v)[M]   ) : type((NodeType_e)(KEYVAL|t              )), key(k       ), key_tag(     ), val(v       ), val_tag(     ), children(), parent(nullptr) { _set_parent(); }
    template< size_t N >           explicit CaseNode(NodeType_e t, const char (&k)[N]   , TaggedScalar const& v) : type((NodeType_e)(KEYVAL|VALTAG|t       )), key(k       ), key_tag(     ), val(v.scalar), val_tag(v.tag), children(), parent(nullptr) { _set_parent(); }
    template< size_t M >           explicit CaseNode(NodeType_e t, TaggedScalar const& k, const char (&v)[M]   ) : type((NodeType_e)(KEYVAL|KEYTAG|t       )), key(k.scalar), key_tag(k.tag), val(v       ), val_tag(     ), children(), parent(nullptr) { _set_parent(); }
                                   explicit CaseNode(NodeType_e t, TaggedScalar const& k, TaggedScalar const& v) : type((NodeType_e)(KEYVAL|KEYTAG|VALTAG|t)), key(k.scalar), key_tag(k.tag), val(v.scalar), val_tag(v.tag), children(), parent(nullptr) { _set_parent(); }


    template< size_t N > explicit CaseNode(const char (&k)[N]   , iseqmap    s) : type(), key(k       ), key_tag(     ), val(), val_tag(     ), children(s      ), parent(nullptr) { _set_parent(); type = _guess(); }
    template< size_t N > explicit CaseNode(const char (&k)[N]   , TaggedList s) : type(), key(k       ), key_tag(     ), val(), val_tag(s.tag), children(s.ilist), parent(nullptr) { _set_parent(); type = _guess(); }
                         explicit CaseNode(TaggedScalar const& k, iseqmap    s) : type(), key(k.scalar), key_tag(k.tag), val(), val_tag(     ), children(s      ), parent(nullptr) { _set_parent(); type = _guess(); }
                         explicit CaseNode(TaggedScalar const& k, TaggedList s) : type(), key(k.scalar), key_tag(k.tag), val(), val_tag(s.tag), children(s.ilist), parent(nullptr) { _set_parent(); type = _guess(); }
                         explicit CaseNode(                       iseqmap    m) : CaseNode("", m) {}
                         explicit CaseNode(                       TaggedList m) : CaseNode("", m) {}

    template< size_t N > explicit CaseNode(NodeType_e t, const char (&k)[N]   , iseqmap    s) : type(t                     ), key(k       ), key_tag(     ), val(), val_tag(     ), children(s      ), parent(nullptr) { _set_parent(); }
    template< size_t N > explicit CaseNode(NodeType_e t, const char (&k)[N]   , TaggedList s) : type(t                     ), key(k       ), key_tag(     ), val(), val_tag(s.tag), children(s.ilist), parent(nullptr) { _set_parent(); }
                         explicit CaseNode(NodeType_e t, TaggedScalar const& k, iseqmap    s) : type((NodeType_e)(t|KEYTAG)), key(k.scalar), key_tag(k.tag), val(), val_tag(     ), children(s      ), parent(nullptr) { _set_parent(); }
                         explicit CaseNode(NodeType_e t,                        iseqmap    s) : CaseNode(t, "", s) {}
                         explicit CaseNode(NodeType_e t,                        TaggedList s) : CaseNode(t, "", s) {}


    CaseNode(CaseNode     && that) { _move(std::move(that)); }
    CaseNode(CaseNode const& that) { _copy(that); }

    CaseNode& operator= (CaseNode     && that) { _move(std::move(that)); return *this; }
    CaseNode& operator= (CaseNode const& that) { _copy(that); return *this; }

public:

    void _move(CaseNode&& that)
    {
        type = that.type;
        key = that.key;
        key_tag = that.key_tag;
        val = that.val;
        val_tag = that.val_tag;
        children = std::move(that.children);
        parent = nullptr;
        _set_parent();
    }
    void _copy(CaseNode const& that)
    {
        type = that.type;
        key = that.key;
        key_tag = that.key_tag;
        val = that.val;
        val_tag = that.val_tag;
        children = that.children;
        parent = nullptr;
        _set_parent();
    }

    void _set_parent()
    {
        for(auto &ch : children)
        {
            ch.parent = this;
        }
    }

    NodeType_e _guess() const
    {
        NodeType_e t;
        C4_ASSERT(val.empty() != children.empty());
        if(children.empty())
        {
            C4_ASSERT(parent);
            if(key.empty())
            {
                t = VAL;
            }
            else
            {
                t = KEYVAL;
            }
        }
        else
        {
            NodeType_e has_key = key.empty() ? NOTYPE : KEY;
            auto const& ch = children.front();
            if(ch.key.empty())
            {
                t = (NodeType_e)(has_key|SEQ);
            }
            else
            {
                t = (NodeType_e)(has_key|MAP);
            }
        }
        if( ! key_tag.empty())
        {
            C4_ASSERT( ! key.empty());
            t = (NodeType_e)(t|KEYTAG);
        }
        if( ! val_tag.empty())
        {
            C4_ASSERT( ! val.empty() || ! children.empty());
            t = (NodeType_e)(t|VALTAG);
        }
        return t;
    }

    bool is_root() const { return parent; }
    bool is_doc() const { return type & DOC; }
    bool is_map() const { return type & MAP; }
    bool is_seq() const { return type & SEQ; }
    bool has_val() const { return type & VAL; }
    bool has_key() const { return type & KEY; }
    bool is_container() const { return type & (SEQ|MAP); }

public:

    CaseNode const& operator[] (size_t i) const
    {
        C4_ASSERT(i >= 0 && i < children.size());
        return children[i];
    }

    CaseNode const& operator[] (cspan const& name) const
    {
        auto ch = lookup(name);
        C4_ASSERT(ch != nullptr);
        return *ch;
    }

    CaseNode const* lookup(cspan const& name) const
    {
        C4_ASSERT( ! children.empty());
        for(auto const& ch : children)
        {
            if(ch.key == name)
            {
                return &ch;
            }
        }
        return nullptr;
    }

public:

    void compare(yml::NodeRef const& n) const;
    void compare_child(yml::NodeRef const& n, size_t pos) const;

    size_t reccount() const
    {
        size_t c = 1;
        for(auto const& ch : children)
        {
            c += ch.reccount();
        }
        return c;
    }

    void recreate(yml::NodeRef *n) const;

};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
typedef enum {
    IGNORE_LIBYAML_PARSE_FAIL = (1<<0),
    IGNORE_YAMLCPP_PARSE_FAIL = (1<<1),
    RESOLVE_REFS = (1<<2),
} TestCaseFlags_e;

struct Case
{
    cspan name;
    cspan src;
    CaseNode root;
    TestCaseFlags_e flags;

    template< size_t N, class... Args >
    Case(cspan const& n, const char (&s)[N], Args&& ...args)
        : name(n), src(s), root(std::forward< Args >(args)...), flags()
    {
    }

    template< size_t N, class... Args >
    Case(cspan const& n, int f_, const char (&s)[N], Args&& ...args)
        : name(n), src(s), root(std::forward< Args >(args)...), flags((TestCaseFlags_e)f_)
    {
    }

};

//-----------------------------------------------------------------------------

// a persistent data store to avoid repeating operations on every test
struct CaseData
{
    std::vector< char > src_buf;
    span src;

    Tree parsed_tree;

    size_t numbytes_stdout;
    std::vector< char > emit_buf;
    cspan emitted_yml;
    std::vector< char > parse_buf;
    span parsed_yml;

    Tree emitted_tree;

    Tree recreated;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// a fixture for running the tests
struct YmlTestCase : public ::testing::TestWithParam< const char* >
{
    cspan const name;
    Case const* c; // the case
    CaseData * d; // the case data

    YmlTestCase() : name(GetParam()), c(get_case(GetParam())), d(get_data(GetParam())) {}

    void SetUp() override
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
        std::cout << "-------------------------------------------\n";
        std::cout << "running test case '" << name << "'\n";
        std::cout << "-------------------------------------------\n";
    }
};


} // namespace yml
} // namespace c4

#endif /* _TEST_CASE_HPP_ */
