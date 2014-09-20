// project1.c++

#include "GLFWController.h"
#include "ModelView.h"

int main(int argc, char* argv[])
{
    // define vertex coordinates for each box
	vec2 vertexPositions[][4] =
	{/*
	    { {-0.9,-0.9 }, {-0.1,-0.9 }, {-0.9,-0.1 }, {-0.1,-0.1 } },
	    { { 0.9, 0.9 }, { 0.1, 0.9 }, { 0.9, 0.1 }, { 0.1, 0.1 } },
	    { {-0.5, 0.1 }, {-0.1, 0.1 }, {-0.5, 0.5 }, {-0.1, 0.5 } },
	    { {-2.5, 0.5 }, {-0.5, 0.5 }, {-2.5, 2.5 }, {-0.5, 2.5 } },
        { { 0.5, 1.0 }, { 1.5, 2.0 }, {-0.5, 2.0 }, { 0.5, 3.0 } } 
	*/
        { {-12, 5 }, { -1, 5 }, {-12, 16}, { -1, 16} },
        { { -7, 16}, { -5, 18}, { -9, 18}, { -7, 20} },
        { {  6, 3 }, { 10, 3 }, {  6, 7 }, { 10, 7 } },
        { {  8, 7 }, { 13, 12}, {  3, 12}, {  8, 17} },
        { {  2,-11}, { 10,-11}, {  2, -3}, { 10, -3} },
        { {-12,-14}, { -6,-14}, {-12, -8}, { -6, -8} }
    };

    vec4 colors[][5] =
    {
        { { 0.2, 1.0, 0.5, 1.0 }, { 0.7, 0.2, 0.5, 1.0 }, { 1.0, 1.0, 0.2, 1.0 },
            { 0.2, 0.2, 0.2, 1.0 }, { 1.0, 1.0, 0.5, 1.0 } },
        { { 0.2, 0.5, 0.5, 1.0 }, { 0.2, 0.9, 0.2, 1.0 }, { 1.0, 0.2, 0.2, 1.0 },
            { 0.2, 0.0, 0.2, 1.0 }, { 0.6, 1.0, 0.5, 1.0 } },
        { { 0.1, 1.0, 0.7, 1.0 }, { 0.7, 0.2, 0.5, 1.0 }, { 0.9, 1.0, 0.6, 1.0 },
            { 0.1, 0.8, 0.6, 1.0 }, { 1.0, 1.0, 0.7, 1.0 } },
        { { 0.2, 0.0, 0.5, 1.0 }, { 0.0, 0.2, 1.0, 1.0 }, { 1.0, 0.3, 0.8, 1.0 },
            { 0.2, 0.0, 1.0, 1.0 }, { 1.0, 0.3, 0.2, 1.0 } },
        { { 0.5, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 1.0, 0.1, 0.3, 1.0 },
            { 0.7, 0.0, 0.5, 1.0 }, { 0.2, 0.3, 0.1, 1.0 } },
        { { 0.9, 0.0, 0.0, 1.0 }, { 0.8, 1.0, 1.0, 1.0 }, { 1.0, 0.8, 0.1, 1.0 },
            { 0.7, 0.9, 0.2, 1.0 }, { 0.9, 0.5, 1.0, 1.0 } }
    };

	GLFWController c(argv[0]);
	c.reportVersions(std::cout);

    for(int i = 0; i < 6; i++)
    {
        c.addModel( new ModelView(vertexPositions[i], colors[i] ) );
    }
    /*
	c.addModel( new ModelView(vertexPositions[0], colors[0] ) );
	c.addModel( new ModelView(vertexPositions[1], colors[1] ) );
	c.addModel( new ModelView(vertexPositions[2], colors[2] ) );
	c.addModel( new ModelView(vertexPositions[3], colors[3] ) );
    c.addModel( new ModelView(vertexPositions[4], colors[4] ) );
*/
	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	return 0;
}
