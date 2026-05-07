#!/bin/bash


cd python

cd process_images

#image_types=("item" "spell" "skill" "creature" "object" "trainer )
image_types=("trainer" )

echo $type

#process images for use
for type in "${image_types[@]}"
do
  echo ${type}
  python copy_first_images.py ../../../assets_raw/sprites/${type} ../../../assets_processed/${type}s/staged
  python transparent_bulk.py ../../../assets_processed/${type}s/staged ../../../assets_processed/${type}s/with_transparency
  python rescale_reformat.py ../../../assets_processed/${type}s/with_transparency ../../../assets_processed/${type}s/deployable
  python compress_img.py ../../../assets_processed/${type}s/deployable
done


cd ..
cd ..


