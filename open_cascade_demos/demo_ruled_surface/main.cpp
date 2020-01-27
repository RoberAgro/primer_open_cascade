// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a ruled surface in OpenCascade
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
#include <Geom_BSplineSurface.hxx>
#include <GeomFill.hxx>
#include <GeomFill_BezierCurves.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Builder.hxx>
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


    /*
     * This demonstration script shows how to create a ruled surface between 2 curves
     * The ruled surface is generated using GeomFill()
     * The output from GeomFill() seems to be the same as the output from GeomFill_BezierCurves/GeomFill_BSplineCurves
     *
     * Author: Roberto Agromayor
     * Data: December 2019
     *
     * */


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the boundaries of the domain
    // -------------------------------------------------------------------------------------------------------------- //

    // The domain is defined by 2 contiguous Bezier curves
    // Each Bezier curve is constructed from an array of control points and it is referenced by handle

    // Boundary 1
    TColgp_Array1OfPnt P_upper(1, 4);
    P_upper(1) = gp_Pnt(0.00, 0.0, 0.0);
    P_upper(2) = gp_Pnt(0.33, 1.0, 0.5);
    P_upper(3) = gp_Pnt(0.66, 1.0, -0.5);
    P_upper(4) = gp_Pnt(1.00, 0.0, 0.0);
    Handle(Geom_BezierCurve) bezier_upper = new Geom_BezierCurve(P_upper);

    // Boundary 2
    TColgp_Array1OfPnt P_lower(1, 4);
    P_lower(1) = bezier_upper->Pole(1);                                     // Conforming corner | same as P_upper(1)
    P_lower(2) = gp_Pnt(0.33, -1.00, -0.50);
    P_lower(3) = gp_Pnt(0.66, -1.00, 0.50);
    P_lower(4) = bezier_upper->Pole(bezier_upper->NbPoles());               // Conforming corner | same as P_upper(3)
    Handle(Geom_BezierCurve) bezier_lower = new Geom_BezierCurve(P_lower);


    // -------------------------------------------------------------------------------------------------------------- //
    // Create a Coons patch defined by its boundaries
    // -------------------------------------------------------------------------------------------------------------- //

    // Create the ruled surface using the GeomFill package
    Handle(Geom_Surface) RuledSurfaceGeo = GeomFill().Surface(bezier_upper, bezier_lower);

    // Cast the Geom_Surface to Geom_BSpline to access its methods if desired [Optional]
    Handle(Geom_BSplineSurface) temp = Handle(Geom_BSplineSurface)::DownCast(RuledSurfaceGeo);

    // Define the topology of the ruled surface using the BRepBuilderAPI
    TopoDS_Face RuledSurfaceFace = BRepBuilderAPI_MakeFace(RuledSurfaceGeo, 0.);


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a TopoDS_Shape object to export as .step
    TopoDS_Shape open_cascade_model = RuledSurfaceFace;

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "ruled_surface";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;


}
