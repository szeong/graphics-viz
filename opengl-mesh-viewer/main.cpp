#include "GL/freeglut.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include "vecmath.h"
using namespace std;

// GLOBAL VARIABLES

// List of vertices (3D vectors)
vector<Vector3f> vecv;

// List of normals corresponding to vertices in 'vecv' (also 3D vectors)
vector<Vector3f> vecn;

// List of faces (indices into vecv and vecn)
vector<vector<unsigned>> vecf;

// Counter for changing light colour
int lightCounter = 0;

// Counter for changing position
GLfloat positionCounterX = 0;
GLfloat positionCounterY = 0;


// These are convenience functions which allow us to call OpenGL methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // changes color of object to yellow
				if (lightCounter != 20)
					++lightCounter;
				else
					lightCounter = 0;
        cout << "Colour modified" << endl;
        break;
    default:
        cout << "Unhandled key entry: " << key << endl;
				break;
    }

		// this will refresh the screen so that the user sees the color change
		glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
			positionCounterY = positionCounterY + 0.5;
		cout << "Light position: " << positionCounterX + 1.0 << ", " << positionCounterY + 1.0 << endl;
		break;
    case GLUT_KEY_DOWN:
			positionCounterY = positionCounterY - 0.5;
			cout << "Light position: " << positionCounterX + 1.0 << ", " << positionCounterY + 1.0 << endl;
		break;
    case GLUT_KEY_LEFT:
			positionCounterX = positionCounterX - 0.5;
			cout << "Light position: " << positionCounterX + 1.0 << ", " << positionCounterY + 1.0 << endl;
		break;
    case GLUT_KEY_RIGHT:
			positionCounterX = positionCounterX + 0.5;
			cout << "Light position: " << positionCounterX + 1.0 << ", " << positionCounterY + 1.0 << endl;
		break;
		default:
			cout << "Unhandled key entry" << endl;
    }

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void)
{
		int light = lightCounter;
		double posX = positionCounterX;
		double posY = positionCounterY;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

	// Here are some colors you might use - feel free to add more
    GLfloat diffColors[21][4] = {{0.0, 0.0, 0.0, 1.0},
																 {0.1, 0.1, 0.1, 1.0},
                                 {0.2, 0.1, 0.2, 1.0},
																 {0.3, 0.2, 0.3, 1.0},
																 {0.3, 0.3, 0.4, 1.0},
																 {0.4, 0.4, 0.5, 1.0},
																 {0.5, 0.5, 0.6, 1.0},
																 {0.6, 0.6, 0.7, 1.0},
																 {0.7, 0.7, 0.6, 1.0},
																 {0.8, 0.8, 0.5, 1.0},
																 {0.9, 0.7, 0.4, 1.0},
                                 {1.0, 0.6, 0.3, 1.0},
																 {0.9, 0.5, 0.4, 1.0},
																 {0.8, 0.4, 0.3, 1.0},
																 {0.7, 0.3, 0.4, 1.0},
																 {0.6, 0.3, 0.3, 1.0},
																 {0.5, 0.4, 0.4, 1.0},
																 {0.4, 0.3, 0.3, 1.0},
																 {0.3, 0.2, 0.2, 1.0},
																 {0.2, 0.1, 0.2, 1.0},
                                 {0.1, 0.1 ,0.1, 1.0}};
    
	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[light]);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};

    // Light position
		GLfloat Lt0pos[] = {1.0 + posX, 1.0 + posY, 5.0, 1.0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

		// This GLUT method draws a teapot.  You should replace
		// it with code which draws the object you loaded.
		// glutSolidTeapot(1.0);
		glBegin(GL_TRIANGLES);
		for (unsigned int i = 0; i < vecv.size(); i++) {
			glNormal3d(vecn[i][0], vecn[i][1], vecn[i][2]);
			glVertex3d(vecv[i][0], vecv[i][1], vecv[i][2]);
		}
		glEnd(); 
    
    // Dump the image to the screen.
    glutSwapBuffers();


}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput()
{

		// Get filename from standard input
getFilename:
		cout << "Enter the filename of the object you wish to render. File must be in same directory.\n";
		char filename[1024];
		cin.getline(filename, 1024);
		ifstream inf(filename);

		// Check if file can be read at all, else prompt for filename again
		if (!inf)
		{
			// Print an error and prompt for another file
			cerr << "Unable to read file input. Please try again or input another file." << endl;
			goto getFilename;
		}

		// Check if file is of the correct type (.obj), else prompt for filename again
		string buffer;
		string overflow;
		int validCounter = 0;
		for (int i = 0; i < 7; i++)
		{
			getline(inf, buffer);
			stringstream ss(buffer);
			ss >> overflow;
			if (overflow == "mtllib")
			{
				cout << "OBJ data found. Proceeding to render..." << endl;
				++validCounter;
				break;
			}
		}
		if (validCounter == 0)
		{
			cout << "File does not contain OBJ data. Please use another file." << endl;
			goto getFilename;
		}

		// If file data is of OBJ type and valid, proceed to extract geometry info
		while (inf)
		{

		// Check for 'v', 'vn' and f' lines
		getline(inf, buffer);
		stringstream ss(buffer);
		Vector3f transferV;
		vector<string> transferF; // outputting ss into transferF throws an exception
		ss >> overflow;
		string f1; // these three individual strings work, but why wtf???
		string f2;
		string f3;

		if (overflow == "v")
			{
				// Transfer vertex point info 'v' to 'vecv'
				ss >> transferV[0] >> transferV[1] >> transferV[2];
				vecv.push_back(transferV);
			}
			else if (overflow == "vn") {
				// Transfer vertex normal info 'vn' to 'vecn'
				ss >> transferV[0] >> transferV[1] >> transferV[2];
				vecn.push_back(transferV);
			}
			else if (overflow == "f") {
				// Transfer face info 'f' to 'vecf'
				ss >> f1 >> f2 >> f3;
				cout << f1 << f2 << f3 << endl;
				// ss >> transferF[0] >> transferF[1] >> transferF[2];
				// cout << "f: " << transferF[0] << " " << transferF[1] << " " << transferF[2] << endl;
				// vecf.push_back(transferf);
			}
		}

}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 480, 480 );
    glutCreateWindow("Assignment 0 - OpenGL Mesh Viewer");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
