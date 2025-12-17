#!/bin/bash
patches=(
    "libmpegh/CMakeLists.txt patches/useobj_cmakelists.patch"
)

for item in "${patches[@]}"; do
    target_file=$(echo "$item" | awk '{print $1}')
    patch_file=$(echo "$item" | awk '{print $2}')

    echo "Applying patch $patch_file to $target_file"

    # CRLF -> LF
    perl -pi -e 's/\r\n/\n/g' "$target_file"
    perl -pi -e 's/\r\n/\n/g' "$patch_file"

    patch "$target_file" < "$patch_file"
done

echo "All patches applied"
