#pragma once

#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef float vec3_t[3];

struct md2_header_t {
	int ident;
	int version;

	int skinwidth;
	int skinheight;

	int framesize;

	int num_skins;
	int num_vertices;
	int num_st;
	int num_tris;
	int num_glcmds;
	int num_frames;

	int offset_skins;
	int offset_st;
	int offset_tris;
	int offset_frames;
	int offset_glcmds;
	int offset_end;
};

struct md2_skin_t {
	char name[64];
};

struct md2_texCoord_t {
	short s;
	short t;
};

struct md2_triangle_t {
	unsigned short vertex[3];
	unsigned short st[3];
};

struct md2_vertex_t {
	unsigned char v[3];
	unsigned char normalIndex;
};

struct md2_frame_t {
	vec3_t scale;
	vec3_t translate;
	char name[16];
	struct md2_vertex_t *verts;
};

struct md2_glcmd_t {
	float s;
	float t;
	int index;
};

struct md2_model_t {
	struct md2_header_t header;

	struct md2_skin_t *skins;
	struct md2_texCoord_t *texcoords;
	struct md2_triangle_t *triangles;
	struct md2_frame_t *frames;
	int *glcmds;

	GLuint tex_id;
};

int ReadMD2Model(const char *filename, struct md2_model_t *mdl);
void FreeModel(struct md2_model_t *mdl);
