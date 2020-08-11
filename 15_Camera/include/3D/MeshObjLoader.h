#pragma once

#include "Common/RandomAccessFile.h"
#include <vector>

class Model;

//This is a 3 point class, used to store the vertices of our Model.
struct SVector3 {
public:
	SVector3() {
		x = y = z = 0;
	}

	float x, y, z;
};

//This is a 2 point class, used to store the UV texture coordinates.
struct SVector2 {
public:
	SVector2() {
		x = y = 0;
	}

	float x, y;
};

// This computes the magnitude of a normal.   (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// This calculates a vector between 2 points and returns the result
static SVector3 Vector(SVector3 vPoint1, SVector3 vPoint2) {
	SVector3 vVector;							// The variable to hold the resultant vector

	vVector.x = vPoint1.x - vPoint2.x;			// Subtract point1 and point2 x's
	vVector.y = vPoint1.y - vPoint2.y;			// Subtract point1 and point2 y's
	vVector.z = vPoint1.z - vPoint2.z;			// Subtract point1 and point2 z's

	return vVector;								// Return the resultant vector
}

// This adds 2 vectors together and returns the result
static SVector3 AddVector(SVector3 vVector1, SVector3 vVector2) {
	SVector3 vResult;							// The variable to hold the resultant vector

	vResult.x = vVector2.x + vVector1.x;		// Add Vector1 and Vector2 x's
	vResult.y = vVector2.y + vVector1.y;		// Add Vector1 and Vector2 y's
	vResult.z = vVector2.z + vVector1.z;		// Add Vector1 and Vector2 z's

	return vResult;								// Return the resultant vector
}

// This divides a vector by a single number (scalar) and returns the result
static SVector3 DivideVectorByScaler(SVector3 vVector1, float Scaler) {
	SVector3 vResult;							// The variable to hold the resultant vector

	vResult.x = vVector1.x / Scaler;			// Divide Vector1's x value by the scaler
	vResult.y = vVector1.y / Scaler;			// Divide Vector1's y value by the scaler
	vResult.z = vVector1.z / Scaler;			// Divide Vector1's z value by the scaler

	return vResult;								// Return the resultant vector
}

// This returns the cross product between 2 vectors
static SVector3 Cross(SVector3 vVector1, SVector3 vVector2) {
	SVector3 vCross;								// The vector to hold the cross product
	// Get the X value
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	// Get the Y value
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	// Get the Z value
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;								// Return the cross product
}

// This returns the normal of a vector
static SVector3 Normalize(SVector3 vNormal) {
	double Magnitude;							// This holds the magnitude			

	Magnitude = Mag(vNormal);					// Get the magnitude

	vNormal.x /= (float)Magnitude;				// Divide the vector's X by the magnitude
	vNormal.y /= (float)Magnitude;				// Divide the vector's Y by the magnitude
	vNormal.z /= (float)Magnitude;				// Divide the vector's Z by the magnitude

	return vNormal;								// Return the normal
}

struct SFace {
	int vertexIndex[3];
	int	texCoordIndex[3];
	int vertexNormalIndex[3];
};

//This holds the material information. It may be a texture map or a colour.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.
struct SMaterial {
	std::string		materialName;				//The Material Name
	std::string		textureFileName;			//The texture file name, if this is a texture map
	unsigned char	colour[3];					//The colour of the object (R, G, B)
	int				textureID;					//The texture ID
	float			uTile;						//U tiling of the texture
	float			vTile;						//V tiling of the texture
	float			uOffset;					//U tiling offset of the texture
	float			vOffset;					//V tiling offset of the texture
};

//This holds the information for a single OBJECT in our MODEL/SCENE.
//You should eventually turn into a robust class that 
//has loading/drawing/querying functions like:
//LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct S3DObject {
	S3DObject() {
		objectName = "";

		numOfVertices = 0;
		numOfFaces = 0;
		numOfTextureVertices = 0;

		bHasTexture = false;
		materialID = 0;

		pFaces = NULL;
		pVertices = NULL;
		pNormals = NULL;
		pTexVertices = NULL;
	}

	std::string	objectName;					//The name of the Object

	int			numOfVertices;				//The number of vertices in the MODEL
	int			numOfFaces;					//The number of faces in the MODEL
	int			numOfTextureVertices;		//The number of texture Coordinates

	bool		bHasTexture;				//Does this object has a texture?
	int			materialID;					//If yes, this is the texture ID to use, which is the index to out texture array

	SFace		*pFaces;					//The objects faces
	SVector3	*pVertices;					//The objects vertices
	SVector3	*pNormals;					//The objects normals
	SVector2	*pTexVertices;				//The objects texture`s UV coordinates
};

///This holds our MODEL/SCENE information.
//This should also turn into a robust class.
// We use STL's (Standard Template Library) vector class to ease our link list burdens. :)
struct S3DModel {
	S3DModel() {
		numOfObjects = 0;
		numOfMaterials = 0;

		vObjects.clear();
		vMaterials.clear();

		numOfTags = 0;
		pLinks = NULL;
	}

	int					numOfObjects;		//The number of Objects in our MODEL/SCENE
	int					numOfMaterials;		//The number of materials used by the Objects in our MODEL/SCENE

	std::vector<S3DObject>	vObjects;			//List of the Objects
	std::vector<SMaterial>	vMaterials;			//List of the Materials (textures & colours).	

	/*MD3 Specific*/
	int					numOfTags;			//
	S3DModel			**pLinks;
	//struct SMD3Tag		*pTags;
};

class MeshObjLoader 
{
	public:
								MeshObjLoader();
		virtual					~MeshObjLoader();

		bool					loadObject(const char* sFileName);
		int32_t					getNumFaces();

		SFace*					getFace(int32_t i);
		SVector3*				getVertex(int32_t i);
		SVector2*				getTexCoord(int32_t i);
		SVector3*				getNormal(int32_t i);

		int32_t					getNormalsSize() { return m_vVertexNormals.size(); };
	private:
		bool					readObjFile();
		void					fillInObjectInfo();
		void					computeNormals();

		S3DModel				m_Model;

		std::vector<SVector3>	m_vVertices;
		std::vector<SVector2>	m_vTextureCoords;
		std::vector<SVector3>	m_vVertexNormals;
		std::vector<SFace>		m_vFaces;

		bool					m_bObjectHasUV;
		bool					m_bObjectHasNormals;
		bool					m_bJustReadAFace;

		RandomAccessFile*		m_pFile;
};