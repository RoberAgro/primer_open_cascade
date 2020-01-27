// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a B-Spline curve in OpenCascade
//  Author: Roberto Agromayor
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <sys/stat.h>
#include <cmath>

// Include OpenCascade libraries
#include <gp_Pnt.hxx>
#include <Geom_BSplineCurve.hxx>
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

    TColgp_Array1OfPnt P(1, 7);
    P(1) = gp_Pnt(0.00, 0.0, 0.0);
    P(2) = gp_Pnt(0.25, -0.5, 0.0);
    P(3) = gp_Pnt(0.50, 0.0, 0.0);
    P(4) = gp_Pnt(0.75, 0.0, 0.0);
    P(5) = gp_Pnt(1.00, 0.0, 0.0);
    P(6) = gp_Pnt(0.50, 0.5, 0.0);
    P(7) = gp_Pnt(0.00, 0.5, 0.0);


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
    // Define the geometry and topology of a B-Spline curve + closing segment
    // -------------------------------------------------------------------------------------------------------------- //

    // Create the  geometry and reference it by handle
    Handle(Geom_BSplineCurve) BSplineGeo = new Geom_BSplineCurve(P, U_values, U_mults, p);

    // Define the topology of the B-Spline curve using the BRepBuilderAPI
    TopoDS_Edge BSplineEdge = BRepBuilderAPI_MakeEdge(BSplineGeo);

    // Make and edge combining the endpoints of the B-Spline curve
    Standard_Integer n_control_points = BSplineGeo->NbPoles();
    gp_Pnt P_start = BSplineGeo->Pole(1);
    gp_Pnt P_end   = BSplineGeo->Pole(n_control_points);
    TopoDS_Edge ClosingEdge = BRepBuilderAPI_MakeEdge(P_start, P_end);

    // Create a closed wire adding the edges to a BRepBuilderAPI_MakeWire object
    BRepBuilderAPI_MakeWire MakeWire;
    MakeWire.Add(BSplineEdge);
    MakeWire.Add(ClosingEdge);
    TopoDS_Wire Wire = MakeWire.Wire();

    // Make a face from the plane wire
    TopoDS_Face Face = BRepBuilderAPI_MakeFace(Wire);


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a TopoDS_Shape object to export as .step
    TopoDS_Shape open_cascade_model = Face;

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "bspline_curve";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;


}
