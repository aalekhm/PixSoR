#include "3D/MeshObjLoader.h"
#include <cassert>
#include "Common/Defines.h"

MeshObjLoader::MeshObjLoader()
	: m_bObjectHasUV(false),
	m_bObjectHasNormals(false),
	m_bJustReadAFace(false),

	m_pFile(NULL)
{
	m_vVertices.clear();
	m_vTextureCoords.clear();
	m_vFaces.clear();
}

bool MeshObjLoader::loadObject(const char* sFileName)
{
	assert(sFileName);

	m_pFile = new RandomAccessFile();
	bool bCanOpen = m_pFile->openForRead(sFileName);

	if (bCanOpen) {
		// Now that we have a valid file and it's open, let's read in the info!
		return readObjFile();

		// Now that we have the file read in, let's compute the vertex normals for lighting
		//computeNormals();
	}
	else
		return false;

	SAFE_DELETE(m_pFile);
}

bool MeshObjLoader::readObjFile()
{
	std::string singleLine;

	while (!m_pFile->isEOF()) {
		singleLine = (char*)m_pFile->readLine();
		//singleLine.trim();

		char ch = singleLine[0];
		switch (ch) {

			// Check if we just read in a 'v' (Could be a vertice/normal/textureCoord)
		case 'v':
		{
			if (m_bJustReadAFace) {

				// Save the last object's info into our model structure
				fillInObjectInfo();
			}
			else {

				// Read the next character in the file to see if it's a vertice/normal/UVCoord
				ch = singleLine[1];

				SVector3 vNewVector;
				SVector2 vNewTexCoord;

				// If we get a space it must have been a vertex ("v")
				if (ch == ' ') {

					// Here we read in a vertice.  The format is "v x y z"
					sscanf_s(singleLine.c_str(), "v %f %f %f", &vNewVector.x, &vNewVector.y, &vNewVector.z);

					// Add a new vertice to our list
					m_vVertices.push_back(vNewVector);
				}
				else
					// If we get a 't' then it must be a texture coordinate ("vt")
					if (ch == 't') {

						// Here we read in a texture coordinate.  The format is "vt u v"
						sscanf_s(singleLine.c_str(), "vt %f %f", &vNewTexCoord.x, &vNewTexCoord.y);

						vNewTexCoord.y = 1.0f - vNewTexCoord.y;

						// Add a new vertice to our list
						m_vTextureCoords.push_back(vNewTexCoord);

						// Set the flag that tells us this object has texture coordinates.
						// Now we know that the face information will list the vertice AND UV index.
						// For example, ("f 1 3 2" verses "f 1/1 2/2 3/3")
						m_bObjectHasUV = true;
					}
					else
						// If we get a 'n' then it must be a normal ("vn")
						if (ch == 'n') {

							// Here we read in a texture coordinate.  The format is "vt u v"
							sscanf_s(singleLine.c_str(), "vn %f %f %f", &vNewVector.x, &vNewVector.y, &vNewVector.z);

							// Add a new vertice to our list
							m_vVertexNormals.push_back(vNewVector);

							// Set the flag that tells us this object has Vertex Normals.
							// Now we know that the face information will list the "vertice" AND/OR "UV index" AND "Normal index".
							// For example, ("f 1 3 2" verses "f 1/1/1 2/2/2 3/3/3")
							m_bObjectHasNormals = true;
						}
			}
		}
		break;

		// Check if we just read in a face header ('f')
		case 'f':
		{
			SFace newFace = { 0 };

			SFace newFace1 = { 0 };
			bool bHasNewFace = false;

			// A face is a polygon (a triangle in this case) that has information about it.
			// It has the 3D points that make up the polygon and may also have texture coordinates.
			// When reading in an .obj, objects don't have to have UV texture coordinates so we
			// need to read in the face information differently in that case.  If the object does have
			// UV coordinates, then the format will look like this:
			// "f vertexIndex1/coordIndex1 vertexIndex2/coordIndex2 vertexIndex3/coordIndex3"
			// otherwise the format will look like this:"f vertexIndex1 vertexIndex2 vertexIndex3"
			// The index values index into our vertice and texture coordinate arrays.  More explained in RenderScene().
			// *Note* Make sure if you cut this code out for your own use you minus 1 from the indices.
			// This is because arrays are zero based and the .obj indices start at 1.  Look at FillInObjectInfo().

			// Check if this object has texture coordinates before reading in the values
			if (m_bObjectHasUV) {

				if (m_bObjectHasNormals) {
					int dummyNormal[3];
					int dummyVertex[3] = { -1 };

					sscanf_s(singleLine.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &newFace.vertexIndex[0], &newFace.texCoordIndex[0], &newFace.vertexNormalIndex[0],
						&newFace.vertexIndex[1], &newFace.texCoordIndex[1], &newFace.vertexNormalIndex[1],
						&newFace.vertexIndex[2], &newFace.texCoordIndex[2], &newFace.vertexNormalIndex[2],
						&dummyVertex[0], &dummyVertex[1], &dummyVertex[2]);

					if (dummyVertex[0] > 0 || dummyVertex[1] > 0 || dummyVertex[2] > 0)
					{
						newFace1.vertexIndex[0] = newFace.vertexIndex[0];
						newFace1.texCoordIndex[0] = newFace.texCoordIndex[0];
						newFace1.vertexNormalIndex[0] = newFace.vertexNormalIndex[0];

						newFace1.vertexIndex[1] = newFace.vertexIndex[2];
						newFace1.texCoordIndex[1] = newFace.texCoordIndex[2];
						newFace1.vertexNormalIndex[1] = newFace.vertexNormalIndex[2];

						newFace1.vertexIndex[2] = dummyVertex[0];
						newFace1.texCoordIndex[2] = dummyVertex[1];
						newFace1.vertexNormalIndex[2] = dummyVertex[2];

						bHasNewFace = true;
					}
				}
				else {
					sscanf_s(singleLine.c_str(), "f %d/%d %d/%d %d/%d", &newFace.vertexIndex[0], &newFace.texCoordIndex[0],
						&newFace.vertexIndex[1], &newFace.texCoordIndex[1],
						&newFace.vertexIndex[2], &newFace.texCoordIndex[2]);
				}
			}
			// The object does NOT have texture coordinates
			else {
				// Here we read in just the object's vertex indices.
				// Here is the format: "f vertexIndex1 vertexIndex2 vertexIndex3"
				sscanf_s(singleLine.c_str(), "f %d %d %d", &newFace.vertexIndex[0], &newFace.vertexIndex[1], &newFace.vertexIndex[2]);
			}

			// Add the new face to our face list
			m_vFaces.push_back(newFace);
			if (bHasNewFace)
				m_vFaces.push_back(newFace1);

			// We set this flag to TRUE so we know that we just read in some face information.
			// Since face information is the last thing we read in for an object we will check
			// this flag when we read in a vertice.  If it's true, then we just finished reading
			// in an object and we need to save off the information before going to the next object.
			// Since there is no standard header for objects in a .obj file we do it this way.
			m_bJustReadAFace = true;
		}
		break;
		}
	}

	// Now that we are done reading in the file, we have need to save the last object read.
	fillInObjectInfo();

	return true;
}

void MeshObjLoader::fillInObjectInfo()
{
	S3DObject newObject;
	int textureOffset = 0, vertexOffset = 0;
	int i = 0;

	// If we get here then we just finished reading in an object
	// and need to increase the object count.
	m_Model.numOfObjects++;

	// Get a pointer to the current object so our code is easier to read
	S3DObject *pObject = &newObject;

	// Now that we have our list's full of information, we can get the size
	// of these lists by calling size() from our vectors.  That is one of the
	// wonderful things that the Standard Template Library offers us.  Now you
	// never need to write a link list or constantly call malloc()/new.

	// Here we get the number of faces, vertices and texture coordinates
	newObject.numOfFaces = m_vFaces.size();
	newObject.numOfVertices = m_vVertices.size();
	newObject.numOfTextureVertices = m_vTextureCoords.size();

	// If we read in any faces for this object (required)
	if (newObject.numOfFaces) {

		// Allocate enough memory to store all the faces in our object
		newObject.pFaces = new SFace[newObject.numOfFaces];
	}

	// If we read in any vertices for this object (required)
	if (newObject.numOfVertices) {

		// Allocate enough memory to store all the vertices in our object
		newObject.pVertices = new SVector3[newObject.numOfVertices];
		newObject.pNormals = new SVector3[newObject.numOfVertices];
	}

	// If we read in any texture coordinates for this object (optional)
	if (newObject.numOfTextureVertices) {

		newObject.pTexVertices = new SVector2[newObject.numOfTextureVertices];
		newObject.bHasTexture = true;
	}

	// Go through all of the faces in the object
	for (int i = 0; i < newObject.numOfFaces; i++) {

		// Copy the current face from the temporary list to our Model list
		newObject.pFaces[i] = m_vFaces[i];

		// Because of the problem with .obj files not being very object friendly,
		// if a new object is found in the file, the face and texture indices start
		// from the last index that was used in the last object.  That means that if
		// the last one was 8, it would then go to 9 for the next object.  We need to
		// bring that back down to 1, so we just create an offset that we subtract from
		// the vertex and UV indices.

		// Check if this is the first face of the object
		//if(i == 0) {

		//	// If the first index is NOT 1, then we must be past the first object
		//	if(newObject.pFaces[0].vertexIndex[0] != 1) {

		//		// To create the offset we take the current starting point and then minus 1.
		//		// Lets say the last object ended at 8.  Well we would then have 9 here.
		//		// We want to then subtract 8 from the 9 to get back to 1.
		//		vertexOffset = newObject.pFaces[0].vertexIndex[0] -1;

		//		// The same goes for texture coordinates, if we have them do the same
		//		if(newObject.numOfTextureVertices > 0) {

		//			// Take the current index and minus 1 from it
		//			textureOffset = newObject.pFaces[0].texCoordIndex[0] - 1;
		//		}
		//	}
		//}

		//// Because the face indices start at 1, we need to minus 1 from them due
		//// to arrays being zero based.  This is VERY important!
		for (int j = 0; j < 3; j++) {

			// For each index, minus 1 to conform with zero based arrays.
			// We also need to add the vertex and texture offsets to subtract
			// the total amount necessary for this to work.  The first object
			// will have a offset of 0 for both since it starts at 1.
			newObject.pFaces[i].vertexIndex[j] -= 1;// + vertexOffset;
			newObject.pFaces[i].texCoordIndex[j] -= 1;// + textureOffset;
		}
	}

	// Go through all the vertices in the object
	for (i = 0; i < newObject.numOfVertices; i++) {

		// Copy the current vertice from the temporary list to our Model list
		newObject.pVertices[i] = m_vVertices[i];
		//if (m_vVertexNormals.size() > 0)
		//	newObject.pNormals[i] = m_vVertexNormals[i];
	}

	// Go through all of the texture coordinates in the object (if any)
	for (i = 0; i < newObject.numOfTextureVertices; i++) {

		// Copy the current UV coordinate from the temporary list to our Model list
		newObject.pTexVertices[i] = m_vTextureCoords[i];
	}

	// Since .OBJ files don't have materials, we set the material ID to -1.
	// We need to manually give it a material using AddMaterial().
	newObject.materialID = -1;

	// Add a new object to the list of objects in our model
	m_Model.vObjects.push_back(newObject);

	// Now that we have all the information from out list's, we need to clear them
	// so we can be ready for the next object that we read in.
	//m_vVertices.clear();
	//m_vFaces.clear();
	//m_vTextureCoords.clear();

	// Reset these booleans to be prepared for the next object
	m_bObjectHasUV = false;
	m_bJustReadAFace = false;
}

///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function computes the normals and vertex normals of the objects
/////
///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
void MeshObjLoader::computeNormals()
{
	SVector3 vVector1, vVector2, vNormal, vPoly[3];

	// If there are no objects, we can skip this part
	if (m_Model.numOfObjects <= 0)
		return;

	// What are vertex normals?  And how are they different from other normals?
	// Well, if you find the normal to a triangle, you are finding a "Face Normal".
	// If you give OpenGL a face normal for lighting, it will make your object look
	// really flat and not very round.  If we find the normal for each vertex, it makes
	// the smooth lighting look.  This also covers up blocky looking objects and they appear
	// to have more polygons than they do.    Basically, what you do is first
	// calculate the face normals, then you take the average of all the normals around each
	// vertex.  It's just averaging.  That way you get a better approximation for that vertex.

	// Go through each of the objects to calculate their normals
	for (int index = 0; index < m_Model.numOfObjects; index++)
	{
		// Get the current object
		S3DObject *pObject = &(m_Model.vObjects[index]);

		// Here we allocate all the memory we need to calculate the normals
		SVector3 *pNormals = new SVector3[pObject->numOfFaces];
		SVector3 *pTempNormals = new SVector3[pObject->numOfFaces];
		pObject->pNormals = new SVector3[pObject->numOfVertices];

		// Go though all of the faces of this object
		for (int i = 0; i < pObject->numOfFaces; i++)
		{
			// To cut down LARGE code, we extract the 3 points of this face
			vPoly[0] = pObject->pVertices[pObject->pFaces[i].vertexIndex[0]];
			vPoly[1] = pObject->pVertices[pObject->pFaces[i].vertexIndex[1]];
			vPoly[2] = pObject->pVertices[pObject->pFaces[i].vertexIndex[2]];

			// Now let's calculate the face normals (Get 2 vectors and find the cross product of those 2)

			vVector1 = Vector(vPoly[0], vPoly[2]);		// Get the vector of the polygon (we just need 2 sides for the normal)
			vVector2 = Vector(vPoly[2], vPoly[1]);		// Get a second vector of the polygon

			vNormal = Cross(vVector1, vVector2);		// Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
			pTempNormals[i] = vNormal;					// Save the un-normalized normal for the vertex normals
			vNormal = Normalize(vNormal);				// Normalize the cross product to give us the polygons normal

			pNormals[i] = vNormal;						// Assign the normal to the list of normals
		}

		//////////////// Now Get The Vertex Normals /////////////////

		SVector3 vSum;
		SVector3 vZero = vSum;
		int shared = 0;

		for (int i = 0; i < pObject->numOfVertices; i++)			// Go through all of the vertices
		{
			for (int j = 0; j < pObject->numOfFaces; j++)	// Go through all of the triangles
			{												// Check if the vertex is shared by another face
				if (pObject->pFaces[j].vertexIndex[0] == i ||
					pObject->pFaces[j].vertexIndex[1] == i ||
					pObject->pFaces[j].vertexIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);// Add the un-normalized normal of the shared face
					shared++;								// Increase the number of shared triangles
				}
			}

			// Get the normal by dividing the sum by the shared.  We negate the shared so it has the normals pointing out.
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

			// Normalize the normal for the final vertex normal
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);

			vSum = vZero;									// Reset the sum
			shared = 0;										// Reset the shared
		}

		// Free our memory and start over on the next object
		delete[] pTempNormals;
		delete[] pNormals;
	}
}

int32_t MeshObjLoader::getNumFaces()
{
	return m_vFaces.size();
}

SFace* MeshObjLoader::getFace(int32_t i)
{
	SFace* pFace = nullptr;
	if (i >= 0 && i < m_vFaces.size())
	{
		pFace = &m_vFaces.at(i);
	}

	return pFace;
}

SVector3* MeshObjLoader::getVertex(int32_t i)
{
	SVector3* pSVector3 = nullptr;
	if (i >= 0 && i < m_vVertices.size())
	{
		pSVector3 = &m_vVertices.at(i);
	}

	return pSVector3;
}

SVector2* MeshObjLoader::getTexCoord(int32_t i)
{
	SVector2* pSVector2 = nullptr;
	if (i >= 0 && i < m_vTextureCoords.size())
	{
		pSVector2 = &m_vTextureCoords.at(i);
	}

	return pSVector2;
}

SVector3* MeshObjLoader::getNormal(int32_t i)
{
	SVector3* pSVector3 = nullptr;
	if (i >= 0 && i < m_vVertexNormals.size())
	{
		pSVector3 = &m_vVertexNormals.at(i);
	}

	return pSVector3;
}

MeshObjLoader::~MeshObjLoader() {
	SAFE_DELETE(m_pFile);
}