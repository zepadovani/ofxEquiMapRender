
#include "ofxCubeMapOld.h"

//--------------------------------------------------------------
ofxCubeMapOld::ofxCubeMapOld()
{
	fov 	= 90.0f;
	nearZ 	= 0.01f;
	farZ 	= 1024.0f;
	
	cubeMapCamerasRenderPosition.set( 0.0f, 0.0f, 0.0f );
	
	setupSkyBoxVertices();
	
}

//--------------------------------------------------------------
// these should all be the same size and all power of two
void ofxCubeMapOld::loadImages( string pos_x, string neg_x,
							 string pos_y, string neg_y,
							 string pos_z, string neg_z )
{	
	
	// We don't want the texture border hack to be on
	/*bool wantsTextureBorderHack = false;
	if( ofGetTextureEdgeHackEnabled() ) {
		wantsTextureBorderHack = true;
		ofDisableTextureEdgeHack();
		ofLogVerbose() << "ofxCubeMapOld:loadImages (string version), disabled texture hack, re-enabling when done.";
	}*/
	
	ofImage images[6];	
	bool loaded1 = images[0].loadImage(pos_x);
	bool loaded2 = images[1].loadImage(neg_x);
	bool loaded3 = images[2].loadImage(pos_y);
	bool loaded4 = images[3].loadImage(neg_y);
	bool loaded5 = images[4].loadImage(pos_z);
	bool loaded6 = images[5].loadImage(neg_z);
	
	if( loaded1 && loaded2 && loaded3 && loaded4 && loaded5 && loaded6 ) {}
	else { ofLogError() << "ofxCubeMapOld: failed to load one of the cubemaps!"; }
	
	loadFromOfImages(images[0],
					 images[1],
					 images[2],
					 images[3],
					 images[4],
					 images[5]);
	
	/* if( wantsTextureBorderHack ) {
		ofEnableTextureEdgeHack();
	} */
	
}

//--------------------------------------------------------------

void ofxCubeMapOld::loadFromOfImages(  ofImage pos_x, ofImage neg_x,
								  	ofImage pos_y, ofImage neg_y,
								  	ofImage pos_z, ofImage neg_z )
{	
	
	//_ofEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
	
	//create a texture object
	glGenTextures(1, &textureObjectID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjectID);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
#ifndef TARGET_OPENGLES	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // GL_TEXTURE_WRAP_R is not in the ES2 header, hmm..
#endif
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	
	
	unsigned char * data_px, * data_nx, * data_py, * data_ny, * data_pz, * data_nz;
	
	size = pos_x.getWidth();
	
	//cout << "ofxCubeMapOld::loadFromOfImages, size: " << size << "  bpp: " << pos_x.bpp << endl;
	
//	data_px = new unsigned char [size * size * 3];
//	data_py = new unsigned char [size * size * 3];
//	data_pz = new unsigned char [size * size * 3];
	
//	data_nx = new unsigned char [size * size * 3];
//	data_ny = new unsigned char [size * size * 3];
//	data_nz = new unsigned char [size * size * 3];
	
	data_px = pos_x.getPixels().getData();
	data_py = pos_y.getPixels().getData();
	data_pz = pos_z.getPixels().getData();    

	data_nx = neg_x.getPixels().getData();
	data_ny = neg_y.getPixels().getData();    
	data_nz = neg_z.getPixels().getData();
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_px); // positive x
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_py); // positive y
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pz); // positive z	
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_nx); // negative x
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_ny); // negative y
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_nz); // negative z	
}


//--------------------------------------------------------------
void ofxCubeMapOld::initEmptyTextures( int _size, GLuint _channels, GLuint _storageFormat )
{
	size = _size;
	
	//create a texture object
	glGenTextures(1, &textureObjectID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjectID);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
#ifndef TARGET_OPENGLES
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // GL_TEXTURE_WRAP_R is not in the ES2 header, hmm..
#endif
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	// set textures
	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _channels, size, size, 0, GL_RGB, _storageFormat, 0);
	}
		
	ofFbo::Settings fboSettings = ofFbo::Settings();
	
	fboSettings.width  = size;
	fboSettings.height = size;

	fboSettings.numColorbuffers = 6; // we intend to attach our own colour buffers
	
	fboSettings.useDepth = true;
	
	fboSettings.textureTarget = GL_TEXTURE_2D;
	
	fbo.allocate( fboSettings );
}


//--------------------------------------------------------------
void ofxCubeMapOld::beginDrawingInto2D( GLuint _face )
{
	fbo.begin();
	
	// Bind the face we wanted to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _face, textureObjectID, 0 );
	
}

//--------------------------------------------------------------
void ofxCubeMapOld::endDrawingInto2D()
{
	fbo.end();
}


//--------------------------------------------------------------
void ofxCubeMapOld::beginDrawingInto3D( GLuint _face )
{
	ofPushView();

	beginDrawingInto2D( _face );	
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	glLoadMatrixf( getProjectionMatrix().getPtr() );
		
	glMatrixMode( GL_MODELVIEW );
	ofPushMatrix();
	glLoadMatrixf( getLookAtMatrixForFace( _face ).getPtr() );
	
}

//--------------------------------------------------------------
void ofxCubeMapOld::endDrawingInto3D()
{
	ofPopView();

	ofPopMatrix();
	
	fbo.end();
}

//--------------------------------------------------------------
void ofxCubeMapOld::bind()
{
	bindToTextureUnit( 0 );
}


//--------------------------------------------------------------
void ofxCubeMapOld::bindToTextureUnit( int pos )
{
	boundToTextureUnit = pos;
	
	glActiveTexture( GL_TEXTURE0 + pos );
	glEnable( GL_TEXTURE_CUBE_MAP );
	glBindTexture( GL_TEXTURE_CUBE_MAP, textureObjectID );
}

//--------------------------------------------------------------
void ofxCubeMapOld::unbind()
{
	glActiveTexture( GL_TEXTURE0 + boundToTextureUnit );

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0 );
	glDisable( GL_TEXTURE_CUBE_MAP );
	
	glActiveTexture( GL_TEXTURE0 );
}

//--------------------------------------------------------------
void ofxCubeMapOld::drawSkybox( float _size )
{
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer( 	3, GL_FLOAT, sizeof(ofVec3f), &cubemapVertices.data()->x );
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer( 	3, GL_FLOAT, sizeof(ofVec3f), &cubemapTexCoords.data()->x );
	
	ofPushMatrix();
		ofScale( _size, _size, _size );
		glDrawArrays(GL_TRIANGLES, 0, cubemapVertices.size() );
	glPopMatrix();
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//--------------------------------------------------------------
void ofxCubeMapOld::debugDrawCubemapCameras()
{
	for( int i = 0; i < 6; i++ )
	{
		GLuint face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		ofMatrix4x4 modelViewProjectionMatrix = getLookAtMatrixForFace( face ) * getProjectionMatrix();
		
		ofMatrix4x4 inverseCameraMatrix;
		inverseCameraMatrix.makeInvertOf( modelViewProjectionMatrix );
		
		ofPushMatrix();
			
			glMultMatrixf( inverseCameraMatrix.getPtr() );
	
			ofNoFill();
		
				// Draw box in camera space, i.e. frustum in world space, box -1, -1, -1 to +1, +1, +1
				ofBox(0, 0, 0, 2.0f);
			
			ofFill();
		
		ofPopMatrix();
		
	}
}

//--------------------------------------------------------------
void ofxCubeMapOld::debugDrawCubemapFaces( float _faceSize, float _border )
{
	for( int i = 0; i < 6; i++ )
	{
		int tmpX = (i * _faceSize) + (i * _border);
		int tmpY = 0;
		drawFace( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i , tmpX, tmpY, _faceSize, _faceSize );
	}
}

//--------------------------------------------------------------
unsigned int ofxCubeMapOld::getTextureID()
{
	return textureObjectID;
}

//--------------------------------------------------------------
int ofxCubeMapOld::getWidth()
{
	return size;
}

//--------------------------------------------------------------
int ofxCubeMapOld::getHeight()
{
	return size;
}

//--------------------------------------------------------------
float ofxCubeMapOld::getFov()
{
	return fov;
}

//--------------------------------------------------------------
void ofxCubeMapOld::setFov( float _fov )
{
	fov = _fov;
}

//--------------------------------------------------------------
ofVec2f ofxCubeMapOld::getNearFar()
{
	return ofVec2f( nearZ, farZ );
}

//--------------------------------------------------------------
void ofxCubeMapOld::setNearFar( ofVec2f _nearFar )
{
	nearZ = _nearFar.x;
	farZ  = _nearFar.y;
}

//--------------------------------------------------------------
void ofxCubeMapOld::setPosition( ofVec3f& _pos )
{
	cubeMapCamerasRenderPosition.set( _pos.x, _pos.y, _pos.z );
}

//--------------------------------------------------------------
void ofxCubeMapOld::setPosition( float _x, float _y, float _z )
{
	cubeMapCamerasRenderPosition.set( _x, _y, _z );
}

//--------------------------------------------------------------
ofVec3f* ofxCubeMapOld::getPosition()
{
	return &cubeMapCamerasRenderPosition;
}

//--------------------------------------------------------------
ofMatrix4x4 ofxCubeMapOld::getProjectionMatrix()
{
	ofMatrix4x4 perspectiveMatrix;
	perspectiveMatrix.makePerspectiveMatrix(fov, size/(float)size, nearZ, farZ );
	
	return perspectiveMatrix;
}

//--------------------------------------------------------------
ofMatrix4x4 ofxCubeMapOld::getLookAtMatrixForFace( GLuint _face )
{
	ofMatrix4x4 lookAt;
	
	switch ( _face )
	{
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			lookAt.makeLookAtViewMatrix( ofVec3f( 0.0f, 0.0f, 0.0f), ofVec3f(  1.0f,  0.0f,  0.0f), ofVec3f(  0.0f, -1.0f,  0.0f) );
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			lookAt.makeLookAtViewMatrix( ofVec3f( 0.0f, 0.0f, 0.0f), ofVec3f( -1.0f,  0.0f,  0.0f), ofVec3f(  0.0f, -1.0f,  0.0f) );
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			lookAt.makeLookAtViewMatrix( ofVec3f( 0.0f, 0.0f, 0.0f), ofVec3f(  0.0f,  1.0f,  0.0f), ofVec3f(  0.0f,  0.0f,  1.0f) );
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			lookAt.makeLookAtViewMatrix( ofVec3f( 0.0f, 0.0f, 0.0f), ofVec3f(  0.0f, -1.0f,  0.0f), ofVec3f(  0.0f,  0.0f, -1.0f) );
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			lookAt.makeLookAtViewMatrix( ofVec3f( 0.0f, 0.0f, 0.0f), ofVec3f(  0.0f,  0.0f,  1.0f), ofVec3f(  0.0f, -1.0f,  0.0f) );
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			lookAt.makeLookAtViewMatrix( ofVec3f( 0.0f, 0.0f, 0.0f), ofVec3f(  0.0f,  0.0f, -1.0f), ofVec3f(  0.0f, -1.0f,  0.0f) );
			break;
		default:
			ofLogError() << "ofxCubeMapOld::getLookAtMatrixForFace, passed in invalid face.";
			break;
    }
	
	//lookAt.translate( cubeMapCamerasRenderPosition.x, cubeMapCamerasRenderPosition.y, cubeMapCamerasRenderPosition.z );
	lookAt.glTranslate( -cubeMapCamerasRenderPosition.x, -cubeMapCamerasRenderPosition.y, -cubeMapCamerasRenderPosition.z );
	
	return lookAt;
}


//--------------------------------------------------------------
void ofxCubeMapOld::drawFace( GLuint _face, float _x, float _y )
{
	drawFace( _face, _x, _y, size, size );
}

//--------------------------------------------------------------
//
//  Used to draw the faces to screen in 2D, usually to debug.
//	The code would look something like:
//
//  for( int i = 0; i < 6; i++ )
//  {
// 	   myFboCubeMap.drawFace( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i , i * 100, 0, 100, 100 );
//  }
//
void ofxCubeMapOld::drawFace( GLuint _face, float _x, float _y, float _w, float _h )
{
	// create a rect with the correct 3D texture coordinates, draw to screen
	scratchVertices.clear();
	scratchTexCoords.clear();
	scratchIndices.clear();
	
	switch ( _face )
	{
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			
			scratchTexCoords.push_back( ofVec3f( 1.0f, -1.0f,  1.0f) );
			scratchTexCoords.push_back( ofVec3f( 1.0f,  1.0f,  1.0f) );
			scratchTexCoords.push_back( ofVec3f( 1.0f,  1.0f, -1.0f) );
			scratchTexCoords.push_back( ofVec3f( 1.0f, -1.0f, -1.0f) );
			
			break;
			
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			
			scratchTexCoords.push_back( ofVec3f( -1.0f, -1.0f, -1.0f) );
			scratchTexCoords.push_back( ofVec3f( -1.0f,  1.0f, -1.0f) );
			scratchTexCoords.push_back( ofVec3f( -1.0f,  1.0f,  1.0f) );
			scratchTexCoords.push_back( ofVec3f( -1.0f, -1.0f,  1.0f) );
			
			break;
			
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			
			scratchTexCoords.push_back( ofVec3f( -1.0f,  1.0f,  1.0f) );
			scratchTexCoords.push_back( ofVec3f( -1.0f,  1.0f, -1.0f) );
			scratchTexCoords.push_back( ofVec3f(  1.0f,  1.0f, -1.0f) );
			scratchTexCoords.push_back( ofVec3f(  1.0f,  1.0f,  1.0f) );
			
			break;
			
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			
			scratchTexCoords.push_back( ofVec3f( -1.0f, -1.0f, -1.0f) );
			scratchTexCoords.push_back( ofVec3f( -1.0f, -1.0f,  1.0f) );
			scratchTexCoords.push_back( ofVec3f(  1.0f, -1.0f,  1.0f) );
			scratchTexCoords.push_back( ofVec3f(  1.0f, -1.0f, -1.0f) );
			
			break;
			
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			
			scratchTexCoords.push_back( ofVec3f( -1.0f, -1.0f,  1.0f) );
			scratchTexCoords.push_back( ofVec3f( -1.0f,  1.0f,  1.0f) );
			scratchTexCoords.push_back( ofVec3f(  1.0f,  1.0f,  1.0f) );
			scratchTexCoords.push_back( ofVec3f(  1.0f, -1.0f,  1.0f) );
			
			break;
			
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			
			scratchTexCoords.push_back( ofVec3f(  1.0f, -1.0f, -1.0f) );
			scratchTexCoords.push_back( ofVec3f(  1.0f,  1.0f, -1.0f) );
			scratchTexCoords.push_back( ofVec3f( -1.0f,  1.0f, -1.0f) );
			scratchTexCoords.push_back( ofVec3f( -1.0f, -1.0f, -1.0f) );
			
			break;
			
		default:
			
			ofLogError() << "ofxCubeMapOld::drawFace, passed in invalid face.";
			
			break;
	}
	
	scratchVertices.push_back( ofVec3f( _x, 		_y + _h, 	0.0f ) );
	scratchVertices.push_back( ofVec3f( _x, 		_y, 		0.0f ) );
	scratchVertices.push_back( ofVec3f( _x + _w, 	_y, 		0.0f ) );
	scratchVertices.push_back( ofVec3f( _x + _w, 	_y + _h, 	0.0f ) );
	
	scratchIndices.push_back( 0 );
	scratchIndices.push_back( 1 );
	scratchIndices.push_back( 2 );
	
	scratchIndices.push_back( 0 );
	scratchIndices.push_back( 2 );
	scratchIndices.push_back( 3 );
	
	// swap all this for an ofMesh when it supports ofVec3f tex coordinates
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer( 	3, GL_FLOAT, sizeof(ofVec3f), &scratchVertices.data()->x );
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer( 	3, GL_FLOAT, sizeof(ofVec3f), &scratchTexCoords.data()->x );
	
	bind();
	
#ifdef TARGET_OPENGLES
	glDrawElements( GL_TRIANGLES, scratchIndices.size(), GL_UNSIGNED_SHORT, 	scratchIndices.data() );
#else
	glDrawElements( GL_TRIANGLES, scratchIndices.size(), GL_UNSIGNED_INT, 		scratchIndices.data() );
#endif
	
	unbind();
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}

//--------------------------------------------------------------
string ofxCubeMapOld::getDescriptiveStringForFace( GLuint _face )
{
	string tmpName = "";
	switch ( _face )
	{
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			tmpName = "Pos X";
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			tmpName = "Neg X";
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			tmpName = "Pos Y";
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			tmpName = "Neg Y";
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			tmpName = "Pos Z";
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			tmpName = "Neg Z";
			break;
		default:
			ofLogError() << "ofxCubeMapOld::descriptiveStringForFace, passed in invalid face.";
			break;
    }
	
	return tmpName;
	
}

//--------------------------------------------------------------
void ofxCubeMapOld::setupSkyBoxVertices()
{
	
	//ofScale( _size, _size, _size );
	//ofScale( _size / 2.0f, _size / 2.0f, _size / 2.0f );
	
	float fExtent = 1.0f / 2.0f;
	
	
	
	///////////////////////////////////////////////
	//  Postive X
	cubemapTexCoords.push_back( ofVec3f(1.0f, -1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, -fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, -1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, -fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, 1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f(1.0f, -1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, -fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, 1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, 1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, fExtent, fExtent) );
	
	
	//////////////////////////////////////////////
	// Negative X
	cubemapTexCoords.push_back( ofVec3f( -1.0f, -1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent , -fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, -1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, -fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, 1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, -1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent , -fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, 1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, 1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, fExtent, -fExtent) );
		
	//////////////////////////////////////////////////
	// Positive Y
	cubemapTexCoords.push_back( ofVec3f( -1.0f, 1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, 1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, 1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, 1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, 1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, 1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, fExtent, -fExtent) );
	
	///////////////////////////////////////////////////
	// Negative Y
	cubemapTexCoords.push_back( ofVec3f( -1.0f, -1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, -fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, -1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, -fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, -1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, -fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, -1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, -fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, -1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, -fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, -1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, -fExtent, fExtent) );
	
	
	////////////////////////////////////////////////
	// Positive Z
	cubemapTexCoords.push_back( ofVec3f( 1.0f, -1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, -fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, -1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, -fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, 1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, -1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, -fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, 1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, fExtent, fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, 1.0f, 1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, fExtent, fExtent) );
	
	
	////////////////////////////////////////////////
	// Negative Z
	cubemapTexCoords.push_back( ofVec3f( -1.0f, -1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, -fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, -1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, -fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, 1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, -1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, -fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( -1.0f, 1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(-fExtent, fExtent, -fExtent) );
	
	cubemapTexCoords.push_back( ofVec3f( 1.0f, 1.0f, -1.0f) );
	cubemapVertices.push_back( ofVec3f(fExtent, fExtent, -fExtent) );
	
}
