// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a NURBS surface in OpenCascade
//  Author: Roberto Agromayor
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <sys/stat.h>
#include <cmath>


// Include OpenCascade libraries
#include <gp_Pnt.hxx>
#include <Geom_BSplineSurface.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <STEPControl_Writer.hxx>


// Define namespaces
using namespace std;


// ------------------------------------------------------------------------------------------------------------------ //
// Step file exporter
// ------------------------------------------------------------------------------------------------------------------ //
IFSelect_ReturnStatus
write_step_file(const string &relative_path, const string &model_name, const TopoDS_Shape &model_object) {

    // Create the .step writer object
    STEPControl_Writer step_writer;

    // Set the type of .step representation
    STEPControl_StepModelType step_mode = STEPControl_StepModelType::STEPControl_AsIs;

    // Create the output directory if it does not exist
    mkdir(relative_path.c_str(), 0777);     // 0007 is used to give the user permissions to read+write+execute

    // Get the full path to the step file as a C-string
    string temp = (relative_path + model_name + ".step");
    Standard_CString file_name = temp.c_str();

    // Write the .step file
    step_writer.Transfer(model_object, step_mode);
    IFSelect_ReturnStatus status = step_writer.Write(file_name);
    return status;

}


// ------------------------------------------------------------------------------------------------------------------ //
// Main body
// ------------------------------------------------------------------------------------------------------------------ //
int main() {


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the array of control points
    // -------------------------------------------------------------------------------------------------------------- //

    // Declare the array
    Standard_Integer rowLower = 1, rowUpper = 5;
    Standard_Integer colLower = 1, colUpper = 3;
    TColgp_Array2OfPnt P(rowLower, rowUpper, colLower, colUpper);

    // First row
    P(1, 1) = gp_Pnt(0.00, 0.0, 0.0);
    P(2, 1) = gp_Pnt(0.25, 0.0, 0.0);
    P(3, 1) = gp_Pnt(0.50, 0.0, 0.0);
    P(4, 1) = gp_Pnt(0.75, 0.0, 0.0);
    P(5, 1) = gp_Pnt(1.00, 0.0, 0.0);

    // Second row
    P(1, 2) = gp_Pnt(0.00, 0.5, 0.0);
    P(2, 2) = gp_Pnt(0.25, 0.5, 1.0);
    P(3, 2) = gp_Pnt(0.50, 0.5, 1.0);
    P(4, 2) = gp_Pnt(0.75, 0.5, 1.0);
    P(5, 2) = gp_Pnt(1.00, 0.5, 0.0);

    // Third row
    P(1, 3) = gp_Pnt(0.00, 1.0, 0.0);
    P(2, 3) = gp_Pnt(0.25, 1.0, 0.0);
    P(3, 3) = gp_Pnt(0.50, 1.0, 0.0);
    P(4, 3) = gp_Pnt(0.75, 1.0, 0.0);
    P(5, 3) = gp_Pnt(1.00, 1.0, 0.0);


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the array of control point weights
    // -------------------------------------------------------------------------------------------------------------- //

    // Declare array
    TColStd_Array2OfReal W(rowLower, rowUpper, colLower, colUpper);

    // First row
    W(1, 1) = 1.;
    W(2, 1) = 1.;
    W(3, 1) = 1.;
    W(4, 1) = 1.;
    W(5, 1) = 1.;

    // Second row
    W(1, 2) = 1.;
    W(2, 2) = 2.;   // Add extra weight to get a funny shape
    W(3, 2) = 1.;
    W(4, 2) = 2.;   // Add extra weight to get a funny shape
    W(5, 2) = 1.;

    // Third row
    W(1, 3) = 1.;
    W(2, 3) = 1.;
    W(3, 3) = 1.;
    W(4, 3) = 1.;
    W(5, 3) = 1.;


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the U-knot vector
    // -------------------------------------------------------------------------------------------------------------- //

    // Maximum index of the control points (counting from zero)
    Standard_Integer n = P.UpperRow() - P.LowerRow();

    // Define the order of the basis polynomials
    // Linear (p = 1), Quadratic (p = 2), Cubic (p = 3), etc.
    // Set p = n (number of control points minus one) to obtain a Bezier base
    Standard_Integer p = 2;

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
    // Define the V-knot vector
    // -------------------------------------------------------------------------------------------------------------- //

    // Maximum index of the control points (counting from zero)
    Standard_Integer m = P.UpperCol() - P.LowerCol();

    // Define the order of the basis polynomials
    // Linear (q = 1), Quadratic (q = 2), Cubic (q = 3), etc.
    // Set q = m (number of control points minus one) to obtain a Bezier base
    Standard_Integer q = 2;

    // Define of the knot vector (clamped spline)
    // q+1 zeros, m-q equispaced points between 0 and 1, and q+1 ones. M+1=m-q+2 distinct values
    Standard_Integer M = m - q + 2 - 1;
    TColStd_Array1OfReal V_values(0, M);
    TColStd_Array1OfInteger V_mults(0, M);

    for (int j = 0; j <= M; ++j) {

        // Set the knot values
        V_values(j) = double(j) / double(M);

        // Set the knot multiplicities
        if (j == 0 || j == M) { V_mults(j) = q+1; }
        else { V_mults(j) = 1; }

    }


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the geometry and topology of a NURBS surface patch
    // -------------------------------------------------------------------------------------------------------------- //

    // Define the geometry of a NURBS surface referenced by handle
    // Note that skipping the weights argument (W) reduces the NURBS surface to a B-Spline surface with unitary weights
    Handle(Geom_BSplineSurface) BSplineGeo = new Geom_BSplineSurface(P, W, U_values, V_values, U_mults, V_mults, p, q, Standard_False, Standard_False);

    // Define the topology of the NURBS surface
    TopoDS_Face BSplineFace = BRepBuilderAPI_MakeFace(BSplineGeo, 0);


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a TopoDS_Shape object to export as .step
    TopoDS_Shape open_cascade_model = BSplineFace;

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "nurbs_surface";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;




}
