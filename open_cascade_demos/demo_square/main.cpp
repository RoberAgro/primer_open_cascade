// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a square in OpenCascade
//  Author: Roberto Agromayor
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <sys/stat.h>
#include <cmath>


// Include OpenCascade libraries
#include <gp_Pnt.hxx>
#include <GC_MakeSegment.hxx>
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
    // Define the geometry and topology of square in a more robust way that extends to more complex cases
    // -------------------------------------------------------------------------------------------------------------- //

    // Define points that define the geometry of the square
    gp_Pnt Point1(0., 0., 0.);   // Bottom left
    gp_Pnt Point2(1., 0., 0.);   // Bottom right
    gp_Pnt Point3(1., 1., 0.);   // Top right
    gp_Pnt Point4(0., 1., 0.);   // Bottom left

    // Make lines connecting the points defining the square
    Handle(Geom_TrimmedCurve) Line12 = GC_MakeSegment(Point1, Point2);
    Handle(Geom_TrimmedCurve) Line23 = GC_MakeSegment(Point2, Point3);
    Handle(Geom_TrimmedCurve) Line34 = GC_MakeSegment(Point3, Point4);
    Handle(Geom_TrimmedCurve) Line41 = GC_MakeSegment(Point4, Point1);

    // Create the edges using Handle(Geom_TrimmedCurve) objects and BRepBuilderAPI_MakeEdge()
    TopoDS_Edge Edge1 = BRepBuilderAPI_MakeEdge(Line12);
    TopoDS_Edge Edge2 = BRepBuilderAPI_MakeEdge(Line23);
    TopoDS_Edge Edge3 = BRepBuilderAPI_MakeEdge(Line34);
    TopoDS_Edge Edge4 = BRepBuilderAPI_MakeEdge(Line41);

    // Create a closed wire adding the edges to a BRepBuilderAPI_MakeWire object
    BRepBuilderAPI_MakeWire MakeWire;
    MakeWire.Add(Edge1);
    MakeWire.Add(Edge2);
    MakeWire.Add(Edge3);
    MakeWire.Add(Edge4);
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
    string file_name = "square";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;


}
