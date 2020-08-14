#pragma once


#define VERTEX_FVF (D3DFVF_XYZ|D3DFVF_TEX1)

struct Vertex
{

	Vertex(float x, float y, float z, float u, float v)
		: X{x}, Y{y}, Z{z},U{u}, V{v}
	{

	}
	float X, Y, Z;  // obj coordinates
	float U, V; // texture coordinates
	

};