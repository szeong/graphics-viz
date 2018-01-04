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

    cerr << "\t>>> evalBezier has been called" << endl;
    cerr << "\t>>> Control points (type vector< Vector3f >): " << endl;
		cerr << "\t>>> Bezier curve points (type vector< Vector3f >): " << endl;

		// Initialise G, C matrices
		Matrix4f BCubic(Vector4f(-1, 3, -3, 1), Vector4f(3, -6, 3, 0), Vector4f(-3, 3, 0, 0), Vector4f(1, 0, 0, 0));
		vector<Vector3f> C;
		Vector3f CVertex;

		// Compute curve coordinates Cx, Cy, Cz
		for (unsigned i = 0; i < steps; i++) {
			float tInterval = i / (float(steps) - 1);
			// for coordinate info
			Vector4f TCubic(tInterval * tInterval * tInterval, tInterval * tInterval, tInterval, 1);
			Vector4f BTCubic(BCubic * TCubic);

			// coordinate info calc
			CVertex[0] = P[0][0] * BTCubic[0] + P[1][0] * BTCubic[1] + P[2][0] * BTCubic[2] + P[3][0] * BTCubic[3];
			CVertex[1] = P[0][1] * BTCubic[0] + P[1][1] * BTCubic[1] + P[2][1] * BTCubic[2] + P[3][1] * BTCubic[3];
			CVertex[2] = P[0][2] * BTCubic[0] + P[1][2] * BTCubic[1] + P[2][2] * BTCubic[2] + P[3][2] * BTCubic[3];
			C.push_back(CVertex);

			//cout << "\t>>> t-value: " << tInterval << endl;
			//cout << "\t>>> C-vector: [" << CVertex[0] << " " << CVertex[1] << " " << CVertex[2] << "]" << endl;
		}

    cerr << "\t>>> Steps (type steps): " << steps << endl;

		vector<CurvePoint> cpVector;
		for (unsigned i = 0; i < steps; i++) {
			struct CurvePoint cp = { C[i], Vector3f(1, 0, 0), Vector3f(0, 1, 0), Vector3f(0, 0, 1) };
			cpVector.push_back(cp);
		}

    // return and draw cubic bezier curve
    return Curve(cpVector);
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

		
		// Initialise bspline -> bezier control point coordinates Gx, Gy, Gz
		Matrix4f B_Bezier(Vector4f(-1, 3, -3, 1), Vector4f(3, -6, 3, 0), Vector4f(-3, 3, 0, 0), Vector4f(1, 0, 0, 0));
		Matrix4f B_Spline(Vector4f(-1, 3, -3, 1), Vector4f(3, -6, 3, 0), Vector4f(-3, 0, 3, 0), Vector4f(1, 4, 1, 0));
		//Matrix4f B_Bezier_inv(B_Bezier.inverse());
		vector<Vector3f> G;
		Vector3f GVertex;

		//Matrix4f B1B2_inv((1.0/6.0) * B_Spline * B_Bezier_inv);
		Matrix4f B1B2_inv(Vector4f(0, 1.0/6.0, 2.0/3.0, 1.0/6.0), Vector4f(0, 1.0/3.0, 2.0/3.0, 0), Vector4f(0, 2.0/3.0, 1.0/3.0, 0), Vector4f(1.0/6.0, 2.0/3.0, 1.0/6.0, 0));
		//G = P * B1B2_inv;
		for (unsigned i = 0; i < P.size(); ++i) {
			cout << "\t>>> P-vector: [" << P[i][0] << " " << P[i][1] << " " << P[i][2] << "]" << endl;
			//cout << "\t>>> P-vector: [" << B1B2_inv(0, i) << " " << B1B2_inv(1, i) << " " << B1B2_inv(2, i) << " " << B1B2_inv(3, i) << "]" << endl;
			GVertex[0] = P[0][0] * B1B2_inv(0, i) + P[1][0] * B1B2_inv(1, i) + P[2][0] * B1B2_inv(2, i) + P[3][0] * B1B2_inv(3, i);
			GVertex[1] = P[0][1] * B1B2_inv(0, i) + P[1][1] * B1B2_inv(1, i) + P[2][1] * B1B2_inv(2, i) + P[3][1] * B1B2_inv(3, i);
			GVertex[2] = P[0][2] * B1B2_inv(0, i) + P[1][2] * B1B2_inv(1, i) + P[2][2] * B1B2_inv(2, i) + P[3][2] * B1B2_inv(3, i);
			cout << "\t>>> NewCtrlPt " << i << ": [" << GVertex[0] << " " << GVertex[1] << " " << GVertex[2] << "]" << endl; 
			G.push_back(GVertex);
		}
    cerr << "\t>>> Steps (type steps): " << steps << endl;

    // return and draw cubic b-spline
    return evalBezier(G, steps);
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

