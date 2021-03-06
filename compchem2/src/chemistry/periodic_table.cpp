// ------------------------------------------------------------
// class: periodic_table
// Orig author: Justin Finnerty
// $author:$
// Orig date: 
// $date:$
// $revision:$
// $log:$
// ------------------------------------------------------------


#include "chemistry/periodic_table.hpp"

namespace chemistry {

/**
 * Private Singleton ctor
 */
periodic_table::periodic_table()
: m_last_element (54)
, m_elements ()
{
m_elements [  1 - 1] = element (  1, "hydrogen",   "H");
m_elements [  2 - 1] = element (  2, "helium",     "He");
m_elements [  3 - 1] = element (  3, "lithium",    "Li");
m_elements [  4 - 1] = element (  4, "berylium",   "Be");
m_elements [  5 - 1] = element (  5, "boron",      "B");
m_elements [  6 - 1] = element (  6, "carbon",     "C");
m_elements [  7 - 1] = element (  7, "nitrogen",   "N");
m_elements [  8 - 1] = element (  8, "oxygen",     "O");
m_elements [  9 - 1] = element (  9, "fluorine",   "F");
m_elements [ 10 - 1] = element ( 10, "neon",       "Ne");
m_elements [ 11 - 1] = element ( 11, "sodium",     "Na");
m_elements [ 12 - 1] = element ( 12, "magnesium",  "Mg");
m_elements [ 13 - 1] = element ( 13, "aluminium",  "Al");
m_elements [ 14 - 1] = element ( 14, "silicon",    "Si");
m_elements [ 15 - 1] = element ( 15, "phosphorous", "P");
m_elements [ 16 - 1] = element ( 16, "sulfur",      "S");
m_elements [ 17 - 1] = element ( 17, "chlorine",    "Cl");
m_elements [ 18 - 1] = element ( 18, "argon",       "Ar");
m_elements [ 19 - 1] = element ( 19, "potassium",   "K");
m_elements [ 20 - 1] = element ( 20, "calcium",     "Ca");
m_elements [ 21 - 1] = element ( 21, "scandium",    "Sc");
m_elements [ 22 - 1] = element ( 22, "titanium",    "Ti");
m_elements [ 23 - 1] = element ( 23, "vanadium",    "V");
m_elements [ 24 - 1] = element ( 24, "chromium",    "Cr");
m_elements [ 25 - 1] = element ( 25, "manganese",   "Mn");
m_elements [ 26 - 1] = element ( 26, "iron",        "Fe");
m_elements [ 27 - 1] = element ( 27, "cobalt",      "Co");
m_elements [ 28 - 1] = element ( 28, "nickel",      "Ni");
m_elements [ 29 - 1] = element ( 29, "copper",      "Cu");
m_elements [ 30 - 1] = element ( 30, "zinc",        "Zn");
m_elements [ 31 - 1] = element ( 31, "gallium",     "Ga");
m_elements [ 32 - 1] = element ( 32, "germanium",   "Ge");
m_elements [ 33 - 1] = element ( 33, "astatine",    "As");
m_elements [ 34 - 1] = element ( 34, "serium",      "Se");
m_elements [ 35 - 1] = element ( 35, "bromine",     "Br");
m_elements [ 36 - 1] = element ( 36, "krypton",     "Kr");
m_elements [ 37 - 1] = element ( 37, "rhubidium",   "Rb");
m_elements [ 38 - 1] = element ( 38, "strontium",   "Sr");
m_elements [ 39 - 1] = element ( 39, "yttrium",     "Y");
m_elements [ 40 - 1] = element ( 40, "zirconium",   "Zr");
m_elements [ 41 - 1] = element ( 41, "nobelium",    "Nb");
m_elements [ 42 - 1] = element ( 42, "molybdenum",  "Mo");
m_elements [ 43 - 1] = element ( 43, "technicium",  "Tc");
m_elements [ 44 - 1] = element ( 44, "rubidium",    "Ru");
m_elements [ 45 - 1] = element ( 45, "rhodium",     "Rh");
m_elements [ 46 - 1] = element ( 46, "palladium",   "Pd");
m_elements [ 47 - 1] = element ( 47, "silver",      "Ag");
m_elements [ 48 - 1] = element ( 48, "cadmium",     "Cd");
m_elements [ 49 - 1] = element ( 49, "indium",	 "In");
m_elements [ 50 - 1] = element ( 50, "strontium",   "Sn");
m_elements [ 51 - 1] = element ( 51, "sb",          "Sb");
m_elements [ 52 - 1] = element ( 52, "tellurium",   "Te");
m_elements [ 53 - 1] = element ( 53, "iodine",      "I");
m_elements [ 54 - 1] = element ( 54, "xeon",        "Xe");
}

/**
 * Get the periodic table.
 */
const periodic_table& periodic_table::table()

{
  static std::auto_ptr< periodic_table > s_table;
  if (NULL == s_table.get ())
  {
    s_table.reset (new periodic_table);
  }
  return *s_table.get ();

}


} // namespace chemistry
