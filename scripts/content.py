import os

THIS_FOLDER = os.path.dirname(os.path.abspath(__file__))

data_tmpl = """#include <PR/ultratypes.h>
#include <PR/gbi.h>

#include "macros.h"
#include "surface_terrains.h"
#include "types.h"
#include "make_const_nonconst.h"

ALIGNED8 const Texture movie_imageNUM[] = {
#include "levels/castle_grounds/imageNUM.rgba16.inc.c"
};
"""

for i in range(334):
    path_folder = f"{THIS_FOLDER}/../compressed_content/img{i}"
    os.makedirs(path_folder, exist_ok=True)
    path_data = f"{path_folder}/data.c"
    with open(path_data, "w") as f:
        data = data_tmpl.replace("NUM", str(i))
        f.write(data)
