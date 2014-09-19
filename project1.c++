// project1.c++

#include "GLFWController.h"
#include "ModelView.h"

int main(int argc, char* argv[])
{
    // define vertex coordinates for each box
	vec2 vertexPositions[][4] =
	{
	    { {-0.9,-0.1 }, {-0.9,-0.9 }, {-0.1,-0.9 }, {-0.1,-0.1 } },
	    { { 0.9, 0.1 }, { 0.9, 0.9 }, { 0.1, 0.9 }, { 0.1, 0.1 } },
	    { {-0.5, 0.5 }, {-0.5, 0.1 }, {-0.1, 0.1 }, {-0.1, 0.5 } },
	    { {-2.5, 2.5 }, {-2.5, 0.5 }, {-0.5, 0.5 }, {-0.5, 2.5 } },
	};

    vec4 colors[] =
    {
        { 0.2, 1.0, 0.5, 1.0 },
        { 0.7, 0.2, 0.5, 1.0 },
        { 1.0, 1.0, 0.2, 1.0 },
        { 0.2, 0.2, 0.2, 1.0 }
    };

	GLFWController c(argv[0]);
	c.reportVersions(std::cout);

	c.addModel( new ModelView(vertexPositions[0], 10, colors[0] ) );
	c.addModel( new ModelView(vertexPositions[1], 20, colors[1] ) );
	c.addModel( new ModelView(vertexPositions[2],  6, colors[2] ) );
	c.addModel( new ModelView(vertexPositions[3], 15, colors[3] ) );

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	return 0;
}
