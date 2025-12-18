#!/bin/bash
PLATFORM="${1:-all}"

patches=(
    "libmpegh/CMakeLists.txt patches/useobj_cmakelists.patch all"
    "libmpegh/test/impeghd_main.c patches/disable_stdoutbuf_wasm.patch wasm"
)

normalize_to_lf() {
    local file="$1"
    if command -v dos2unix &> /dev/null; then
        dos2unix "$file" &> /dev/null
    else
        perl -pe 's/\r\n|\n|\r/\n/g' -i "$file"
    fi
}

platform_matches() {
    local wanted="$1"
    local allowed="$2"

    if [[ "$allowed" == "all" ]]; then
        return 0
    fi

    IFS=',' read -ra platforms <<< "$allowed"
    for p in "${platforms[@]}"; do
        if [[ "$p" == "$wanted" ]]; then
            return 0
        fi
    done

    return 1
}

for item in "${patches[@]}"; do
    read -r target_file patch_file platforms <<< "$item"

    if ! platform_matches "$PLATFORM" "$platforms"; then
        echo "Skipping $patch_file (platform: $platforms)"
        continue
    fi

    echo "Applying patch $patch_file to $target_file (platform: $PLATFORM)"

    normalize_to_lf "$patch_file"
    normalize_to_lf "$target_file"

    patch -p0 --fuzz=3 --ignore-whitespace "$target_file" < "$patch_file"
done

echo "All applicable patches applied"
