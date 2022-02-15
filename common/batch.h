#include "define.h"

namespace el
{	
	//=======================================================================================
	// Batches are flexible enough to be used by all sorts of graphic libraries 
	// Able to accomodate all kinds of vertex and material types
	//=======================================================================================
	struct Batch
	{
		void* vertices; 
		sizet* indices;
		uint32 vertex_count, index_count, material, flag;
		float depth;

		Batch() {};
		Batch(void* vertices_, sizet* indices_, uint32 vertex_count_, uint32 index_count_, uint32 material_, uint32 flag_, float depth_)
			: vertices(vertices_), vertex_count(vertex_count_), indices(indices_), 
			index_count(index_count_), material(material_), depth(depth_), flag(flag_)
		{}
	};	
}
