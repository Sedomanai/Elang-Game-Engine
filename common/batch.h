/*****************************************************************//**
 * @file   batch.h
 * @brief  
 * Batches are flexible enough to be used by all sorts of graphic libraries.
 * Able to accomodate all kinds of vertex and material types.
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/


#include "define.h"

namespace el
{	
	struct Batch
	{
		void* vertices; 
		sizet* indices;
		uint32 vertex_count, index_count, material, flag;
		float depth;

#pragma warning ( push )
#pragma warning ( disable: 26495 )
		// Necessary for entt
		Batch() {};
#pragma warning ( pop )

		/**
		 * Construct a batch.
		 * 
		 * @param vertices_- Array of verticies
		 * @param indices_- Array of indicies
		 * @param vertex_count_- Amount of vertex count
		 * @param index_count_- Amount of index count
		 * @param material_- Entity index of material
		 * @param flag_- Batch flags which usage may vary between graphic engines
		 * @param depth_- General sort depth of batch
		 */
		Batch(void* vertices_, sizet* indices_, uint32 vertex_count_, uint32 index_count_, uint32 material_, uint32 flag_, float depth_)
			: vertices(vertices_), vertex_count(vertex_count_), indices(indices_), 
			index_count(index_count_), material(material_), depth(depth_), flag(flag_)
		{}
	};	
}
