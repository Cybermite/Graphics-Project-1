// ModelView.c++ - a basic combined Model and View for OpenGL

#include <iostream>

#include "ModelView.h"
#include "Controller.h"
#include "ShaderIF.h"

ShaderIF* ModelView::shaderIF = NULL;
int ModelView::numInstances = 0;
GLuint ModelView::shaderProgram = 0;
GLint ModelView::ppuLoc_colorMode = -2; // uniform variable (per-primitive)
GLint ModelView::ppuLoc_scaleTrans = -2;
GLint ModelView::pvaLoc_mcPosition = -2; // attribute variable (per-vertex)
//GLint ModelView::ppuLoc_mvBounds = -2;
GLint ModelView::pvaLoc_mvMinBounds = -2;
GLint ModelView::pvaLoc_mvMaxBounds = -2;
GLint ModelView::pvaLoc_mvColor = -2;
GLint ModelView::ppuLoc_mvNumOfCircles = -2;

double ModelView::mcRegionOfInterest[6] = { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 };

static const int numVertices = 4;

ModelView::ModelView(vec2* vertices, int numberOfCircles, vec4 color) 
    : colorMode(8), visible(true), numberOfCircles(numberOfCircles)
{
	if (ModelView::shaderProgram == 0)
	{
		// Create the common shader program:
		ModelView::shaderIF = new ShaderIF("project1.vsh", "project1.fsh");
		ModelView::shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}

    setColor(color);
    defineGeometry(vertices);
	ModelView::numInstances++;
}

ModelView::~ModelView()
{
    deleteObject();
	if (--numInstances == 0)
	{
		ModelView::shaderIF->destroy();
		delete ModelView::shaderIF;
		ModelView::shaderIF = NULL;
		ModelView::shaderProgram = 0;
	}
}

void ModelView::deleteObject()
{
	if (vao[0] > 0) // if it hasn't been deleted
	{
		glDeleteBuffers(1, vbo);
		glDeleteVertexArrays(1, vao);
		vao[0] = vbo[0] = 0;
	}
}

void ModelView::setColor(vec4 color)
{
    for(int i = 0; i < 4; i++)
    {
        mvColor[i] = color[i];
    }
}

// computeScaleTrans determines the current model coordinate region of
// interest and then uses linearMap to determine how to map coordinates
// in the region of interest to their proper location in Logical Device
// Space. (Returns float[] because glUniform currently favors float[].)
void ModelView::computeScaleTrans(float* scaleTransF) // CLASS METHOD
{

	double xmin = mcRegionOfInterest[0];
	double xmax = mcRegionOfInterest[1];
	double ymin = mcRegionOfInterest[2];
	double ymax = mcRegionOfInterest[3];

	// BEGIN: ASPECT RATIO CONSIDERATIONS
	// If we wish to preserve aspect ratios, make "region of interest"
	// wider or taller to match the Controller's viewport aspect ratio.
	double vAR = Controller::getCurrentController()->getViewportAspectRatio();

	double wHeight = ymax - ymin;
	double wWidth = xmax - xmin;
	double wAR = wHeight / wWidth; // wAR == MC Window Aspect Ratio
	if (wAR > vAR)
	{
		// make MC window wider
		wWidth = wHeight / vAR;
		double xmid = 0.5 * (xmin + xmax);
		xmin = xmid - 0.5*wWidth;
		xmax = xmid + 0.5*wWidth;
	}
	else
	{
		// make MC window taller
		wHeight = wWidth * vAR;
		double ymid = 0.5 * (ymin + ymax);
		ymin = ymid - 0.5*wHeight;
		ymax = ymid + 0.5*wHeight;
	}
	// END: ASPECT RATIO CONSIDERATIONS

    // We are only concerned with the xy extents for now, hence we will
    // ignore mcRegionOfInterest[4] and mcRegionOfInterest[5].
    // Map the overall limits to the -1..+1 range expected by the OpenGL engine:
	double scaleTrans[4];
	linearMap(xmin, xmax, -1.0, 1.0, scaleTrans[0], scaleTrans[1]);
	linearMap(ymin, ymax, -1.0, 1.0, scaleTrans[2], scaleTrans[3]);
	for (int i=0 ; i<4 ; i++)
		scaleTransF[i] = static_cast<float>(scaleTrans[i]);
}

void ModelView::fetchGLSLVariableLocations()
{
	if (ModelView::shaderProgram > 0)
	{
		ModelView::ppuLoc_colorMode = ppUniformLocation(shaderProgram, "colorMode");
		ModelView::ppuLoc_scaleTrans = ppUniformLocation(shaderProgram, "scaleTrans");
		ModelView::pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
	    ModelView::pvaLoc_mvMinBounds = pvAttribLocation(shaderProgram, "mvMinBounds");
	    ModelView::pvaLoc_mvMaxBounds = pvAttribLocation(shaderProgram, "mvMaxBounds");
        ModelView::pvaLoc_mvColor = pvAttribLocation(shaderProgram, "mvColor");
        ModelView::ppuLoc_mvNumOfCircles = ppUniformLocation(shaderProgram, "numberOfCircles");
    }
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void ModelView::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin; xyzLimits[1] = xmax;
	xyzLimits[2] = ymin; xyzLimits[3] = ymax;
	xyzLimits[4] = -1.0; xyzLimits[5] =  1.0;
}

void ModelView::handleCommand(unsigned char key, double ldsX, double ldsY)
{
    std::cout << "ldsX: " << ldsX << "ldsY: " << ldsY << "\n";
}

// linearMap determines the scale and translate parameters needed in
// order to map a value, f (fromMin <= f <= fromMax) to its corresponding
// value, t (toMin <= t <= toMax). Specifically: t = scale*f + trans.
void ModelView::linearMap(double fromMin, double fromMax, double toMin, double toMax,
					  double& scale, double& trans) // CLASS METHOD
{
	scale = (toMax - toMin) / (fromMax - fromMin);
	trans = toMin - scale*fromMin;
}

GLint ModelView::ppUniformLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetUniformLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-primitive uniform: '" << name << "'\n";
	return loc;
}

GLint ModelView::pvAttribLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetAttribLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-vertex attribute: '" << name << "'\n";
	return loc;
}

void ModelView::render() const
{
    if ((vao[0] == 0)  || // if it has been deleted
	    (!visible))    // if it has been temporarily blanked
		return;    
    
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);

	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderProgram);

	float scaleTrans[4];
	//vec2 mvBounds[2] = { { xmin, ymin }, { xmax, ymax } };
	computeScaleTrans(scaleTrans);
	glUniform4fv(ModelView::ppuLoc_scaleTrans, 1, scaleTrans);
	
	// pass in "2" because there is 2 vec2's
	//glUniform2fv(ModelView::ppuLoc_mvBounds, 2, mvBounds[0]);
    //glUniform4fv(ModelView::ppuLoc_mvColor, 1, mvColor);
    glUniform1i(ModelView::ppuLoc_mvNumOfCircles, numberOfCircles);

	// establish the current color mode and draw
	glUniform1i(ModelView::ppuLoc_colorMode, colorMode);

	// bind the current triangle VAO
	glBindVertexArray(vao[0]);

	// Draw the box.
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	// restore the previous program
	glUseProgram(pgm);
}

void ModelView::setMCRegionOfInterest(double xyz[6])
{
	for (int i=0 ; i<6 ; i++)
		mcRegionOfInterest[i] = xyz[i];
}

void ModelView::defineGeometry(vec2* vertices) // num of vertices is a class variable
{
    // store the min and max for x and y, so we can use them in the fragment shader
	xmin = xmax = vertices[0][0];
	ymin = ymax = vertices[0][1];
	for (int i=1 ; i<numVertices ; i++)
	{
		if (vertices[i][0] < xmin)
			xmin = vertices[i][0];
		else if (vertices[i][0] > xmax)
			xmax = vertices[i][0];
		if (vertices[i][1] < ymin)
			ymin = vertices[i][1];
		else if (vertices[i][1] > ymax)
			ymax = vertices[i][1];
	}
	
	// the size of 4 is for each vertex
	vec2 mvMaxBounds[4] = { {xmax, ymax}, {xmax, ymax}, {xmax, ymax}, {xmax, ymax} };
	vec2 mvMinBounds[4] = { {xmin, ymin}, {xmin, ymin}, {xmin, ymin}, {xmin, ymin} };
    vec4 mvColors[4] = { {mvColor[0], mvColor[1], mvColor[2], mvColor[3]},
                         {mvColor[0], mvColor[1], mvColor[2], mvColor[3]},
                         {mvColor[0], mvColor[1], mvColor[2], mvColor[3]},
                         {mvColor[0], mvColor[1], mvColor[2], mvColor[3]} 
                       };

	glGenVertexArrays(1, vao);
	glGenBuffers(4, vbo); // 0 for position, 1 for model view max bounds, 2 for model view min bounds, and 3 for color
	
	glBindVertexArray(vao[0]);
	
	// Allocate and send data to GPU (mc position)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	int numBytesInBuffer = numVertices * sizeof(vec2);
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(ModelView::pvaLoc_mcPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ModelView::pvaLoc_mcPosition);
	
	// Allocate and send data to GPU (model view maxBound)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//int numBytesInBuffer = numVertices * sizeof(vec2);
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, mvMaxBounds, GL_STATIC_DRAW);
	glVertexAttribPointer(ModelView::pvaLoc_mvMaxBounds, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ModelView::pvaLoc_mvMaxBounds);
	
	// Allocate and send data to GPU (model view minBound)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	//int numBytesInBuffer = numVertices * sizeof(vec2);
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, mvMinBounds, GL_STATIC_DRAW);
	glVertexAttribPointer(ModelView::pvaLoc_mvMinBounds, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ModelView::pvaLoc_mvMinBounds);
	
	// Allocate and send data to GPU (model view color)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	int numBytesInBuffer_Color = numVertices * sizeof(vec4);
	glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer_Color, mvColors, GL_STATIC_DRAW);
	glVertexAttribPointer(ModelView::pvaLoc_mvColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ModelView::pvaLoc_mvColor);
}
