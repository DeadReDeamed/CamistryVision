#pragma once

#include "assimp/Importer.hpp"    //OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"
// include DevIL for image loading
#include <IL/il.h>

#include <math.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <thread>         // std::thread

#include "glm/glm.hpp"

using namespace std;
namespace shit
{
	// assimp include files. These three are usually needed.

	struct MyModel {
		aiScene* scene; // the global Assimp scene object for each model
		std::vector<cv::Mat> viewMatrix = { cv::Mat::zeros(4, 4, CV_32F),cv::Mat::zeros(4, 4, CV_32F),cv::Mat::zeros(4, 4, CV_32F) };
		int seenlast = 0;
		bool seennow = false;
		Assimp::Importer importer; // Create an instance of the Importer class
		std::vector<struct MyMesh> myMeshes;
		float scaleFactor; // scale factor for the model to fit in the window
		int marker; //marker corresponding to this model

	};

	std::map<int, std::string> modelMap;
	std::map<int, MyModel> models;
	std::map<std::string, GLuint> textureIdMap;

	// For push and pop matrix
	std::vector<float*> matrixStack;

	GLuint program, vertexShader, fragmentShader;
	GLuint p, vertexShader2D, fragmentShader2D;

	// Information to render each assimp node
	struct MyMesh {
		GLuint vao;
		GLuint texIndex;
		GLuint uniformBlockIndex;
		int numFaces;
	};

	// This is for a shader uniform block
	struct MyMaterial {

		float diffuse[4];
		float ambient[4];
		float specular[4];
		float emissive[4];
		float shininess;
		int texCount;
	};

	void set_float4(float f[4], float a, float b, float c, float d)
	{
		f[0] = a;
		f[1] = b;
		f[2] = c;
		f[3] = d;
	}

	void color4_to_float4(const aiColor4D* c, float f[4])
	{
		f[0] = c->r;
		f[1] = c->g;
		f[2] = c->b;
		f[3] = c->a;
	}
	GLuint vertexLoc = 0, normalLoc = 1, texCoordLoc = 2;
	void genVAOsAndUniformBuffer(aiScene* sc, std::vector<struct MyMesh>& myMeshes) {

		struct MyMesh aMesh;
		struct MyMaterial aMat;
		GLuint buffer;

		// For each mesh
		for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
		{
			const aiMesh* mesh = sc->mMeshes[n];

			// create array with faces
			// have to convert from Assimp format to array
			unsigned int* faceArray;
			faceArray = (unsigned int*)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
			unsigned int faceIndex = 0;

			for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
				const aiFace* face = &mesh->mFaces[t];

				memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
				faceIndex += 3;
			}
			aMesh.numFaces = sc->mMeshes[n]->mNumFaces;

			// generate Vertex Array for mesh
			glGenVertexArrays(1, &(aMesh.vao));
			glBindVertexArray(aMesh.vao);

			// buffer for faces
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

			// buffer for vertex positions
			if (mesh->HasPositions()) {
				glGenBuffers(1, &buffer);
				glBindBuffer(GL_ARRAY_BUFFER, buffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(vertexLoc);
				glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
			}

			// buffer for vertex normals
			if (mesh->HasNormals()) {
				glGenBuffers(1, &buffer);
				glBindBuffer(GL_ARRAY_BUFFER, buffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
				glEnableVertexAttribArray(normalLoc);
				glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);
			}

			// buffer for vertex texture coordinates
			if (mesh->HasTextureCoords(0)) {
				float* texCoords = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);
				for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

					texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
					texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;

				}
				glGenBuffers(1, &buffer);
				glBindBuffer(GL_ARRAY_BUFFER, buffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
				glEnableVertexAttribArray(texCoordLoc);
				glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
			}

			// unbind buffers
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// create material uniform buffer
			aiMaterial* mtl = sc->mMaterials[mesh->mMaterialIndex];

			aiString texPath;    //contains filename of texture
			if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)) {
				//bind texture
				unsigned int texId = textureIdMap[texPath.data];
				aMesh.texIndex = texId;
				aMat.texCount = 1;
			}
			else
				aMat.texCount = 0;

			float c[4];
			set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
			aiColor4D diffuse;
			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
				color4_to_float4(&diffuse, c);
			memcpy(aMat.diffuse, c, sizeof(c));

			set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
			aiColor4D ambient;
			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
				color4_to_float4(&ambient, c);
			memcpy(aMat.ambient, c, sizeof(c));

			set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
			aiColor4D specular;
			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
				color4_to_float4(&specular, c);
			memcpy(aMat.specular, c, sizeof(c));

			set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
			aiColor4D emission;
			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
				color4_to_float4(&emission, c);
			memcpy(aMat.emissive, c, sizeof(c));

			float shininess = 0.0;
			unsigned int max;
			aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
			aMat.shininess = shininess;

			glGenBuffers(1, &(aMesh.uniformBlockIndex));
			glBindBuffer(GL_UNIFORM_BUFFER, aMesh.uniformBlockIndex);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(aMat), (void*)(&aMat), GL_STATIC_DRAW);

			myMeshes.push_back(aMesh);
		}
	}

	bool loadModelFile(std::string filename)
	{
		filename = filename;
		//open file stream to the file
		std::ifstream infile(filename);

		//if opened successfully, read in the data
		if (infile.is_open())
		{
			int marker;
			std::string file;

			while (infile >> file >> marker)
			{
				modelMap[marker] = file;
			}
		}
		else
		{
			//if here, file was not opened correctly, notify user
			printf("Error opening file,%s exiting", filename.c_str());
			exit(0);
		}

		return true;
	}

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)
	void get_bounding_box_for_node(const aiNode* nd,
		aiVector3D* min,
		aiVector3D* max,
		aiScene* scene)

	{
		aiMatrix4x4 prev;
		unsigned int n = 0, t;

		for (; n < nd->mNumMeshes; ++n) {
			const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
			for (t = 0; t < mesh->mNumVertices; ++t) {

				aiVector3D tmp = mesh->mVertices[t];

				min->x = aisgl_min(min->x, tmp.x);
				min->y = aisgl_min(min->y, tmp.y);
				min->z = aisgl_min(min->z, tmp.z);

				max->x = aisgl_max(max->x, tmp.x);
				max->y = aisgl_max(max->y, tmp.y);
				max->z = aisgl_max(max->z, tmp.z);
			}
		}

		for (n = 0; n < nd->mNumChildren; ++n) {
			get_bounding_box_for_node(nd->mChildren[n], min, max, scene);
		}
	}

	void get_bounding_box(aiVector3D* min, aiVector3D* max, aiScene* scene)
	{
		min->x = min->y = min->z = 1e10f;
		max->x = max->y = max->z = -1e10f;
		get_bounding_box_for_node(scene->mRootNode, min, max, scene);
	}

	bool Import3DFromFile(const std::string& pFile, aiScene*& scene, Assimp::Importer& importer, float& scaleFactor)
	{
		std::string fileDir = pFile;
		//check if file exists
		std::ifstream fin(fileDir.c_str());
		if (!fin.fail()) {
			fin.close();
		}
		else {
			printf("Couldn't open file: %s\n", fileDir.c_str());
			printf("%s\n", importer.GetErrorString());
			return false;
		}

		scene = const_cast<aiScene*>(importer.ReadFile(fileDir, aiProcessPreset_TargetRealtime_Quality));


		// If the import failed, report it
		if (!scene)
		{
			printf("%s\n", importer.GetErrorString());
			return false;
		}

		// Now we can access the file's contents.
		printf("Import of scene %s succeeded.\n", fileDir.c_str());

		aiVector3D scene_min, scene_max, scene_center;
		get_bounding_box(&scene_min, &scene_max, scene);
		float tmp;
		tmp = scene_max.x - scene_min.x;
		tmp = scene_max.y - scene_min.y > tmp ? scene_max.y - scene_min.y : tmp;
		tmp = scene_max.z - scene_min.z > tmp ? scene_max.z - scene_min.z : tmp;
		scaleFactor = 1.f / tmp;

		// We're done. Everything will be cleaned up by the importer destructor
		return true;
	}

	int LoadGLTextures(aiScene* scene)
	{
		ILboolean success;

		/* initialization of DevIL */
		ilInit();

		/* scan scene's materials for textures */
		for (unsigned int m = 0; m < scene->mNumMaterials; ++m)
		{
			int texIndex = 0;
			aiString path;    // filename

			aiReturn texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			while (texFound == AI_SUCCESS) {
				//fill map with textures, OpenGL image ids set to 0
				textureIdMap[path.data] = 0;
				// more textures?
				texIndex++;
				texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			}
		}

		int numTextures = textureIdMap.size();

		/* create and fill array with DevIL texture ids */
		ILuint* imageIds = new ILuint[numTextures];
		ilGenImages(numTextures, imageIds);

		/* create and fill array with GL texture ids */
		GLuint* textureIds = new GLuint[numTextures];
		glGenTextures(numTextures, textureIds); /* Texture name generation */

												/* get iterator */
		std::map<std::string, GLuint>::iterator itr = textureIdMap.begin();
		int i = 0;
		for (; itr != textureIdMap.end(); ++i, ++itr)
		{
			//save IL image ID
			std::string filename = (*itr).first;  // get filename
			std::replace(filename.begin(), filename.end(), '\\', '/'); //Replace backslash with forward slash so linux can find the files
			filename = filename;
			(*itr).second = textureIds[i];      // save texture id for filename in map

			ilBindImage(imageIds[i]); /* Binding of DevIL image name */
			ilEnable(IL_ORIGIN_SET);
			ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
			success = ilLoadImage((ILstring)filename.c_str());

			if (success) {
				/* Convert image to RGBA */
				ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

				/* Create and load textures to OpenGL */
				glBindTexture(GL_TEXTURE_2D, textureIds[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
					ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					ilGetData());
			}
			else
				printf("Couldn't load Image: %s\n", filename.c_str());
		}
		/* Because we have already copied image data into texture data
		we can release memory used by image. */
		ilDeleteImages(numTextures, imageIds);

		//Cleanup
		delete[] imageIds;
		delete[] textureIds;

		//return success;
		return true;
	}

	GLuint matricesUniBuffer;
	int loadModels() {


		map<int, string>::iterator it;

		for (it = modelMap.begin(); it != modelMap.end(); it++)
		{
			int markerNum = it->first;
			string modelName = it->second;

			models[markerNum].marker = markerNum;

			if (!Import3DFromFile(modelName, models[markerNum].scene, models[markerNum].importer, models[markerNum].scaleFactor))
				return(-1);

			LoadGLTextures(models[markerNum].scene);

			genVAOsAndUniformBuffer(models[markerNum].scene, models[markerNum].myMeshes);

		}

		//
	// Uniform Block
	//
		glGenBuffers(1, &matricesUniBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 16 * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, matricesUniBuffer, 0, 16 * 3 * sizeof(float));    //setUniforms();
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		return 0;
	}

	void setCamera(cv::Mat viewMatrix) {

		//Set these to make the view matrix happy

		viewMatrix.at<float>(0, 3) = 0;
		viewMatrix.at<float>(1, 3) = 0;
		viewMatrix.at<float>(2, 3) = 0;
		viewMatrix.at<float>(3, 3) = 1;

		glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 16, sizeof(float) * 16, (float*)viewMatrix.data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}


	// sets the square matrix mat to the identity matrix,
	// size refers to the number of rows (or columns)
	void setIdentityMatrix(float* mat, int size) {

		// fill matrix with 0s
		for (int i = 0; i < size * size; ++i)
			mat[i] = 0.0f;

		// fill diagonal with 1s
		for (int i = 0; i < size; ++i)
			mat[i + i * size] = 1.0f;
	}

	// ----------------------------------------------------
// Model Matrix 
//
// Copies the modelMatrix to the uniform buffer

	float modelMatrix[16];

	void setModelMatrix() {

		glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER,
			sizeof(float) * 16 * 2, sizeof(float) * 16, modelMatrix);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}

	void multMatrix(float* a, float* b) {

		float res[16];

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				res[j * 4 + i] = 0.0f;
				for (int k = 0; k < 4; ++k) {
					res[j * 4 + i] += a[k * 4 + i] * b[j * 4 + k];
				}
			}
		}
		memcpy(a, res, 16 * sizeof(float));

	}


	// Defines a transformation matrix mat with a translation
	void setTranslationMatrix(float* mat, float x, float y, float z) {

		setIdentityMatrix(mat, 4);
		mat[12] = x;
		mat[13] = y;
		mat[14] = z;
	}

	// The equivalent to glTranslate applied to the model matrix
	void translate(float x, float y, float z) {

		float aux[16];

		setTranslationMatrix(aux, x, y, z);
		multMatrix(modelMatrix, aux);
		setModelMatrix();
	}

	void setRotationMatrix(float* mat, float angle, float x, float y, float z) {

		float radAngle = glm::radians(angle);
		float co = cos(radAngle);
		float si = sin(radAngle);
		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;

		mat[0] = x2 + (y2 + z2) * co;
		mat[4] = x * y * (1 - co) - z * si;
		mat[8] = x * z * (1 - co) + y * si;
		mat[12] = 0.0f;

		mat[1] = x * y * (1 - co) + z * si;
		mat[5] = y2 + (x2 + z2) * co;
		mat[9] = y * z * (1 - co) - x * si;
		mat[13] = 0.0f;

		mat[2] = x * z * (1 - co) - y * si;
		mat[6] = y * z * (1 - co) + x * si;
		mat[10] = z2 + (x2 + y2) * co;
		mat[14] = 0.0f;

		mat[3] = 0.0f;
		mat[7] = 0.0f;
		mat[11] = 0.0f;
		mat[15] = 1.0f;

	}// Defines a transformation matrix mat with a scale
	void setScaleMatrix(float* mat, float sx, float sy, float sz) {

		setIdentityMatrix(mat, 4);
		mat[0] = sx;
		mat[5] = sy;
		mat[10] = sz;
	}

	// The equivalent to glRotate applied to the model matrix
	void rotate(float angle, float x, float y, float z) {

		float aux[16];

		setRotationMatrix(aux, angle, x, y, z);
		multMatrix(modelMatrix, aux);
		setModelMatrix();
	}

	// The equivalent to glScale applied to the model matrix
	void scale(float x, float y, float z) {

		float aux[16];

		setScaleMatrix(aux, x, y, z);
		multMatrix(modelMatrix, aux);
		setModelMatrix();
	}

	char* textFileRead(char* fn) {


		FILE* fp;
		char* content = NULL;

		int count = 0;

		if (fn != NULL) {
			fopen_s(&fp, fn, "rt");

			if (fp != NULL) {

				fseek(fp, 0, SEEK_END);
				count = ftell(fp);
				rewind(fp);

				if (count > 0) {
					content = (char*)malloc(sizeof(char) * (count + 1));
					count = fread(content, sizeof(char), count, fp);
					content[count] = '\0';
				}
				fclose(fp);
			}
		}
		return content;
	}

	int textFileWrite(char* fn, char* s) {

		FILE* fp;
		int status = 0;

		if (fn != NULL) {
			fopen_s(&fp, fn, "w");

			if (fp != NULL) {

				if (fwrite(s, sizeof(char), strlen(s), fp) == strlen(s))
					status = 1;
				fclose(fp);
			}
		}
		return(status);
	}
	GLuint texUnit = 0;
	GLuint setupShaders() {

		char* vs = NULL, * fs = NULL;

		GLuint p, v, f;

		v = glCreateShader(GL_VERTEX_SHADER);
		f = glCreateShader(GL_FRAGMENT_SHADER);

		vs = textFileRead((char*)"del\\dirlightdiffambpix.vert");
		fs = textFileRead((char*)"del\\dirlightdiffambpix.frag");

		const char* vv = vs;
		const char* ff = fs;

		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);

		free(vs); free(fs);

		glCompileShader(v);
		glCompileShader(f);

		p = glCreateProgram();
		glAttachShader(p, v);
		glAttachShader(p, f);

		glBindFragDataLocation(p, 0, "output");

		glBindAttribLocation(p, vertexLoc, "position");
		glBindAttribLocation(p, normalLoc, "normal");
		glBindAttribLocation(p, texCoordLoc, "texCoord");

		glLinkProgram(p);
		glValidateProgram(p);

		int Result;
		int InfoLogLength;

		glGetProgramiv(p, GL_LINK_STATUS, &Result);
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(p, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}

		program = p;
		vertexShader = v;
		fragmentShader = f;

		GLuint k = glGetUniformBlockIndex(p, "Matrices");
		glUniformBlockBinding(p, k, 1);
		glUniformBlockBinding(p, glGetUniformBlockIndex(p, "Material"), 1);

		texUnit = glGetUniformLocation(p, "texUnit");

		return(p);
	}

	void pushMatrix() {

		float* aux = (float*)malloc(sizeof(float) * 16);
		memcpy(aux, modelMatrix, sizeof(float) * 16);
		matrixStack.push_back(aux);
	}

	void popMatrix() {

		float* m = matrixStack[matrixStack.size() - 1];
		memcpy(modelMatrix, m, sizeof(float) * 16);
		matrixStack.pop_back();
		free(m);
	}

	GLuint materialUniLoc = 2;
	void recursive_render(aiScene* sc, const aiNode* nd, std::vector<struct MyMesh>& myMeshes)
	{

		// Get node transformation matrix
		aiMatrix4x4 m = nd->mTransformation;
		// OpenGL matrices are column major
		m.Transpose();

		// save model matrix and apply node transformation
		pushMatrix();

		float aux[16];
		memcpy(aux, &m, sizeof(float) * 16);
		multMatrix(modelMatrix, aux);
		setModelMatrix();


		// draw all meshes assigned to this node
		for (unsigned int n = 0; n < nd->mNumMeshes; ++n) {
			// bind material uniform
			glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc, myMeshes[nd->mMeshes[n]].uniformBlockIndex, 0, sizeof(struct MyMaterial));
			// bind texture
			glBindTexture(GL_TEXTURE_2D, myMeshes[nd->mMeshes[n]].texIndex);
			// bind VAO
			glBindVertexArray(myMeshes[nd->mMeshes[n]].vao);
			// draw
			glDrawElements(GL_TRIANGLES, myMeshes[nd->mMeshes[n]].numFaces * 3, GL_UNSIGNED_INT, 0);

		}

		// draw all children
		for (unsigned int n = 0; n < nd->mNumChildren; ++n) {
			recursive_render(sc, nd->mChildren[n], myMeshes);
		}
		popMatrix();
	}

	void loop()
	{
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageMat.cols, imageMat.rows, GL_RGB, GL_UNSIGNED_BYTE, imageMat.data);
		// clear the framebuffer (color and depth)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//DRAW 3D MODEL
		glClear(GL_DEPTH_BUFFER_BIT);

		// set camera matrix
		map<int, MyModel>::iterator it;

		for (it = models.begin(); it != models.end(); it++)
		{
			MyModel currentModel = it->second;


			setCamera(currentModel.viewMatrix[0]);

			// set the model matrix to the identity Matrix
			setIdentityMatrix(modelMatrix, 4);

			// sets the model matrix to a scale matrix so that the model fits in the window
			scale(currentModel.scaleFactor, currentModel.scaleFactor, currentModel.scaleFactor);

			// keep rotating the model
			rotate(90.0f, 1.0f, 0.0f, 0.0f);

			// use our shadershader
			glUseProgram(program);

			// we are only going to use texture unit 0
			// unfortunately samplers can't reside in uniform blocks
			// so we have set this uniform separately
			glUniform1i(texUnit, 0);


			//glLoadMatrixf((float*)viewMatrix.data);
			recursive_render(currentModel.scene, currentModel.scene->mRootNode, currentModel.myMeshes);
		}
	}

}

