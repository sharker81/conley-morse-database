/* Database */

#ifndef CMDP_DATABASE
#define CMDP_DATABASE

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/utility.hpp>

#include "chomp/ConleyIndex.h"

/* File Format.
 
 Parameter Box Record
 1. A unique integer identifier
 2. Grid Element
 3. A list of grid elements to identify Morse Sets
 4. A list of directed edges to give partial order information
 
 Grid Element Record
 1. Array of Doubles
 
 Clutching Record
 1. Two Parameter Box Record / Morse Graph Record id's
 2. A (possibly empty?) list of undirected edges
 
 */

struct GridElementRecord {
  std::vector < double > lower_bounds_;
  std::vector < double > upper_bounds_;
  GridElementRecord ( void );
  GridElementRecord ( int dimension );
  template < class Geometric >
  GridElementRecord ( const Geometric & g );
  
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      //std::cout << "Serialize lower bounds\n";
    ar & lower_bounds_;
      //std::cout << "Serialize upper bounds\n";
    ar & upper_bounds_;
  }
};

struct ParameterBoxRecord {
  int id_;
  GridElementRecord ge_;
  int num_morse_sets_;
  //std::list < GridElementRecord > morse_set_tags_;
  //std::vector < int > ms_sizes_;
  std::list < std::pair < int, int > > partial_order_;

  // Constructor
  ParameterBoxRecord ( void );
  template < class Geometric, class CMG >
  ParameterBoxRecord ( int id_, const Geometric & g, const CMG & cmg);
  bool operator < ( const ParameterBoxRecord & rhs ) const;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      //std::cout << "Serialize box record.\n";
      //std::cout << "Serialize ID\n";
    ar & id_;
      //std::cout << "Serialize Grid Element\n";
    ar & ge_;
      //std::cout << "Serialize number of morse sets\n";
    ar & num_morse_sets_;
      //std::cout << "Serialize partial order\n";
    ar & partial_order_;
  }
};

struct ClutchingRecord {
  int id1_, id2_;
  std::list < std::pair < int, int > > clutch_;
  bool operator < ( const ClutchingRecord & rhs ) const;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      //std::cout << "Serialize clutch record.\n";
      //std::cout << "Serialize ID\n";
    ar & id1_;
      //std::cout << "Serialize ID\n";
    ar & id2_;
      //std::cout << "Serialize clutch list\n";
    ar & clutch_;
  }
};

struct ConleyRecord {
  std::pair < int, int > id_;
  chomp::ConleyIndex_t ci_;
  bool operator < ( const ConleyRecord & rhs ) const;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      //std::cout << "Serialize conley record.\n";
      //std::cout << "Serialize ID\n";
    ar & id_;
      //std::cout << "Serialize Conley Index\n";

    ar & ci_;
  }
};

class Database {
private:
  friend class boost::serialization::access;
  std::set < ParameterBoxRecord > box_records_;
  std::set < ClutchingRecord > clutch_records_;
  std::set < ConleyRecord > conley_records_;

public:
  void merge ( const Database & other );
  void insert ( const ParameterBoxRecord & record );
  void insert ( const ClutchingRecord & record );
  void insert ( const ConleyRecord & record );
  void save ( const char * filename );
  void load ( const char * filename );
  std::set < ParameterBoxRecord > & box_records ( void );
  std::set < ClutchingRecord > & clutch_records ( void );
  std::set < ConleyRecord > & conley_records ( void );
  const std::set < ParameterBoxRecord > & box_records ( void ) const;
  const std::set < ClutchingRecord > & clutch_records ( void ) const;
  const std::set < ConleyRecord > & conley_records ( void ) const;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
      //std::cout << "Serialize box records.\n";
    ar & box_records_;
      //std::cout << "Serialize clutch records.\n";

    ar & clutch_records_;
      //std::cout << "Serialize conley records.\n";

    ar & conley_records_;
    
      //int x = 753;
      
      //ar & x;
      //std::cout << "Seralize the number " << x << "\n";
  }
};

#include "database/structures/Database.hpp"

#endif
