#!/bin/bash

patches=(
    "libmpegh/CMakeLists.txt patches/useobj_cmakelists.patch"
)

normalize_to_lf() {
    local file="$1"
    if command -v dos2unix &> /dev/null; then
        dos2unix "$file" &> /dev/null
    else
        perl -pe 's/\r\n|\n|\r/\n/g' "$file"
    fi
}

for item in "${patches[@]}"; do
    target_file=$(echo "$item" | awk '{print $1}')
    patch_file=$(echo "$item" | awk '{print $2}')

    echo "Applying patch $patch_file to $target_file"

    normalize_to_lf "$patch_file"
    normalize_to_lf "$target_file"

    patch -N "$target_file" < "$patch_file"
done

echo "All patches applied"
