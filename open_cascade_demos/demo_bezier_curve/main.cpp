// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a Bezier curve in OpenCascade
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
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
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

    // Note: it seems that OpenCascade prefers to start indexing control point arrays from one instead of zero :(


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the geometry and topology of the Bezier curve
    // -------------------------------------------------------------------------------------------------------------- //

    // Create the  geometry and reference it by handle
    Handle(Geom_BezierCurve) BezierGeo = new Geom_BezierCurve(P);
    Handle_Geom_BezierCurve BezierGeo_bis = new Geom_BezierCurve(P);  // Equivalent alternative

    // Define the topology of the Bezier curve using the BRepBuilderAPI
    TopoDS_Edge BezieEdge = BRepBuilderAPI_MakeEdge(BezierGeo);


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a TopoDS_Shape object to export as .step
    TopoDS_Shape open_cascade_model = BezieEdge;

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "bezier_curve";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;


}
