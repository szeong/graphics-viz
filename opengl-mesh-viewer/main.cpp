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
vector<Vector3f> vecv;         // Array of vertex coordinates
vector<Vector3f> vecn;         // Array of normal vectors corresponding to vertices in 'vecv'
vector<vector<unsigned>> vecf; // Array of faces (indices to vecv and vecn)
int lightCounter(0);           // Counter for changing light colour
GLfloat positionCounterX(0);   // Counter for changing x-position
GLfloat positionCounterY(0);   // Counter for changing y-position
bool rotateInd(false);         // If rotate toggle on, then true
float angle(0.0);              // Initial angle of object
GLfloat mtrx[16];


// Convenience functions to call OpenGL methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }
inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// Rotate object with 16.666 ms interval
void rotateObject(int value) {
	if (angle >= 360.0) {
		angle = -360.0;
	}
	else {
		angle += 1.0;
	}
	glutPostRedisplay();
	glutTimerFunc(16.666, rotateObject, 0); // 60 fps
}


// Receive "normal" key presses
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
      exit(0);
      break;
    case 'c': // Change colour of object
			if (lightCounter != 3) {
				++lightCounter;
				glutPostRedisplay();
			}
			else {
				lightCounter = 0;
				glutPostRedisplay(); // update screen with changes
			}
			cout << "Colour modified." << endl;
      break;
		case 'r': // Toggle rotation of object
			rotateInd = !rotateInd;
			if (rotateInd) {
				glutTimerFunc(100, rotateObject, 0);
				cout << "Rotation enabled." << endl;
			}
			else {
				cout << "Rotation disabled." << endl;
			}
			break;
    default:
      cout << "Unhandled key entry: " << key << endl;
			break; 
    }
}

// Receive special key presses
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

    glutPostRedisplay(); // update screen with changes
}

// This function is responsible for displaying the object
void drawScene(void)
{
		int light = lightCounter;
		GLfloat posX = positionCounterX;
		GLfloat posY = positionCounterY;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
		glPushMatrix();
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

	  // Colour bank
    GLfloat diffColors[21][4] = {{0.5, 0.5, 0.6, 1.0},
                                 {1.0, 0.6, 0.3, 1.0},
																 {0.9, 0.5, 0.4, 1.0},
																 {0.4, 0.3, 0.3, 1.0}};
    
	  // Use first colour entry as diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[light]);

	  // Define specular colour and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	  // Note that specular colour and shininess are constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // Set light properties
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0}; // light colour (RGBA)
		GLfloat Lt0pos[] = {1.0 + posX, 1.0 + posY, 5.0, 1.0}; // light position

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

		if (rotateInd) {
			glRotatef(angle, 0.0f, 1.0f, 0.0f);
			glGetFloatv(GL_MODELVIEW_MATRIX, mtrx);

			// Draw triangles from OBJ data
			glBegin(GL_TRIANGLES);
			for (unsigned int i = 0; i < vecf.size(); i++) {
				glNormal3d(vecn[vecf[i][2] - 1][0], vecn[vecf[i][2] - 1][1], vecn[vecf[i][2] - 1][2]);
				glVertex3d(vecv[vecf[i][0] - 1][0], vecv[vecf[i][0] - 1][1], vecv[vecf[i][0] - 1][2]);
				glNormal3d(vecn[vecf[i][5] - 1][0], vecn[vecf[i][5] - 1][1], vecn[vecf[i][5] - 1][2]);
				glVertex3d(vecv[vecf[i][3] - 1][0], vecv[vecf[i][3] - 1][1], vecv[vecf[i][3] - 1][2]);
				glNormal3d(vecn[vecf[i][8] - 1][0], vecn[vecf[i][8] - 1][1], vecn[vecf[i][8] - 1][2]);
				glVertex3d(vecv[vecf[i][6] - 1][0], vecv[vecf[i][6] - 1][1], vecv[vecf[i][6] - 1][2]);
			}
			glEnd();
		}
		else {
			// Draw triangles from OBJ data
			glBegin(GL_TRIANGLES);
			for (unsigned int i = 0; i < vecf.size(); i++) {
				glNormal3d(vecn[vecf[i][2] - 1][0], vecn[vecf[i][2] - 1][1], vecn[vecf[i][2] - 1][2]);
				glVertex3d(vecv[vecf[i][0] - 1][0], vecv[vecf[i][0] - 1][1], vecv[vecf[i][0] - 1][2]);
				glNormal3d(vecn[vecf[i][5] - 1][0], vecn[vecf[i][5] - 1][1], vecn[vecf[i][5] - 1][2]);
				glVertex3d(vecv[vecf[i][3] - 1][0], vecv[vecf[i][3] - 1][1], vecv[vecf[i][3] - 1][2]);
				glNormal3d(vecn[vecf[i][8] - 1][0], vecn[vecf[i][8] - 1][1], vecn[vecf[i][8] - 1][2]);
				glVertex3d(vecv[vecf[i][6] - 1][0], vecv[vecf[i][6] - 1][1], vecv[vecf[i][6] - 1][2]);
			}
			glEnd();
		}

    glutSwapBuffers(); 		// dump image to the screen
}

// Initialize OpenGL rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0
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


// Prompt for user file input to render OBJ
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
			istringstream ss(buffer);
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
			cerr << "File does not contain OBJ data. Please use another file." << endl;
			goto getFilename;
		}

	// If file data is of OBJ type and valid, proceed to extract geometry info
	while (getline(inf, buffer)) {

		// Read lines from file
		istringstream ss(buffer);
		Vector3f transferV;
		vector<unsigned int> transferF;
		vector<string> faceVector(3);

		// Break loop when end of file is reached
		if (!(ss >> overflow))
		{
			cout << "End of file reached." << endl;
			break;
		}

		// Extract info if line contains 'v', 'vn' and 'f' header
		if (overflow == "v") {
			// Transfer vertex point info 'v' to 'vecv'
			ss >> transferV[0] >> transferV[1] >> transferV[2];
			//cout << "v: " << transferV[0] << " " << transferV[1] << " " << transferV[2] << endl;
			vecv.push_back(transferV);
		}
		else if (overflow == "vn") {
			// Transfer vertex normal info 'vn' to 'vecn'
			ss >> transferV[0] >> transferV[1] >> transferV[2];
			//cout << "vn: " << transferV[0] << " " << transferV[1] << " " << transferV[2] << endl;
			vecn.push_back(transferV);
		}
		else if (overflow == "f") {
			// Transfer face info 'f' to 'vecf'
			ss >> faceVector[0] >> faceVector[1] >> faceVector[2];
			for (unsigned int i = 0; i < faceVector.size(); i++) {
				istringstream iss(faceVector[i]);
				string token;
				while (getline(iss, token, '/'))
				{
					transferF.push_back(stoi(token));
				}
			}
			vecf.push_back(transferF);
			//cout << "f: " << transferF[0] << " " << transferF[1] << " " << transferF[2] << " " << transferF[3] << " " << transferF[4] << " " << transferF[5] << " " << transferF[6] << " " << transferF[7] << " " << transferF[8] << endl;
		}
	}
	cout << "OBJ data loaded." << endl;
}

// MAIN ROUTINE - Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{

		// Ask for user input to render object
    loadInput();

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 480, 480 );
    glutCreateWindow("OpenGL Mesh Viewer");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop - glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}