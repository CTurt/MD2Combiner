/*
 * md2.c -- md2 model loader
 * last modification: aug. 14, 2007
 *
 * Copyright(c) 2005-2007 David HENRY
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files(the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * gcc -Wall -ansi -lGL -lGLU -lglut md2.c -o md2
 */

#include "MD2.h"

vec3_t anorms_table[162] = {
	//#include "anorms.h"
};

/**
 * Load an MD2 model from file.
 *
 * Note: MD2 format stores model's data in little-endian ordering.	On
 * big-endian machines, you'll have to perform proper conversions.
 */
int ReadMD2Model(const char *filename, struct md2_model_t *mdl) {
	FILE *fp;
	int i;
	
	fp = fopen(filename, "rb");
	if(!fp) {
		fprintf(stderr, "Error: couldn't open \"%s\"!\n", filename);
		return 0;
	}
	
	fread(&mdl->header, 1, sizeof(struct md2_header_t), fp);
	
	if((mdl->header.ident != 844121161) || (mdl->header.version != 8)) {
		fprintf(stderr, "Error: bad version or identifier\n");
		fclose(fp);
		return 0;
	}
	
	/* Memory allocations */
	mdl->skins = (struct md2_skin_t *)malloc(sizeof(struct md2_skin_t) * mdl->header.num_skins);
	mdl->texcoords = (struct md2_texCoord_t *)malloc(sizeof(struct md2_texCoord_t) * mdl->header.num_st);
	mdl->triangles = (struct md2_triangle_t *)malloc(sizeof(struct md2_triangle_t) * mdl->header.num_tris);
	mdl->frames = (struct md2_frame_t *)malloc(sizeof(struct md2_frame_t) * mdl->header.num_frames);
	mdl->glcmds = (int *)malloc(sizeof(int) * mdl->header.num_glcmds);
	
	/* Read model data */
	fseek(fp, mdl->header.offset_skins, SEEK_SET);
	fread(mdl->skins, sizeof(struct md2_skin_t), mdl->header.num_skins, fp);
	
	fseek(fp, mdl->header.offset_st, SEEK_SET);
	fread(mdl->texcoords, sizeof(struct md2_texCoord_t), mdl->header.num_st, fp);
	
	fseek(fp, mdl->header.offset_tris, SEEK_SET);
	fread(mdl->triangles, sizeof(struct md2_triangle_t), mdl->header.num_tris, fp);
	
	fseek(fp, mdl->header.offset_glcmds, SEEK_SET);
	fread(mdl->glcmds, sizeof(int), mdl->header.num_glcmds, fp);
	
	/* Read frames */
	fseek(fp, mdl->header.offset_frames, SEEK_SET);
	for(i = 0; i < mdl->header.num_frames; i++) {
		/* Memory allocation for vertices of this frame */
		mdl->frames[i].verts =(struct md2_vertex_t *)malloc(sizeof(struct md2_vertex_t) * mdl->header.num_vertices);
		
		/* Read frame data */
		fread(mdl->frames[i].scale, sizeof(vec3_t), 1, fp);
		fread(mdl->frames[i].translate, sizeof(vec3_t), 1, fp);
		fread(mdl->frames[i].name, sizeof(char), 16, fp);
		fread(mdl->frames[i].verts, sizeof(struct md2_vertex_t), mdl->header.num_vertices, fp);
	}
	
	fclose(fp);
	return 1;
}

/**
 * Free resources allocated for the model.
 */
void FreeModel(struct md2_model_t *mdl) {
	int i;
	
	if(mdl->skins) {
		free(mdl->skins);
		mdl->skins = NULL;
	}
	
	if(mdl->texcoords) {
		free(mdl->texcoords);
		mdl->texcoords = NULL;
	}
	
	if(mdl->triangles) {
		free(mdl->triangles);
		mdl->triangles = NULL;
	}
	
	if(mdl->glcmds) {
		free(mdl->glcmds);
		mdl->glcmds = NULL;
	}
	
	if(mdl->frames) {
		for(i = 0; i < mdl->header.num_frames; i++) {
			free(mdl->frames[i].verts);
			mdl->frames[i].verts = NULL;
		}
		
		free(mdl->frames);
		mdl->frames = NULL;
	}
}
