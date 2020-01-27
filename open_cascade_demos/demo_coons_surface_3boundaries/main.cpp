// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a Coons surface in OpenCascade
//  Author: Roberto Agromayor
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <sys/stat.h>
#include <cmath>


// Include OpenCascade libraries
#include <gp_Pnt.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BezierSurface.hxx>
#include <GeomFill_BezierCurves.hxx>
#include <TopoDS.hxx>
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
    // Define the boundaries of the domain
    // -------------------------------------------------------------------------------------------------------------- //

    // The domain is defined by 3 contiguous Bezier curves
    // Each Bezier curve is constructed from an array of control points and it is referenced by handle

    // Mathematical constants
    const double pi = M_PI;

    // Boundary 1
    TColgp_Array1OfPnt P_1(1, 3);
    P_1(1) = gp_Pnt(0.00, 0.0, 0.0);
    P_1(2) = gp_Pnt(0.50, 0.0, 0.5);
    P_1(3) = gp_Pnt(1.00, 0.0, 0.0);
    Handle(Geom_BezierCurve) bezier_1 = new Geom_BezierCurve(P_1);

    // Boundary 2
    TColgp_Array1OfPnt P_2(1, 3);
    P_2(1) = bezier_1->Pole(bezier_1->NbPoles());                    // Conforming corner | same as P_1(3)
    P_2(2) = gp_Pnt(1.00-0.50*cos(pi/3), 0.50*sin(pi/3), 0.5);
    P_2(3) = gp_Pnt(1.00-1.00*cos(pi/3), 1.00*sin(pi/3), 0.0);
    Handle(Geom_BezierCurve) bezier_2 = new Geom_BezierCurve(P_2);

    // Boundary 3
    TColgp_Array1OfPnt P_3(1, 3);
    P_3(1) = bezier_2->Pole(bezier_2->NbPoles());                    // Conforming corner | same as P_2(3)
    P_3(2) = gp_Pnt(0.50*cos(pi/3), 0.5*sin(pi/3), 0.50);
    P_3(3) = bezier_1->Pole(1);                                      // Conforming corner | same as P_1(1)
    Handle(Geom_BezierCurve) bezier_3 = new Geom_BezierCurve(P_3);



    // -------------------------------------------------------------------------------------------------------------- //
    // Create a Coons patch defined by its boundaries
    // -------------------------------------------------------------------------------------------------------------- //

    // Create the Bezier surface from the boundaries and a filling style
    // Styles available: 1) GeomFill_CoonsStyle 2) GeomFill_StretchStyle 3) GeomFill_CurvedStyle
    GeomFill_BezierCurves makeBezierSurfGeo(bezier_1, bezier_2, bezier_3, GeomFill_CoonsStyle);
    Handle(Geom_BezierSurface) BezierSurfGeo = makeBezierSurfGeo.Surface();

    // Check the bounds of the patch in parametric space [Optional]
    double u_lower, u_upper, v_lower, v_upper;
    BezierSurfGeo->Bounds(u_lower, u_upper, v_lower, v_upper);

    // Define the topology of the Bezier surface using the BRepBuilderAPI
    TopoDS_Face BezierSurfTopo = BRepBuilderAPI_MakeFace(BezierSurfGeo, 0.);


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a TopoDS_Shape object to export as .step
    TopoDS_Shape open_cascade_model = BezierSurfTopo;

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "coons_surface";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;


}
