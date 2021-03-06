/** Copyright (C) 2010-2012 by Jason L. McKesson **/
/** This file is licensed under the MIT License. **/


#ifndef FRAMEWORK_MESH_H
#define FRAMEWORK_MESH_H

#include <string>

namespace Framework
{
	struct MeshData;

	class Mesh
	{
	public:
		Mesh(const std::string &strFilename);
		~Mesh();

		void Render() const;
		void Render(const std::string &strMeshName) const;
		void DeleteObjects();

	private:
		MeshData *m_pData;
	};
}


#endif //FRAMEWORK_MESH_H
