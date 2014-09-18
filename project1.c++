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
	    { {-9,-1 }, {-9,-9 }, {-1,-9 }, {-1,-1 } },
	};

	GLFWController c(argv[0]);
	c.reportVersions(std::cout);

	c.addModel( new ModelView(vertexPositions[0]) );
	c.addModel( new ModelView(vertexPositions[1]) );
	c.addModel( new ModelView(vertexPositions[2]) );
	c.addModel( new ModelView(vertexPositions[3]) );

	// initialize 2D viewing information:
	// Get the overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	return 0;
}
