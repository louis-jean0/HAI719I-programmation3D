// Include standard headers
// #include <glm/detail/qualifier.hpp>
// #include <glm/ext/matrix_transform.hpp>
// #include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GL/glut.h>

using namespace glm;
using Vec3 = glm::vec3;

#include "src/shader.hpp"
#include "src/objloader.hpp"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
float deltaTime = 0.1f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;

static GLint window;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;


GLuint programID;
GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint elementbuffer;
GLuint LightID;
GLuint transformationMatrixID;


std::vector<unsigned short> indices; //Triangles concaténés dans une liste
std::vector<std::vector<unsigned short> > triangles;
std::vector<glm::vec3> indexed_vertices;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 TransformationMatrix;
glm::mat4 TransformationMatrix2;
glm::mat4 TransformationMatrix3;
glm::vec3 scaleTransformation = glm::vec3(1.0f,1.0f,1.0f);
glm::vec3 translationTransformation = glm::vec3(0.0f,0.0f,0.0f);
float radiansChaise3 = 10.0f; // Angle de rotation pour la 3ème chaise
float radiansSuzanne = 10.f; // Angle de rotation pour Suzanne


glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 0 );
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;


	// Right vector
glm::vec3 rightVector() {
    return glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
}

// Direction : Spherical coordinates to Cartesian coordinates conversion
glm::vec3 directionVector() {
    return glm::vec3(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );
}

void computeMatricesFromInputs(float moveX, float moveY);
void initLight ();
void init ();
void draw ();
void display ();
void idle ();
void key (unsigned char keyPressed, int x, int y);
void mouse (int button, int state, int x, int y);
void motion (int x, int y);
void reshape(int w, int h);
int main (int argc, char ** argv);
void printMatrix(const glm::mat4& mat);

// ------------------------------------

void printMatrix(const glm::mat4& mat) {
    std::cout << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << "\n" << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << "\n" << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << "\n" << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << std::endl;
}

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {
    // camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

}




// ------------------------------------
// rendering.
// ------------------------------------

void draw () {
    glUseProgram(programID);
    // Model matrix : an identity matrix (model will be at the origin) then change

    glm::mat4 ModelMatrix = glm::mat4(1.0f);

    // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up

    glm::mat4 ViewMatrix = glm::lookAt(camera_position,camera_target,camera_up);

    // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f),4.0f / 3.0f, 0.1f, 100.0f);

    // Send our transformation to the currently bound shader,
    // in the "Model View Projection" to the shader uniforms

    GLuint ModelMatrixLocation = glGetUniformLocation(programID,"ModelMatrix");
    GLuint ViewMatrixLocation = glGetUniformLocation(programID,"ViewMatrix");
    GLuint ProjectionMatrixLocation = glGetUniformLocation(programID,"ProjectionMatrix");

    glUniformMatrix4fv(ModelMatrixLocation,1,GL_FALSE,(const GLfloat *)&ModelMatrix[0]);
    glUniformMatrix4fv(ViewMatrixLocation,1,GL_FALSE,(const GLfloat *)&ViewMatrix[0]);
    glUniformMatrix4fv(ProjectionMatrixLocation,1,GL_FALSE,(const GLfloat *)&ProjectionMatrix[0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    glUniformMatrix4fv(transformationMatrixID, 1, GL_FALSE, (const GLfloat*)&TransformationMatrix[0]);

    // Draw the triangles !
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_SHORT,(void*)0);

    // Afficher une seconde chaise

    // glUniformMatrix4fv(transformationMatrixID, 1, GL_FALSE, (const GLfloat*)&TransformationMatrix2[0]);

    // glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_SHORT,(void*)0);

    // Afficher une troisieme chaise!

    // glUniformMatrix4fv(transformationMatrixID, 1, GL_FALSE, (const GLfloat*)&TransformationMatrix3[0]);

    // glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_SHORT,(void*)0);

    glDisableVertexAttribArray(0);
}


void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
    deltaTime = time - lastFrame;
    lastFrame = time;
}

void key (unsigned char keyPressed, int x, int y) {
    float cameraSpeed = 2.5 * deltaTime;
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCR_WIDTH, SCR_HEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;

    case 'a':
        camera_position -= cameraSpeed * camera_target;
        break;

    case 'w':
        camera_position += cameraSpeed * camera_target;
        break;
    
    case '+':
        scaleTransformation += glm::vec3(0.1f);
        TransformationMatrix = glm::mat4(1.0f);
        TransformationMatrix = glm::scale(TransformationMatrix, scaleTransformation);
        TransformationMatrix = glm::translate(TransformationMatrix, translationTransformation);
        glUniformMatrix4fv(transformationMatrixID,1,GL_FALSE,(const GLfloat *)&TransformationMatrix[0]);
        break;
    
    case '-':
        if(scaleTransformation.x > 0.0f) { // Condition pour limiter la mise à l'échelle (si x est > 0, alors y et z aussi vu l'implémentation)
            scaleTransformation -= glm::vec3(0.1f);
            TransformationMatrix = glm::mat4(1.0f);
            TransformationMatrix = glm::scale(TransformationMatrix, scaleTransformation);
            TransformationMatrix = glm::translate(TransformationMatrix, translationTransformation);
            glUniformMatrix4fv(transformationMatrixID,1,GL_FALSE,(const GLfloat *)&TransformationMatrix[0]);
        }
        break;

    case 'd':
        translationTransformation += glm::vec3(0.1f,0.0f,0.0f);
        TransformationMatrix = glm::mat4(1.0f);
        TransformationMatrix = glm::translate(TransformationMatrix, translationTransformation);
        TransformationMatrix = glm::scale(TransformationMatrix, scaleTransformation);
        glUniformMatrix4fv(transformationMatrixID,1,GL_FALSE,(const GLfloat *)&TransformationMatrix[0]);
        break;

    case 'q':
        translationTransformation -= glm::vec3(0.1f,0.0f,0.0f);
        TransformationMatrix = glm::mat4(1.0f);
        TransformationMatrix = glm::translate(TransformationMatrix, translationTransformation);
        TransformationMatrix = glm::scale(TransformationMatrix, scaleTransformation);
        glUniformMatrix4fv(transformationMatrixID,1,GL_FALSE,(const GLfloat *)&TransformationMatrix[0]);
        break;

    case 'z':
        translationTransformation += glm::vec3(0.0f,0.1f,0.0f);
        TransformationMatrix = glm::mat4(1.0f);
        TransformationMatrix = glm::translate(TransformationMatrix, translationTransformation);
        TransformationMatrix = glm::scale(TransformationMatrix, scaleTransformation);
        glUniformMatrix4fv(transformationMatrixID,1,GL_FALSE,(const GLfloat *)&TransformationMatrix[0]);
        break;

    case 's':
        translationTransformation -= glm::vec3(0.0f,0.1f,0.0f);
        TransformationMatrix = glm::mat4(1.0f);
        TransformationMatrix = glm::translate(TransformationMatrix, translationTransformation);
        TransformationMatrix = glm::scale(TransformationMatrix, scaleTransformation);
        glUniformMatrix4fv(transformationMatrixID,1,GL_FALSE,(const GLfloat *)&TransformationMatrix[0]);
        break;

    case '3': {
        glm::mat4 trans1 = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-0.5f,0.0f));
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f),glm::radians(radiansChaise3),glm::vec3(0.0f,0.0f,1.0f));
        glm::mat4 trans2 = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.5f,0.0f));
        TransformationMatrix3 = (trans2 * rot) * trans1;
        glUniformMatrix4fv(transformationMatrixID,1,GL_FALSE,(const GLfloat *)&TransformationMatrix3[0]);
        radiansChaise3 += 10.0f;
        break;
    }

    case 'r':
        TransformationMatrix = glm::rotate(glm::mat4(1.0f),glm::radians(radiansSuzanne),glm::vec3(1.0f,1.0f,1.0f)); // Rotations selon les trois axes
        radiansSuzanne+=10.0f;
        break;

    case 'l': {
        glm::vec3 axeObjet = glm::vec3(0.0f,0.1f,0.0f);
        glm::vec3 vecteurCible = glm::vec3(1.0f,1.0f,1.0f);
        glm::vec3 axeRotation = glm::cross(axeObjet,vecteurCible); // Axe de la rotation
        float angle = glm::acos(glm::dot(axeObjet,vecteurCible)); // Angle de la rotation
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f),angle,axeRotation);
        TransformationMatrix = rotationMatrix * TransformationMatrix;
    }

    case 'u':
        camera_position += glm::vec3(0.0f,cameraSpeed,0.0f);
        break;

    case 'j':
        camera_position -= glm::vec3(0.0f,cameraSpeed,0.0f);
        break;

    case 'h':
        camera_position -= glm::vec3(cameraSpeed,0.0f,0.0f);
        break;

    case 'k':
        camera_position += glm::vec3(cameraSpeed,0.0f,0.0f);
        break;

    default:
        break;
    }
    //TODO add translations
    idle ();
}

void specialKeys(int key, int x, int y) {
    if(key == GLUT_KEY_LEFT)
		position -= rightVector() * deltaTime * speed;
    else if(key == GLUT_KEY_RIGHT)
		position += rightVector() * deltaTime * speed;
    else if(key == GLUT_KEY_DOWN)
		position -= directionVector() * deltaTime * speed;
    else if(key == GLUT_KEY_UP)
        position += directionVector() * deltaTime * speed;
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            //camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
            lastX = x;
            lastY = y;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        computeMatricesFromInputs(x - lastX, y - lastY);
        lastX = x;
        lastY = y;
    }
    else if (mouseMovePressed == true) {
    }
    else if (mouseZoomPressed == true) {
    }
}

void computeMatricesFromInputs(float moveX, float moveY){
    std::cout << moveX << " " << moveY << std::endl;
	// Compute new orientation
	horizontalAngle += mouseSpeed * moveX / 10.f;
	verticalAngle   += mouseSpeed * moveY / 10.f;

	// Up vector
	glm::vec3 up = glm::cross( rightVector(), directionVector() );

	float FoV = initialFoV;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix
	ViewMatrix       = glm::lookAt(
								camera_position,           // Camera is here
								camera_position + directionVector(), // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );
}


void reshape(int w, int h) {
    // camera.resize (w, h);
}

int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCR_WIDTH, SCR_HEIGHT);
    window = glutCreateWindow ("TP HAI719I");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    glutSpecialFunc(specialKeys);
    key ('?', 0, 0);

    computeMatricesFromInputs(0.f, 0.f);

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    //Chargement du fichier de maillage
    std::string filename("data/suzanne.off");
    loadOFF(filename, indexed_vertices, indices, triangles );

    transformationMatrixID = glGetUniformLocation(programID,"TransformationMatrix");
    TransformationMatrix = glm::mat4(1.0f);
    //TransformationMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(-0.5f,-1.0f,0)) * glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f));
    //TransformationMatrix2 = glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,-1.0f,0)) * glm::scale(glm::mat4(1.0f),glm::vec3(-0.5f,0.5f,0.5f));

    // Load it into a VBO

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    glutMainLoop ();

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    return EXIT_SUCCESS;

}
