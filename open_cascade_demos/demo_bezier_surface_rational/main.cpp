// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a rational Beziert surface in OpenCascade
//  Author: Roberto Agromayor
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <sys/stat.h>
#include <cmath>


// Include OpenCascade libraries
#include <gp.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>

#include <Geom_BezierSurface.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>

#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <STEPControl_Writer.hxx>


// Setting namespaces
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
    Standard_Integer rowLower = 1, rowUpper = 3;
    Standard_Integer colLower = 1, colUpper = 2;
    TColgp_Array2OfPnt P(rowLower, rowUpper, colLower, colUpper);

    // First row
    P(1, 1) = gp_Pnt(1.0, 0.0, 0.0);
    P(2, 1) = gp_Pnt(1.0, 1.0, 0.0);
    P(3, 1) = gp_Pnt(0.0, 1.0, 0.0);

    // Second row
    P(1, 2) = gp_Pnt(1.0, 0.0, 2.0);
    P(2, 2) = gp_Pnt(1.0, 1.0, 2.0);
    P(3, 2) = gp_Pnt(0.0, 1.0, 2.0);


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the array of control point weights
    // -------------------------------------------------------------------------------------------------------------- //

    // Declare array
    TColStd_Array2OfReal W(rowLower, rowUpper, colLower, colUpper);

    // First row
    W(1, 1) = 1.;
    W(2, 1) = sqrt(2.)/2.;      // This choice of weight leads to a circular sector of 90 degrees
    W(3, 1) = 1.;

    // Second row
    W(1, 2) = 1.;
    W(2, 2) = sqrt(2.)/2.;      // This choice of weight leads to a circular sector of 90 degrees
    W(3, 2) = 1.;


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the geometry and topology of a rational Bezier surface patch (90 degrees)
    // -------------------------------------------------------------------------------------------------------------- //

    // Create the  geometry and reference it by handle
    Handle(Geom_BezierSurface) BezierGeo = new Geom_BezierSurface(P, W);

    // Define the topology of the Bezier surface using the BRepBuilderAPI
    TopoDS_Face BezierFace = BRepBuilderAPI_MakeFace(BezierGeo, 0.);


    // -------------------------------------------------------------------------------------------------------------- //
    // Define the geometry and topology of a rational Bezier surface patch
    // -------------------------------------------------------------------------------------------------------------- //

    // Get the value of pi
    const double pi = M_PI;

    // Rotate the Bezier patch 90 degrees around the Z-axis
    gp_Trsf myTransformation;                                           // Declare a transformation object
    gp_Ax1 axisOfRotation = gp::OZ();                                   // Set Z as the axis of rotation
    Standard_Real angleOfRotation = pi/2;                               // Set the angle of rotation
    myTransformation.SetRotation(axisOfRotation, angleOfRotation);      // Set the transformation as a rotation
    BRepBuilderAPI_Transform BRepT1(BezierFace, myTransformation);      // Apply the rotation to the Bezier patch
    TopoDS_Shape temp = BRepT1.Shape();                                 // Retrieve the rotated bezier patch
    TopoDS_Face BezierRotatedFace = TopoDS::Face(temp);                 // Cast the TopoDS_Shape into a TopoDS_Face

    // Make a compound with the two bezier patches
    TopoDS_Compound myCompound;                                         // Declare the TopoDS_Compound object
    BRep_Builder myBuilder;                                             // Declare the compound builder object
    myBuilder.MakeCompound (myCompound);                                // Start the compound creation
    myBuilder.Add (myCompound, BezierFace);                             // Add the original Bezier patch
    myBuilder.Add (myCompound, BezierRotatedFace);                      // Add the rotated Bezier patch

    // Mirror the two Bezier patches in the XZ plane
    gp_Ax2 planeOfReflexion = gp::ZOX();                                // Set the XY plane as the plane of symmetry
    myTransformation.SetMirror(planeOfReflexion);                       // Set the transformation as a reflection
    BRepBuilderAPI_Transform BRepT2(myCompound, myTransformation);      // Apply the mirroring to the compound
    temp = BRepT2.Shape();                                              // Retrieve the rotated bezier patch
    TopoDS_Compound CylinderMirrored = TopoDS::Compound(temp);          // Cast the TopoDS_Shape into a TopoDS_Compound
    myBuilder.Add (myCompound, CylinderMirrored);                       // Add the mirrored patches to the compound


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a TopoDS_Shape object to export as .step
    TopoDS_Shape open_cascade_model = myCompound;

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "bezier_rational_surface";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());



}
