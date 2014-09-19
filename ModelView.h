// ModelView.h - a basic combined Model and View for OpenGL

#ifndef MODELVIEW_H
#define MODELVIEW_H

class ShaderIF;

#include <string>

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

typedef float vec2[2];
typedef float vec4[4];

class ModelView
{
public:
	ModelView(vec2* vertices, int numberOfCircles, vec4 color);
	virtual ~ModelView();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render() const;

	static void setMCRegionOfInterest(double xyz[6]);

private:
	GLuint vao[1]; // VAO for the triangle
	GLuint vbo[3]; // VBO - 0 for position, 1 for model view maxBounds, 2 for model view minBounds, and 3 for color
    vec4 mvColor;
    int numberOfCircles;
	
	// interactive display options
	int colorMode; // flag to tell fragment shader how to assign colors
	bool visible; // i.e., whether we actually currently draw it
	double xmin, xmax, ymin, ymax;

	// we assume all instances share the same shader program:
	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	
	// "pvaLoc_" prefix means shader program location of an "attribute" variable
	// "ppuLoc_" prefix means shader program location of an "uniform" variable
	static GLint ppuLoc_colorMode; // a uniform variable (per-primitive)
	static GLint ppuLoc_scaleTrans; // a uniform used to scale MC to LDS
	static GLint pvaLoc_mcPosition; // an attribute variable (per-vertex)
    static GLint pvaLoc_mvMaxBounds; // the bounds of this model.
    static GLint pvaLoc_mvMinBounds;
    static GLint pvaLoc_mvColor; // color to be used when coloring the circles
    static GLint ppuLoc_mvNumOfCircles;

	// "pp": "per-primitive"; "pv": "per-vertex"
	static GLint ppUniformLocation(GLuint glslProgram, const std::string& name);
	static GLint pvAttribLocation(GLuint glslProgram, const std::string& name);

	// Routines for computing parameters necessary to map from arbitrary
	// model coordinate ranges into OpenGL's -1..+1 Logical Device Space.
	// 1. linearMap determines the scale and translate parameters needed in
	//    order to map a value, f (fromMin <= f <= fromMax) to its corresponding
	//    value, t (toMin <= t <= toMax). Specifically: t = scale*f + trans.
	static void linearMap(double fromMin, double fromMax, double toMin, double toMax,
						  double& scale, double& trans);
	// 2. computeScaleTrans determines the current model coordinate region of
	//    interest and then uses linearMap to determine how to map coordinates
	//    in the region of interest to their proper location in Logical Device
	//    Space. (Returns float[] because glUniform currently allows only float[].)
	static void computeScaleTrans(float* scaleTrans);

	static double mcRegionOfInterest[6];

	static void fetchGLSLVariableLocations();
	
	void defineGeometry(vec2* vertices);
	void deleteObject();
    void setColor(vec4 color);
};

#endif
