#pragma once

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>          
#include <assimp/postprocess.h>     

#include <omp.h>
#include <string>

#include "Mesh.h"
class ModelLoader {
public:
	static bool loadAssimp(Mesh *mesh, std::string path) {
		Assimp::Importer importer;
		if (mesh->m_vertices.empty() == false) {
			return false; //if not empty return
		}
		std::cout << "\nLoading file : " << path << ".\n";
		//aiProcess_Triangulate !!!
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_FixInfacingNormals | aiProcess_GenUVCoords); //aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_CalcTangentSpace );
		if (!scene) { //| aiProcess_JoinIdenticalVertices
			fprintf(stderr, importer.GetErrorString());
			return false;
		}
		std::cout << "Number of meshes : " << scene->mNumMeshes << ".\n";

		//current mesh
		const aiMesh* aiMesh = scene->mMeshes[0];

		omp_set_num_threads(omp_get_max_threads());

		mesh->m_vertices.resize(aiMesh->mNumVertices, vec3(0.0f));
#pragma omp parallel for
		for (size_t i = 0; i < aiMesh->mNumVertices; i++) {
			aiVector3D pos = aiMesh->mVertices[i];
			//Can't use push_back parallely!
			//this->vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
			mesh->vertices[i] = glm::vec3(pos.x, pos.y, pos.z);
		}

		// Fill vertices texture coordinates
		mesh->textureCoordinates.resize(aiMesh->mNumVertices, glm::vec2(0.0f));
		if (aiMesh->HasTextureCoords(0)) {
#pragma omp parallel for
			for (size_t i = 0; i < aiMesh->mNumVertices; i++) {
				aiVector3D UVW = aiMesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
				mesh->textureCoordinates[i] = (glm::vec2(UVW.x, UVW.y));
			}
		}

		// Fill vertices normals
		mesh->normals.resize(aiMesh->mNumVertices, vec3(0.0f));
		if (aiMesh->HasNormals()) {
#pragma omp parallel for
			for (size_t i = 0; i < aiMesh->mNumVertices; i++) {
				// std::cout<<"Number of Vertices : "<<mesh->mNumVertices<<"\n";
				aiVector3D n = aiMesh->mNormals[i];
				mesh->normals[i] = glm::normalize(glm::vec3(n.x, n.y, n.z));
			}
		}
		else {
			std::cout << "Model has no normals.\n";
			//mesh->
#pragma omp parallel for
			for (size_t i = 0; i < aiMesh->mNumVertices; i++) {
				aiVector3D n = aiMesh->mNormals[i];
				mesh->normals[i] = glm::normalize(glm::vec3(n.x, n.y, n.z));
			}
		}
		// Fill face indices
//#pragma omp parallel for
		for (size_t i = 0; i < aiMesh->mNumFaces; i++) {
			std::array<unsigned int, 3> fac{ 0 };
			for (size_t j = 0; j < aiMesh->mFaces[i].mNumIndices; j++) {
				mesh->indices.push_back(aiMesh->mFaces[i].mIndices[j]);
				fac[j] = aiMesh->mFaces[i].mIndices[j];
			}
			mesh->faces.push_back(fac);
		}
		/*
		*/
		std::cout << "Number of vertices : " << mesh->vertices.size() << "\n";
		std::cout << "Number of normals : " << mesh->normals.size() << "\n";
		std::cout << "Number of indices : " << mesh->indices.size() << "\n";
		std::cout << "Number of faces : " << mesh->faces.size() << "\n";

		constexpr bool scaleDist = false;
		//Scale model for regular edge length
		if (scaleDist) {
			while (mesh->getMinDistance() < 0.1f) {
#pragma omp parallel for
				for (size_t i = 0; i < mesh->vertices.size(); i++) {
					mesh->vertices[i] *= 10.0f;
				}
			}
			while (mesh->getMinDistance() > 10.0f) {
#pragma omp parallel for
				for (size_t i = 0; i < mesh->vertices.size(); i++) {
					mesh->vertices[i] *= 0.1f;
				}
			}
		}

		// The "scene" pointer will be deleted automatically by "importer"
		return true;
	}
};