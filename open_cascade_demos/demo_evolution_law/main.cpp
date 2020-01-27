// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a Bezier curve in OpenCascade
//  Author: Roberto Agromayor
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sys/stat.h>


// Include OpenCascade libraries
#include <gp_Pnt.hxx>
#include <Law_BSpline.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <STEPControl_Writer.hxx>


// Define namespaces
using namespace std;


// ------------------------------------------------------------------------------------------------------------------ //
// Main body
// ------------------------------------------------------------------------------------------------------------------ //
int main() {


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the array of control points
    // -------------------------------------------------------------------------------------------------------------- //

    TColStd_Array1OfReal P(0, 4);
    P(0) = 0.0;
    P(1) = 2.0;
    P(2) = 3.0;
    P(3) = 1.0;
    P(4) = 1.0;


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the knot vector
    // -------------------------------------------------------------------------------------------------------------- //

    // Maximum index of the control points (counting from zero)
    Standard_Integer n = P.Length() - 1;

    // Define the order of the basis polynomials
    // Linear (p = 1), Quadratic (p = 2), Cubic (p = 3), etc.
    // Set p = n (number of control points minus one) to obtain Bezier basis polynomials
    Standard_Integer p = 3;

    // Define of the knot vector (clamped spline)
    // p+1 zeros, n-p equispaced points between 0 and 1, and p+1 ones. N+1=n-p+2 distinct values
    Standard_Integer N = n - p + 2 - 1;
    TColStd_Array1OfReal U_values(0, N);
    TColStd_Array1OfInteger U_mults(0, N);

    for (int i = 0; i <= N; ++i) {

        // Set the knot values
        U_values(i) = double(i) / double(N);

        // Set the knot multiplicities
        if (i == 0 || i == N) { U_mults(i) = p+1; }
        else { U_mults(i) = 1; }

    }



    // -------------------------------------------------------------------------------------------------------------- //
    // Create the B-Spline evolution law
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a Law_BSpline object referenced by value
    Law_BSpline bsplineLaw(P, U_values, U_mults, p);

    // Create a Law_BSpline object referenced by handle
    Handle(Law_BSpline) bsplineLawHandle = new Law_BSpline(P, U_values, U_mults, p);



    // -------------------------------------------------------------------------------------------------------------- //
    // Modify one control point of the B-Spline law
    // -------------------------------------------------------------------------------------------------------------- //

    // Print the original set of control points
    cout << "\n\n";
    cout << "Original set of control points" << endl;
    cout.precision(8);
    cout.setf(ios::fixed);
    for (int i = 1; i <= bsplineLaw.NbPoles(); ++i){
        cout << setw(20) << i << setw(20) << bsplineLaw.Pole(i) << endl;
    }

    // Modify one control point of the B-Spline law
    Standard_Real newPole = 1.00;
    bsplineLaw.SetPole(2, newPole);

    // Print the new set of control points
    cout << "\n\n";
    cout << "New set of control points" << endl;
    for (int i = 1; i <= bsplineLaw.NbPoles(); ++i){
        cout << setw(20) << i << setw(20) << bsplineLaw.Pole(i) << endl;
    }



    // -------------------------------------------------------------------------------------------------------------- //
    // Print the knot vector and the vector of knot multiplicities
    // -------------------------------------------------------------------------------------------------------------- //

    // Get the knot vector values
    TColStd_Array1OfReal knot_vector(bsplineLaw.FirstUKnotIndex(), bsplineLaw.LastUKnotIndex());
    bsplineLaw.Knots(knot_vector);

    // Get the knot multiplicities
    TColStd_Array1OfInteger knot_mults(bsplineLaw.FirstUKnotIndex(), bsplineLaw.LastUKnotIndex());
    bsplineLaw.Multiplicities(knot_mults);

    // Print the knot vector values and multiplicities
    cout << "\n\nKnot vector" << endl;
    cout << setw(10) << "Values" << setw(10) << "Mults" << endl;
    for (int i = knot_vector.Lower(); i <= knot_vector.Upper(); ++i) {
        cout << setw(10) << knot_vector(i) << setw(10) << knot_mults(i) << endl;
    }



    // -------------------------------------------------------------------------------------------------------------- //
    // Evaluate the B-Spline law
    // -------------------------------------------------------------------------------------------------------------- //

    int Nu = 101;
    TColStd_Array1OfReal u(0, Nu-1);

    double a = 0.0, b = 1.00, step = (b-a)/(Nu-1);
    for (int i = 0; i < Nu; ++i) {
        u(i) = a + step*i;
    }

    TColStd_Array1OfReal bsplineValues(0, Nu-1);
    for (int i = 0; i < Nu; ++i) {
        bsplineValues(i) = bsplineLaw.Value(u(i));
    }

    cout << "\n\nEvaluate the B-Spline law" << endl;
    cout << setw(15) << "u-parameter" << setw(15) << "BSpline value" << endl;
    for (int i = 0; i < Nu; ++i) {
        cout << setw(15) << u(i)  << setw(15) << bsplineValues(i) << endl;
    }



    // -------------------------------------------------------------------------------------------------------------- //
    // Print the coordinates of the B-Spline law
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a file output object
    ofstream bsplineFile;
    string relative_path = "../output/";
    string file_name = "bspline_law.csv";
    mkdir(relative_path.c_str(), 0777);     // 0007 is used to give the user permissions to read+write+execute
    string full_path = relative_path + file_name;
    bsplineFile.open (full_path);
    bsplineFile.precision(8);
    bsplineFile.setf(ios::fixed);

    // Print the coordinates of the B-Spline law
    for (int i = 0; i < Nu; ++i) {
        bsplineFile << u(i) << ", " << bsplineValues(i) << endl;
    }

    // Close the output file
    bsplineFile.close();


    return 0;


}