// // [[Rcpp::interfaces(r, cpp)]]
// // [[Rcpp::plugins("cpp11")]]
// #include <Rcpp.h>
// #include <vector>
// // put any other includes you need here.
// 
// using namespace Rcpp;
// //' intersectPolygons
// //' 
// //' Takes a set of 5 convex polygons from R and returns the intersection of each pair.
// //' 
// //' 
// //' @param listOfPolygons An R list with 3 to 5 convex polygons. Each polygon is one
// //'     element of the list and is stored as an R matrix. The x-coordinates for 
// //'     the polygon are the first column, and the y-coordinates are the second
// //'     column.  
// //'   
// //'   
// //' @return intersections A List where each element is the intersection of two of 
// //'     the provided polygons. For example: If there are 5 polygons the list will contain 10 elements.
// //'     Each element should be NumericalMatrix with x-coordinates as the first
// //'     column and y-coordinates as the second column.  
// //'     
// //'     The elements of the list should be named based on the two polygons
// //'     that were intersected to find that particular element. For example
// //'     when listOfPolygons[[1]] and listOfPolygons[[2]] are intersected, 
// //'     the resulting polygon should be stored in the list as "1_intersect_2".
// //' 
// //' @export
// //'

// We can now use the BH package
// [[Rcpp::depends(BH)]]

#include <Rcpp.h>
using namespace Rcpp;

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)
  
typedef boost::tuple<double, double> point;
typedef boost::geometry::model::polygon<point, true, true> polygon; 

namespace Rcpp {

// `as` converter from R to Boost.Geometry's polygon type
template <> polygon as(SEXP pointsMatrixSEXP) {
  // the coordinates are the rows of the (n x 2) matrix
  NumericMatrix pointsMatrix(pointsMatrixSEXP);
  polygon poly;
  for (int i = 0; i < pointsMatrix.nrow(); ++i) {
    double x = pointsMatrix(i,0);
    double y = pointsMatrix(i,1);
    point p(x,y);
    poly.outer().push_back(p); 
  }
  return (poly);
} 



// `wrap` converter from Boost.Geometry's polygon to an R(cpp) matrix
// The Rcpp NumericMatrix can be converted to/from a SEXP
template <> SEXP wrap(const polygon& poly) {
  const std::vector<point>& points = poly.outer();
  NumericMatrix rmat(points.size(), 2);
  for(unsigned i = 0; i < points.size(); ++i) {
    const point& p = points[i];
    rmat(i,0) = p.get<0>();
    rmat(i,1) = p.get<1>();
  }
  return Rcpp::wrap(rmat);
}
}




// [[Rcpp::export]]
NumericMatrix intersectionTwoPolygon(SEXP pointsMatrixSEXP1, SEXP pointsMatrixSEXP2){
  
  // Conversion of pointsMatrix here to boost::geometry polygon
  polygon poly1 = as<polygon>(pointsMatrixSEXP1);
  polygon poly2 = as<polygon>(pointsMatrixSEXP2);
  
  std::deque<polygon> output;
  
  boost::geometry::intersection(poly1, poly2, output);
  
  // Convert output polygon into a NumericMatrixsomething that Rcpp can hand back to R
  return wrap(output[0]);
}


// [[Rcpp::export]]
List findIntersections(List listOfPolygons){
  
  
  // Here is where you write the code that uses boost::geometry::intersection() to
  // intersect all possible pairs of the provided polygons.
  // The intersection of a pair ( example: listOfPolygons[[1]] and listOfPolygons[[2]] )
  // should be stored as a matrix, with the x-coorinates as the first column and the
  // y-coordinates as the second column (in the same way as the polygons that served as the inputs). 
  // The matrix should be stored in a list for output purposes. Each matrix in the list should
  // be named according to the two polygons that were interesected to generate the matrix. For example
  // when listOfPolygons[[1]] and listOfPolygons[[2]] are intersected, 
  // the resulting polygon should be stored in the list as "1_intersect_2". Or simply "1_2". 
  
  List outListOfPolygons; 
  
  for (unsigned i=0; i<listOfPolygons.size()-1; i++) {
    for (unsigned j=i+1; j<listOfPolygons.size(); j++) {
      
      // Conversion of pointsMatrix here to boost::geometry polygon
      polygon poly1 = as<polygon>(listOfPolygons[i]);
      polygon poly2 = as<polygon>(listOfPolygons[j]);
      
      //intersection of two polygons
      std::deque<polygon> output;
      boost::geometry::intersection(poly1, poly2, output);
      
      // create name of the polygon result
      std::string name;
      name =  boost::lexical_cast<std::string>(i+1)+"_intersect_" +boost::lexical_cast<std::string>(j+1);
      
      // checking empty result
      if( output.size() <= 0)
        outListOfPolygons[name] = Rcpp::wrap(NumericMatrix());
      
      else
        // Convert output polygon into a NumericMatrix
        outListOfPolygons[name] =  wrap(output[0]);
    }
  }
  

  return outListOfPolygons;
}
