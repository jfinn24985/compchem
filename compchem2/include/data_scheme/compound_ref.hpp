#ifndef COMPCHEM_DATA_SCHEME_COMPOUND_REF_HPP
#define COMPCHEM_DATA_SCHEME_COMPOUND_REF_HPP

// ------------------------------------------------------------
// class: compound_ref
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include "data_scheme/basic_datum.hpp"
#include <string>

namespace data_scheme { class project; } 
namespace data_scheme { class comment; } 
namespace data_scheme { class processing_instruction; } 
namespace data_scheme { class text; } 
namespace data_scheme { class visitor; } 

namespace data_scheme {

class compound_ref : public basic_datum
{
  private:
    /**
     * The set of formulae to import, when empty import all.
     */
    std::string m_formulae;

    /**
     * The importer of the compound referred to.
     */
    project & m_importer;

    /**
     * The label of the compound to import.
     */
    std::string m_label;


  public:
    /**
     * Throws an error as no children are allowed. 
     */
    comment & add_comment_swallow(std::string & a_value);

    void add_formula_label(const std::string & a_set);

    /**
     * Throws an error as no children are allowed. 
     */
    processing_instruction & add_pi_swallow(const std::string & a_name, std::string & a_value);

    /**
     * Throws an error as no children are allowed. 
     */
    text & add_text_swallow(std::string & a_value);

    /**
     * Get the label of the compound to import.
     */
    const std::string & compound_label() const throw ()
    {
      return m_label;
    }

    /**
     * Construct a new compound-ref object, swallow a_label
     * 
     * \post a_label.empty
     */
    compound_ref(project & a_parent, std::string & a_label);

    ~compound_ref() throw ();


  private:
    /**
     * no copy
     */
    compound_ref(const compound_ref & source);


  public:
    /**
     * Get the list of formulae to import
     */
    const std::string & formulae() const throw ()
    {
      return m_formulae;
    }

    /**
     * Add attributes to element writer.
     * 
     * No default implementation.
     */
    bool get_attribute(const std::string & a_name, std::string & a_value) const;


  private:
    /**
     * no assign
     */
    compound_ref & operator=(const compound_ref & source);


  public:
    /**
     * Get a reference to the project this is part of.
     */
    const project & section() const throw ()
    {
      return m_importer;
    }

    /**
     * The XML element definition for this datum.
     */
    std::string serial_name() const;

    /**
     * Write an attribute.  Throw an error if arguments are incorrect or if no 
     * attributes are allowed.
     */
    void set_attribute(const std::string & a_name, std::string a_value);

    void visit(visitor & a_visitor) const;


};

} // namespace data_scheme
#endif
