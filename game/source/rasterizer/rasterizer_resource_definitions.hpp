#pragma once

#include <d3d9.h>

enum e_vertex_type;
enum e_lighting_vertex_types;
enum e_transfer_vector_vertex_types;

struct c_vertex_declaration_table
{
	static IDirect3DVertexDeclaration9* __cdecl get_d3d_vertex_declaration(e_vertex_type base_vertex_type, e_lighting_vertex_types lighting_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type);
	static IDirect3DVertexDeclaration9* __cdecl get_d3d_vertex_declaration_by_entry_point(e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, long entry_point);
	static bool __cdecl set(e_vertex_type base_vertex_type, e_transfer_vector_vertex_types transfer_vertex_type, long entry_point);

	static D3DVERTEXELEMENT9(&lighting_vertex_decls)[4][64];
	static D3DVERTEXELEMENT9(&transfer_vertex_decls)[4][64];
	static D3DVERTEXELEMENT9(&other_vertex_decls)[4][64]; // 1: contrail, 2: beam, 3: light_volume
	static D3DVERTEXELEMENT9(&base_vertex_decls)[23][64];
	static IDirect3DVertexDeclaration9*(&m_vertex_declarations)[23][4][4];
};

