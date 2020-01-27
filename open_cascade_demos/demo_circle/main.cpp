// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a circle in OpenCascade
//  Author: Roberto Agromayor
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <sys/stat.h>
#include <cmath>


// Include OpenCascade libraries
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax2.hxx>
#include <Geom_Circle.hxx>
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

     /*
      * This demonstration script shows how to create the geometry and topology of a circle and how to export as STEP
      * The geometry of the circle is built using a Geom_Circle object
      * The topology is built using the BRepBuilderAPI package
      *
      * Author: Roberto Agromayor
      * Data: December 2019
      *
      * */


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the geometry and topology of a circle
    // -------------------------------------------------------------------------------------------------------------- //

    // Initialize coordinate values
    const Standard_Real x1 = 0., y1 = 0., z1 = 0.;
    const Standard_Real x2 = 0., y2 = 0., z2 = 1.;

    // Mathematical constants
    const double pi = M_PI;
    const double e = M_E;

    // Initialize the radius and the axis that define the circle
    // The radius is declared as a Standard_Real object
    // The axis is declared as a gp_Ax2 object with "X Direction" and "Y Direction" defining the plane of the circle
    Standard_Real my_radius = pi;
    gp_Pnt P(x1, y1, z1);
    gp_Dir V(x2, y2, z2);
    gp_Ax2 my_Ax2(P, V);

    // Initialize the GeomCircle object referenced by handle using the Geom package.
    Handle(Geom_Circle) myCircle = new Geom_Circle(my_Ax2, my_radius);

    // Create the topology of the circle
    TopoDS_Edge circle_edge = BRepBuilderAPI_MakeEdge(myCircle);                        // Make edge from curve
    TopoDS_Wire circle_wire = BRepBuilderAPI_MakeWire(circle_edge);                     // Make wire from edge
    TopoDS_Face circle_face = BRepBuilderAPI_MakeFace(circle_wire, Standard_True);      // Make face from plane wire


    // -------------------------------------------------------------------------------------------------------------- //
    // Make a copy of the circle and change its radius [Optional to learn more about the arrow operator and casting!]
    // -------------------------------------------------------------------------------------------------------------- //
    Handle(Geom_Geometry) CircleGeometry = myCircle->Copy();                                                            // Use the arrow operator to access the members of an object referenced by handle
    Handle(Geom_Curve) CircleCurve = Handle(Geom_Curve)::DownCast (CircleGeometry);                                     // Downcast from Geom_Geometry to Geom_Curve
    Handle(Geom_Circle) CircleCircle = Handle(Geom_Circle)::DownCast (CircleGeometry);                                  // Downcast from Geom_Geometry to Geom_Circle
    CircleCircle->SetRadius(e);
    cout << "The original circle radius is: " << myCircle->Radius() << endl;
    cout << "The modified circle radius is: " << CircleCircle->Radius() << endl;


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a TopoDS_Shape object to export as .step
    TopoDS_Shape open_cascade_model = circle_face;

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "circle";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());


    return 0;


}
