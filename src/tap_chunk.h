#ifndef TAP_CHUNK_H
#define TAP_CHUNK_H

#include "util/tap_def.h"
#include "util/tap_result.h"
#include "util/tap_face.h"

#define CHUNK_SZ 3

typedef struct {
    TapVec3 world_coords;
    TapFace *faces;
    size_t num_faces;
} TapChunkMesh;

TapResult tap_chunk_init(void);
TapResult tap_chunk_get_meshes(TapVec3 player_coords, const TapChunkMesh **meshes, size_t *num_meshes);
TapResult tap_chunk_deinit(void);

#endif /* TAP_CHUNK_H */
