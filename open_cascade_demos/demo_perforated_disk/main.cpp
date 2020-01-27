// ------------------------------------------------------------------------------------------------------------------- //
//
//  Demonstration script showing how to create a perforated disk in OpenCascade
//  Author: Roberto Agromayor
//  This project was adapted from: https://neweopencascade.wordpress.com/2014/03/01/post1/
//
// ------------------------------------------------------------------------------------------------------------------ //


// Include standard C++ libraries
#include <iostream>
#include <sys/stat.h>
#include <cmath>


// Include OpenCascade libraries
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <gp_Circ.hxx>
#include <Geom_BezierCurve.hxx>
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


    // -------------------------------------------------------------------------------------------------------------- //
    // Create the model
    // -------------------------------------------------------------------------------------------------------------- //

    //Init brep builder utility
    BRep_Builder aBuilder;

    //Creation of an infinite face lying on a plane
    gp_Pln planeXY;
    TopoDS_Face aFace = BRepBuilderAPI_MakeFace(planeXY);

    //Crating tow wires to bound the face
    gp_Ax2 Ax2(gp_Pnt(),gp_Dir(0,0,1),gp_Dir(1,0,0));
    TopoDS_Wire wireIn = BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(gp_Circ(Ax2,1)));
    TopoDS_Wire wireOut = BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(gp_Circ(Ax2,2)));

    //Add outer bound to the face
    aBuilder.Add(aFace,wireOut);

    //Add inner bound. Must be reversed
    aBuilder.Add(aFace,wireIn.Reversed());

    //Add more inner boundaries
    int nCuts = 30;
    for(int i = 1 ; i < nCuts ; i++){
        gp_Ax2 Ax(gp_Pnt(1.5,0,0),gp_Dir(0,0,1),gp_Dir(1,0,0));
        TopoDS_Wire wire = BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(gp_Circ(Ax,0.1)));
        gp_Trsf rot;
        rot.SetRotation(gp_Ax1(gp_Pnt(),gp_Dir(0,0,1)),2.*M_PI*i/(nCuts-1.));
        wire.Move(rot);
        aBuilder.Add(aFace,wire.Reversed());
    }


    // -------------------------------------------------------------------------------------------------------------- //
    // Export the model as a STEP file
    // -------------------------------------------------------------------------------------------------------------- //

    // Create a TopoDS_Shape object to export as .step
    TopoDS_Shape open_cascade_model = aFace;

    // Set the destination path and the name of the .step file
    string relative_path = "../output/";
    string file_name = "perforated_disk";

    // Write the .step file
    write_step_file(relative_path, file_name, open_cascade_model);


    // -------------------------------------------------------------------------------------------------------------- //
    // Visualize the geometry in a graphical user interface (for instance the FreeCAD GUI)
    // -------------------------------------------------------------------------------------------------------------- //
    string open_gui = "FreeCAD --single-instance " + relative_path + file_name + ".step";
    system(open_gui.c_str());



}