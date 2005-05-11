// $Id: cell_hex27.h,v 1.10 2005-05-11 18:31:16 roystgnr Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2005  Benjamin S. Kirk, John W. Peterson
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __cell_hex27_h__
#define __cell_hex27_h__

// C++ includes

// Local includes
#include "cell_hex.h"




/**
 * The \p Hex27 is an element in 3D composed of 27 nodes.
 * It is numbered like this:
   \verbatim
   HEX27:      7              18             6     			      
               o--------------o--------------o     			      
              /:             /              /|     			      
             / :            /              / |     			      
            /  :           /              /  |     			      
         19/   :        25/            17/   |     			      
          o--------------o--------------o    |     			      
         /     :        /              /|    |     			      
        /    15o       /    23o       / |  14o     
       /       :      /              /  |   /|     
     4/        :   16/             5/   |  / |     
     o--------------o--------------o    | /  |     			      
     |         :    |   26         |    |/   |     
     |  24o    :    |    o         |  22o    |     
     |         :    |       10     |   /|    |                                
     |        3o....|.........o....|../.|....o     
     |        .     |              | /  |   / 2      
     |       .    21|            13|/   |  /        
  12 o--------------o--------------o    | /         
     |     .        |              |    |/          
     |  11o         | 20o          |    o           
     |   .          |              |   / 9          
     |  .           |              |  /             
     | .            |              | /              
     |.             |              |/               
     o--------------o--------------o                
     0              8              1                
   \endverbatim
 */

// ------------------------------------------------------------
// Hex27 class definition
class Hex27 : public Hex
{
public:

  /**
   * Constructor.  By default this element has no parent.
   */
  Hex27  (const Elem* p=NULL);
  
  /**
   * @returns \p HEX27
   */
  ElemType     type ()   const { return HEX27; }

  /**
   * @returns 27
   */
  unsigned int n_nodes() const { return 27; }
  
  /**
   * @returns 8
   */
  unsigned int n_sub_elem() const { return 8; }
  
  /**
   * @returns true iff the specified (local) node number is a vertex.
   */
  virtual bool is_vertex(const unsigned int i) const;

  /**
   * @returns true iff the specified (local) node number is an edge.
   */
  virtual bool is_edge(const unsigned int i) const;

  /**
   * @returns true iff the specified (local) node number is a face.
   */
  virtual bool is_face(const unsigned int i) const;
  
  /*
   * @returns true iff the specified (local) node number is on the
   * specified side
   */
  virtual bool is_node_on_side(const unsigned int n,
			       const unsigned int s) const;
  
  /*
   * @returns true iff the specified (local) node number is on the
   * specified edge
   */
  virtual bool is_node_on_edge(const unsigned int n,
			       const unsigned int e) const;

  /**
   * @returns SECOND
   */
  Order default_order() const { return SECOND; }

  /**
   * @returns an id associated with the \p s side of this element.
   * The id is not necessariy unique, but should be close.  This is
   * particularly useful in the \p MeshBase::find_neighbors() routine.
   *
   * We reimplemenet this method here for the \p Hex27 since we can
   * use the center node of each face to provide a perfect (unique)
   * key.
   */
  unsigned int key (const unsigned int s) const;
  
  /**
   * Builds a \p QUAD9 built coincident with face i.  
   * The \p AutoPtr<Elem> handles the memory aspect.
   */
  AutoPtr<Elem> build_side (const unsigned int i) const;

  /**
   * Builds a \p EDGE3 built coincident with edge i.  
   * The \p AutoPtr<Elem> handles the memory aspect.
   */
  AutoPtr<Elem> build_edge (const unsigned int i) const;

  virtual void connectivity(const unsigned int sc,
			    const IOPackage iop,
			    std::vector<unsigned int>& conn) const;

  /**
   * @returns 2 for all edge nodes, 4 for all face nodes, and
   * 8 for the bubble node
   */
  unsigned int n_second_order_adjacent_vertices (const unsigned int) const;

  /**
   * @returns the element-local number of the  \f$ v^{th} \f$ vertex
   * that defines the \f$ n^{th} \f$ second-order node.
   * Note that \p n is counted as depicted above, \f$ 8 \le n < 27 \f$.
   */
  unsigned short int second_order_adjacent_vertex (const unsigned int n,
						   const unsigned int v) const;

  /**
   * This maps the \f$ j^{th} \f$ node of the \f$ i^{th} \f$ side to
   * element node numbers.
   */
  static const unsigned int side_nodes_map[6][9];

  /**
   * This maps the \f$ j^{th} \f$ node of the \f$ i^{th} \f$ edge to
   * element node numbers.
   */
  static const unsigned int edge_nodes_map[12][3];

  
protected:
  
  
#ifdef ENABLE_AMR

  /**
   * Matrix used to create the elements children.
   */
  float embedding_matrix (const unsigned int i,
			  const unsigned int j,
			  const unsigned int k) const
  { return _embedding_matrix[i][j][k]; }

  /**
   * Matrix that computes new nodal locations/solution values
   * from current nodes/solution.
   */
  static const float _embedding_matrix[8][27][27];
  
#endif


private:
  
  /**
   * Matrix that tells which vertices define the location
   * of mid-side (or second-order) nodes.  This matrix only
   * covers the nodes that are unique to \p Hex27, while the
   * second-order-nodes that are identical with \p Hex20 are covered
   * through the \p _second_order_adjacent_vertices matrix in
   * \p cell_hex.C.  Note that this matrix also does @e not 
   * cover the bubble node.  The interpolation
   * is trivial and would only blow up the size of this
   * matrix.
   */
  static const unsigned short int _remaining_second_order_adjacent_vertices[6][4];

};



// ------------------------------------------------------------
// Hex27 class member functions
inline
Hex27::Hex27(const Elem* p) :
  Hex(Hex27::n_nodes(), p) 
{
}


#endif
