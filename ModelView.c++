// AUTHOR: ZANE RALSTON
// DATE: SEPTEMBER 19, 2014
// EMAIL: z641r311@ku.edu
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
GLint ModelView::ppuLoc_mvColor = -2; 
GLint ModelView::pvaLoc_relPosition = -2;
GLint ModelView::ppuLoc_disableCircle = -2;

double ModelView::mcRegionOfInterest[6] = { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 };

static const int numVertices = 4;

ModelView::ModelView(vec2* vertices, vec4 colors[]) : colorMode(0)
{
    if (ModelView::shaderProgram == 0)
    {
        // Create the common shader program:
        ModelView::shaderIF = new ShaderIF("project1.vsh", "project1.fsh");
        ModelView::shaderProgram = shaderIF->getShaderPgmID();
        fetchGLSLVariableLocations();
    }
    for(int i = 0; i < 7; i++)
    {
        disableCircle[i] = 0;
    }
    setColors(colors);
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

void ModelView::setColors(vec4 colors[])
{
    mvColor = colors;
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
        ModelView::ppuLoc_mvColor = ppUniformLocation(shaderProgram, "userColors");
        ModelView::pvaLoc_relPosition = pvAttribLocation(shaderProgram, "relativePos");
        ModelView::ppuLoc_disableCircle = ppUniformLocation(shaderProgram, "disableCircle");
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
    // scaleTrans is [sX, tX, sY, tY]
    float scaleTrans[4];
    computeScaleTrans(scaleTrans);
    float mcX = (ldsX - scaleTrans[1])/(scaleTrans[0]);
    float mcY = (ldsY - scaleTrans[3])/(scaleTrans[2]);
    
    // if this is the correct modelview
    if((mcX > xmin) && (mcX < xmax) && (mcY > ymin) && (mcY < ymax))
    {
        if(key == 'c')
            colorMode = (colorMode + 1) % 4;
        else if(key == '0')
            disableCircle[0] = (disableCircle[0] + 1) % 2;
        else if(key == '1')
            disableCircle[1] = (disableCircle[1] + 1) % 2;
        else if(key == '2')
            disableCircle[2] = (disableCircle[2] + 1) % 2;
        else if(key == '3')
            disableCircle[3] = (disableCircle[3] + 1) % 2;
        else if(key == '4')
            disableCircle[4] = (disableCircle[4] + 1) % 2;
        else if(key == '5')
            disableCircle[5] = (disableCircle[5] + 1) % 2;
        else if(key == '6')
            disableCircle[6] = (disableCircle[6] + 1) % 2;
    }
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

    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);

    // draw the triangles 
    glUseProgram(shaderProgram);

    float scaleTrans[4];
    computeScaleTrans(scaleTrans);
    glUniform4fv(ModelView::ppuLoc_scaleTrans, 1, scaleTrans);

    // set the color of the circles
    glUniform4fv(ModelView::ppuLoc_mvColor, 5, mvColor[0]);
    
    // set the currect color mode
    glUniform1i(ModelView::ppuLoc_colorMode, colorMode);

    // set the disabled circles
    glUniform1iv(ModelView::ppuLoc_disableCircle, 7, disableCircle);

    // bind the current triangle VAO
    glBindVertexArray(vao[0]);

    // Draw the box.
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
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
    // store the min and max for x and y
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
    
    // I assume vertices are coming in as bottom left, bottom right, top left,
    // top right.
    vec2 relPosition[4] = { { -1.0, -1.0 }, { 1.0, -1.0 }, { -1.0, 1.0 }, { 1.0, 1.0 } };

    glGenVertexArrays(1, vao);
    glGenBuffers(2, vbo); // 0 for position, and 2 for local coord system 
    
    glBindVertexArray(vao[0]);
    
    // Allocate and send data to GPU (mc position)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    int numBytesInBuffer = numVertices * sizeof(vec2);
    glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(ModelView::pvaLoc_mcPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ModelView::pvaLoc_mcPosition);
    
    // Allocate and send data to GPU (relative positioning for the model view)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, numBytesInBuffer, relPosition, GL_STATIC_DRAW);
    glVertexAttribPointer(ModelView::pvaLoc_relPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(ModelView::pvaLoc_relPosition);
    
}
