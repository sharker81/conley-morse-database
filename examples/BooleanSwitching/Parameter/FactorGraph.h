
/// FactorGraph.h
/// Author: Shaun Harker

#ifndef BOOLEANSWITCHINGFACTORGRAPH_H
#define BOOLEANSWITCHINGFACTORGRAPH_H

#include <vector>
#include "boost/unordered_map.hpp"
#include "boost/foreach.hpp"
#include "boost/shared_ptr.hpp"

#include "Parameter/MonotonicMap.h"

/// class ConnectedSmartGraph where vertices are smart in the sense that they
///  (a) supply neighbors via a method "neighbors"
///  (b) the graph is connected
template < class T >
class ConnectedSmartGraph {
public:
  // gives indexing
  boost::unordered_map<T, int> preorder;
  // array of vertices
  std::vector<T> vertices;
  // optional
  std::vector<std::vector< int > > adjacencies_;

  size_t size ( void ) const {
    return vertices . size ();
  }

  void construct ( const T & start ) {
    vertices . push_back ( start );
    preorder [ start ] = 0;

    std::stack<T> dfs_stack;
    dfs_stack . push ( start );

    //size_t count = 0;
    while ( not dfs_stack . empty () ) {
      //std::cout << ++ count << "   and stack size is " << dfs_stack . size () << "\n";
      T vertex = dfs_stack . top ();
      dfs_stack . pop ();
      std::vector<boost::shared_ptr<T> > neighbors = vertex . neighbors ();
      //std::cout << "Found " << neighbors . size () << " neighbors.\n";
      BOOST_FOREACH ( boost::shared_ptr<T> ptr, neighbors ) {
        if ( preorder . count ( *ptr ) ) continue;
        preorder [ * ptr ] = vertices . size ();
        vertices . push_back ( * ptr );
        dfs_stack . push ( * ptr );
      }
    }

    compute_adjacencies ();
    
    //std::cout << "FactorGraph. Number of vertices = " << vertices . size () << "\n";
    //std::cout << " Starting vertex was " << start << "\n";
  }

  void compute_adjacencies ( void ) {
    adjacencies_ . resize ( vertices . size () );
    for ( int v = 0; v < vertices . size (); ++ v ) {
      T vertex = vertices [ v ];
      std::vector<boost::shared_ptr<T> > neighbors = vertex . neighbors ();
      BOOST_FOREACH ( boost::shared_ptr<T> ptr, neighbors ) {
        adjacencies_ [ v ] . push_back ( preorder [ *ptr ] );
      }
    }
  }

  const std::vector<int> & adjacencies ( int v ) const {
    return adjacencies_ [ v ];
  }

  void saveToFile ( void ) const {
    std::ofstream outfile ( "graph.gv" );
    outfile << "graph factorgraph {\n";
    for ( int v = 0; v < vertices . size (); ++ v ) {
      const T & vertex = vertices [ v ];
      std::vector<boost::shared_ptr<T> > neighbors = vertex . neighbors ();
      BOOST_FOREACH ( boost::shared_ptr<T> ptr, neighbors ) {
        int u = preorder . find ( * ptr) -> second;
        if ( v < u ) {
          outfile << v << " -- " << u << "\n";
        }
      }
    }
    outfile << "}\n\n";
  }

};

// Specialization of ConnectedSmartGraph to MonotonicMap smart vertices
typedef ConnectedSmartGraph<MonotonicMap> FactorGraph;

#endif

