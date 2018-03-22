// #include "log_manager.hpp"
// #include "log_entry.hpp"
// #include "log_experiment.hpp"

int main(int argc
     , char** argv)
{
  experiment_log::journal l_log_man;
  l_log_man.get_view ().update ();
  // create and open a character archive for output
  std::ofstream l_ofstream ("filename");
  boost::archive::text_oarchive l_oarchive (l_ofstream);

  // write class instance to archive
  l_oarchive << l_log_man;
  // close archive
  l_ofstream.close ();

  // ... some time later restore the class instance to its orginal state
  // create and open an archive for input
  std::ifstream l_ifstream("filename", std::ios::binary);
  boost::archive::text_iarchive l_iarchive(l_ifstream);
  // read class state from archive
  l_iarchive >> l_log_man;
  l_log_man.get_view ().update ();
  // close archive
  l_ifstream.close();
  return 0;
}
