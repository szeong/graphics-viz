#include "curve.h"
#include "extra.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
using namespace std;

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx( const Vector3f& lhs, const Vector3f& rhs )
    {
        const float eps = 1e-8f;
        return ( lhs - rhs ).absSquared() < eps;
    }
}
    

Curve evalBezier(const vector<Vector3f>& P, unsigned steps)
{
    // Check if curve is (composite) cubic
    if( P.size() < 4 || P.size() % 3 != 1 )
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit( 0 );
    }
		if (P.size() > 4) {
			int blocks((P.size() - 1) / 3);
			for (unsigned i = 0; i < blocks; ++i) {
				vector<Vector3f>::const_iterator first = P.begin() + 3*i;
				vector<Vector3f>::const_iterator last = P.begin() + 3*(i+1);
				vector<Vector3f> subVec(first, last);
				evalBezier(subVec, steps/blocks);
			}
			return Curve();
		}

    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.

    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.

    cerr << "\t>>> evalBezier has been called with the following input:" << endl;
    cerr << "\t>>> Control points (type vector< Vector3f >): " << endl;

		unsigned ctrlPts = 4;

		// Initialise control point coordinates Gx, Gy, Gz
		vector<Vector3f> G;
		Vector3f GVertex;
    for( unsigned i = 0; i < ctrlPts; ++i ) {
			GVertex[0] = P[i][0];
			GVertex[1] = P[i][1];
			GVertex[2] = P[i][2];
			cout << "\t>>> CtrlPt " << i << ": [" << GVertex[0] << " " << GVertex[1] << " " << GVertex[2] << "]" << endl;
			G.push_back(GVertex);
    }

		cerr << "\t>>> Bezier curve points (type vector< Vector3f >): " << endl;

		// Compute curve coordinates Cx, Cy, Cz
		Matrix4f B(Vector4f(-1, 3, -3, 1), Vector4f(3, -6, 3, 0), Vector4f(-3, 3, 0, 0), Vector4f(1, 0, 0, 0));
		vector<Vector3f> C;
		Vector3f CVertex;
		//float tInterval = 0.1;
		//float dt = 1 / (steps - 1);
		for (unsigned i = 0; i < steps; ++i) {
			float tInterval = 0 + i * (1 /(steps - 1));
			Vector4f T(tInterval * tInterval * tInterval, tInterval * tInterval, tInterval, 1);
			cout << "\t>>> i-value: " << i << endl;
			cout << "\t>>> t-value: " << tInterval << endl;
			cout << "\t>>> T-vector: [" << T[0] << " " << T[1] << " " << T[2] << "]" << endl;
			// fuck this shit im outta here for now

			//Vector4f BT(B * T);
			//CVertex[0] = G[0][0] * BT[0] + G[1][0] * BT[1] + G[2][0] * BT[2] + G[3][0] * BT[3];
			//CVertex[1] = G[0][1] * BT[0] + G[1][1] * BT[1] + G[2][1] * BT[2] + G[3][1] * BT[3];
			//CVertex[2] = G[0][2] * BT[0] + G[1][2] * BT[1] + G[2][2] * BT[2] + G[3][2] * BT[3];
			//tInterval += dt;
			//cout << ">>> t-step: "  << tInterval << endl;
			//C.push_back(CVertex);
			//cout << "\t>>> CurvePt " << i << ": [" << C[i][0] << " " << C[i][1] << " " << C[i][2] << "]" << endl;
		}

		/*for (unsigned i = 0; i < steps; ++i) {
			float t = 0 + i * (1 / (steps - 1));
			Vector4f tVec(t * t * t, t * t, t, 1);

			cout << ">>> i-value: " << i << endl;
			cout << ">>> t-value: " << t << endl;
			cout << ">>> T-vector: [" << tVec[0] << " " << tVec[1] << " " << tVec[2] << "]" << endl;
		}*/

    cerr << "\t>>> Steps (type steps): " << steps << endl;

    // return and draw cubic bezier curve
    return Curve(/* add parameters here */);
}

Curve evalBspline( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 )
    {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit( 0 );
    }

    // TODO:
    // It is suggested that you implement this function by changing
    // basis from B-spline to Bezier.  That way, you can just call
    // your evalBezier function.

    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    cerr << "\t>>> Returning empty curve." << endl;

    // Return an empty curve right now.
    return Curve();
}

Curve evalCircle( float radius, unsigned steps )
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).
    
    // Preallocate a curve with steps+1 CurvePoints
    Curve R( steps+1 );

    // Fill it in counterclockwise
    for( unsigned i = 0; i <= steps; ++i )
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float( i ) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vector3f( cos(t), sin(t), 0 );
        
        // Tangent vector is first derivative
        R[i].T = Vector3f( -sin(t), cos(t), 0 );
        
        // Normal vector is second derivative
        R[i].N = Vector3f( -cos(t), -sin(t), 0 );

        // Finally, binormal is facing up.
        R[i].B = Vector3f( 0, 0, 1 );
    }

    return R;
}

void drawCurve( const Curve& curve, float framesize )
{
    // Save current state of OpenGL
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Setup for line drawing
    glDisable( GL_LIGHTING ); 
    glColor4f( 1, 1, 1, 1 );
    glLineWidth( 1 );
    
    // Draw curve
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i < curve.size(); ++i )
    {
        glVertex( curve[ i ].V );
    }
    glEnd();

    glLineWidth( 1 );

    // Draw coordinate frames if framesize nonzero
    if( framesize != 0.0f )
    {
        Matrix4f M;

        for( unsigned i = 0; i < curve.size(); ++i )
        {
            M.setCol( 0, Vector4f( curve[i].N, 0 ) );
            M.setCol( 1, Vector4f( curve[i].B, 0 ) );
            M.setCol( 2, Vector4f( curve[i].T, 0 ) );
            M.setCol( 3, Vector4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf( M );
            glScaled( framesize, framesize, framesize );
            glBegin( GL_LINES );
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }
    
    // Pop state
    glPopAttrib();
}

