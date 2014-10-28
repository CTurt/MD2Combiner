#include "MD2.h"

#define VERSION "0.1"

struct md2_model_t md2inputs[2];
struct md2_header_t combinedHeader;

void cleanup(void) {
	FreeModel(&md2inputs[0]);
	FreeModel(&md2inputs[1]);
}

int main(int argc, char *argv[]) {
	printf("MD2Combiner %s - Combines all frames from two MD2s into one MD2\n\n", VERSION);
	
	if(argc != 4) {
		printf("Usage: %s <input1.md2> <input2.md2> <output.md2>\n", argv[0]);
		return -1;
	}
	
	atexit(cleanup);
	
	// Read models
	if(!ReadMD2Model(argv[1], &md2inputs[0])) {
		printf("Error reading %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	if(!ReadMD2Model(argv[2], &md2inputs[1])) {
		printf("Error reading %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}
	
	// Make sure they are compatible
	if(md2inputs[0].header.num_vertices != md2inputs[1].header.num_vertices || md2inputs[0].header.num_tris != md2inputs[1].header.num_tris) {
		printf("Models were read but could not be combined due to either a different amount of vertices or triangles\n");
		exit(EXIT_FAILURE);
	}
	
	// Calculate new offsets
	int offset = 0;
	
	memcpy(&combinedHeader, &md2inputs[0].header, sizeof(struct md2_header_t));
	offset += sizeof(struct md2_header_t);
	
	combinedHeader.offset_skins = offset;
	offset += md2inputs[0].header.num_skins * sizeof(struct md2_skin_t);
	
	combinedHeader.offset_st = offset;
	offset += md2inputs[0].header.num_st * sizeof(struct md2_texCoord_t);
	
	combinedHeader.offset_tris = offset;
	offset += md2inputs[0].header.num_tris * sizeof(struct md2_triangle_t);
	
	combinedHeader.offset_glcmds = offset;
	offset += md2inputs[0].header.num_glcmds * sizeof(int);
	
	combinedHeader.num_frames += md2inputs[1].header.num_frames;
	combinedHeader.offset_frames = offset;
	offset += combinedHeader.num_frames * sizeof(struct md2_frame_t);
	
	combinedHeader.offset_end = offset;
	
	// Save new model
	FILE *f = fopen(argv[3], "wb");
	if(!f) {
		printf("Error writing to %s\n", argv[3]);
		exit(EXIT_FAILURE);
	}
	
	fwrite(&combinedHeader, sizeof(struct md2_header_t), 1, f);
	
	fseek(f, combinedHeader.offset_skins, SEEK_SET);
	fwrite(md2inputs[0].skins, sizeof(struct md2_skin_t), combinedHeader.num_skins, f);
	
	fseek(f, combinedHeader.offset_st, SEEK_SET);
	fwrite(md2inputs[0].texcoords, sizeof(struct md2_texCoord_t), combinedHeader.num_st, f);
	
	fseek(f, combinedHeader.offset_tris, SEEK_SET);
	fwrite(md2inputs[0].triangles, sizeof(struct md2_triangle_t), combinedHeader.num_tris, f);
	
	fseek(f, combinedHeader.offset_glcmds, SEEK_SET);
	fwrite(md2inputs[0].glcmds, sizeof(int), combinedHeader.num_glcmds, f);
	
	int n, i;
	for(n = 0; n < 2; n++) {
		for(i = 0; i < md2inputs[n].header.num_frames; i++) {
			printf("Writing frame %s\n", md2inputs[n].frames[i].name);
			fwrite(md2inputs[n].frames[i].scale, sizeof(vec3_t), 1, f);
			fwrite(md2inputs[n].frames[i].translate, sizeof(vec3_t), 1, f);
			fwrite(md2inputs[n].frames[i].name, sizeof(char), 16, f);
			fwrite(md2inputs[n].frames[i].verts, sizeof(struct md2_vertex_t), md2inputs[n].header.num_vertices, f);
		}
	}
	
	fclose(f);
	
	return 0;
}
