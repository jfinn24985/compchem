#ifndef COMPCHEM_TABLE_WRITER_HPP
#define COMPCHEM_TABLE_WRITER_HPP

// ------------------------------------------------------------
// class: table_writer
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "config.hpp"
#include <boost/bind.hpp>

#include "utility/scoped_call.hpp"
#include <boost/iostreams/filtering_stream.hpp>

#include <boost/ptr_container/ptr_array.hpp>
#include <memory>

namespace report { namespace filetype { class basic_markup; }  } 

namespace report {

namespace filetype {

/**
 * This table_writer provides support for formatting the output of data
 * that follows a table concept. A table concept is a two dimensional 
 * array of types. This object allows inserting markers into the stream
 * around the table elements and a filter to transform the
 * table elements (eg escaping <, >, and & for HTML) without 
 * transforming the markup.
 * 
 * Minimum requirements:
 * iterator that follows input_iterator_tag
 * value_type supports the stream inserter (for arrays with multiple types see boost::variant)
 */
class table_writer
{
  public:
    typedef boost::iostreams::filtering_ostream filter_type;


  private:
    /**
     * The indices of m_marks
     */
    enum
    {
      start_table_index = 0,
      end_table_index,
      start_row_index,
      end_row_index,
      start_element_index,
      end_element_index

    };

    /**
     * The mark ups for various points in the table.
     */
    boost::ptr_array<basic_markup, 6> m_marks;

    /**
     * The output filter. Base operation is no-op.
     */
    std::auto_ptr<filter_type> m_filter;

    /**
     * Insert end-element mark into a_os.
     */
    void end_element(std::ostream & a_os);
    /**
     * Insert end-element mark into a_os.
     */
    void end_row(std::ostream & a_os, unsigned int a_width);

    /**
     * Insert end-element mark into a_os.
     */
    void end_table(std::ostream & a_os, unsigned int a_length, unsigned int a_width);


  public:
    /**
     * Get a table_writer for simple html table output.
     */
    static std::auto_ptr< table_writer > html_table_writer();


  private:
    /**
     * no assign
     */
    table_writer & operator=(const table_writer & a_source);


  public:
    /**
     * Remove any filter.
     */
    void remove_filter()
    {
      m_filter.reset ();
    }

    /**
     * Take possession of the end_element markup.
     * 
     * Exception safety: strong guarantee
     */
    void replace_end_element_markup(basic_markup * a_markup)
    {
      this->m_marks.replace< end_element_index > (a_markup);
    }

    /**
     * Take possession of the end_element markup.
     * 
     * Exception safety: strong guarantee
     */
    void replace_end_row_markup(basic_markup * a_markup)
    {
      this->m_marks.replace< end_row_index > (a_markup);
    }

    /**
     * Take possession of the end_element markup.
     * 
     * Exception safety: strong guarantee
     */
    void replace_end_table_markup(basic_markup * a_markup)
    {
      this->m_marks.replace< end_table_index > (a_markup);
    }

    /**
     * Take possession of the end_element markup.
     * 
     * Exception safety: strong guarantee
     */
    void replace_start_element_markup(basic_markup * a_markup)
    {
      this->m_marks.replace< start_element_index > (a_markup);
    }

    /**
     * Take possession of the end_element markup.
     * 
     * Exception safety: strong guarantee
     */
    void replace_start_row_markup(basic_markup * a_markup)
    {
      this->m_marks.replace< start_row_index > (a_markup);
    }

    /**
     * Take possession of the end_element markup.
     * 
     * Exception safety: strong guarantee
     */
    void replace_start_table_markup(basic_markup * a_markup)
    {
      this->m_marks.replace< start_table_index > (a_markup);
    }

    /**
     * Create a filter stream with a_filt.
     */
    template< typename Sink > void set_filter(const Sink & a_filt)
    {
      m_filter.reset (new boost::iostreams::filtering_ostream (a_filt));
    }


  private:
    /**
     * Insert end-element mark into a_os.
     */
    void start_element(std::ostream & a_os);
    /**
     * Insert end-element mark into a_os.
     */
    void start_row(std::ostream & a_os, unsigned int a_width);

    /**
     * Insert end-element mark into a_os.
     */
    void start_table(std::ostream & a_os, unsigned int a_length, unsigned int a_width);


  public:
    /**
     * Default ctor
     */
    table_writer();

    /**
     * Dtor
     */
    ~table_writer();


  private:
    /**
     * Copy is private. Swallows content of a_source.
     */
    table_writer(table_writer & a_source)
    : m_marks ()
    , m_filter (a_source.m_filter)
    {
      m_marks.swap (a_source.m_marks);
    }


  public:
    /**
     * Write the table using a_decorator
     * 
     * A table concept is a two dimensional array of types. 
     * Minimum requirements:
     * iterator that follows input_iterator_tag
     * value_type supports the stream inserter (for arrays with multiple types see boost::variant)
     * 
     * \pre a_width * a_length = distance (a_begin, a_end)
     */
    template< class InputIterator > inline void write_table(InputIterator a_begin, const InputIterator & a_end, size_t a_length, size_t a_width, std::ostream & a_os);


  private:
    /**
     * Write the table (private implementation)
     * 
     * A table concept is a two dimensional array of types. 
     * Minimum requirements:
     * iterator that follows input_iterator_tag
     * value_type supports the stream inserter (for arrays with multiple types see boost::variant)
     * 
     * \pre a_width * a_length = distance (a_begin, a_end)
     */
    template< class InputIterator > inline void write_table_priv(InputIterator a_begin, const InputIterator & a_end, size_t a_length, size_t a_width, std::ostream & a_os, std::ostream & a_filtos);


};
/**
 * Write the table using a_decorator
 * 
 * A table concept is a two dimensional array of types. 
 * Minimum requirements:
 * iterator that follows input_iterator_tag
 * value_type supports the stream inserter (for arrays with multiple types see boost::variant)
 * 
 * \pre a_width * a_length = distance (a_begin, a_end)
 */
template< class InputIterator > inline void table_writer::write_table(InputIterator a_begin, const InputIterator & a_end, size_t a_length, size_t a_width, std::ostream & a_os) 
{
  COMPCHEM_REQUIRE (a_width > 0, "can not have a table with width less than 1");
  COMPCHEM_REQUIRE (a_length > 0, "can not have a table with less than 1 one row");
  if (NULL != m_filter.get ())
  {
    m_filter->push (a_os); // Must succeed before we save pop
    utility::scoped_call l_pop (boost::bind (&boost::iostreams::filtering_ostream::pop, m_filter.get ()));
    a_os.tie (m_filter.get ()); // Must succeed before we save "untie" op.
    utility::scoped_call l_untie (boost::bind (&std::ostream::tie, &a_os, (std::basic_ostream<char, std::char_traits<char> >*)NULL));
    write_table_priv (a_begin, a_end, a_length, a_width, a_os, *m_filter.get ());
  }
  else
  {
    write_table_priv (a_begin, a_end, a_length, a_width, a_os, a_os);
  }

}

/**
 * Write the table (private implementation)
 * 
 * A table concept is a two dimensional array of types. 
 * Minimum requirements:
 * iterator that follows input_iterator_tag
 * value_type supports the stream inserter (for arrays with multiple types see boost::variant)
 * 
 * \pre a_width * a_length = distance (a_begin, a_end)
 */
template< class InputIterator > inline void table_writer::write_table_priv(InputIterator a_begin, const InputIterator & a_end, size_t a_length, size_t a_width, std::ostream & a_os, std::ostream & a_filtos) 
{
  this->start_table (a_os, a_length, a_width);
  if (1 == a_width) // Specialisation for lists.
  {
    while (a_begin != a_end)
    {
      this->start_row (a_os, 1);
      a_filtos << *a_begin;
      this->end_row (a_os, 1);
      ++a_begin;
    }
  }
  else if (2 == a_width) // specialisation two column tables
  {
    while (a_begin != a_end)
    {
      this->start_row (a_os, 2);
      a_filtos << *a_begin;
      this->end_element (a_os);
      ++a_begin;
      COMPCHEM_CHECK (a_begin != a_end, "table dimension/iterator distance mismatch");
      this->start_element (a_os);
      a_filtos << *a_begin;
      this->end_row (a_os, 2);
      ++a_begin;
    }
  }
  else if (3 == a_width) // specialisation three column tables
  {
    while (a_begin != a_end)
    {
      this->start_row (a_os, 3);
      a_filtos << *a_begin;
      this->end_element (a_os);
      ++a_begin;
      COMPCHEM_CHECK (a_begin != a_end, "table dimension/iterator distance mismatch");
      this->start_element (a_os);
      a_filtos << *a_begin;
      this->end_element (a_os);
      ++a_begin;
      COMPCHEM_CHECK (a_begin != a_end, "table dimension/iterator distance mismatch");
      this->start_element (a_os);
      a_filtos << *a_begin;
      this->end_row (a_os, 3);
      ++a_begin;
    }
  }
  else // General case
  {
    size_t index (0);
    while (a_begin != a_end)
    {
      if (a_width == index)
      {
        index = 0;
      }
      if (0 == index)
      {
        this->start_row (a_os, a_width);
        a_filtos << *a_begin;
        this->end_element (a_os);
      }
      else if (a_width - 1 == index)
      {
        this->start_element (a_os);
        a_filtos << *a_begin;
        this->end_row (a_os, a_width);
      }
      else
      {
        this->start_element (a_os);
        a_filtos << *a_begin;
        this->end_element (a_os);
      }
      ++a_begin;
      ++index;
    }
    COMPCHEM_CHECK (a_width == index, "table dimension/iterator distance mismatch");
  }
  this->end_table (a_os, a_length, a_width);

}


} // namespace report::filetype

} // namespace report
#endif
