const platformSelect = document.getElementById('platform');
const archSelect = document.getElementById('arch');
const archContainer = document.getElementById('arch-container');
const typeContainer = document.getElementById('type-container');
const downloadBtn = document.getElementById('download-btn');
const sharedRadio = document.getElementById('shared');

const config = {
    windows: ['x64', 'arm64'],
    linux: ['x64', 'x86', 'arm64', 'armv7'],
    android: ['arm64', 'armv7', 'x64', 'x86'],
};

function getDownloadUrl() {
    const platform = platformSelect.value;

    if (platform === 'wasm') {
        return 'downloads/libmpegh-build-wasm.zip';
    }

    let arch = '';
    if (platform === 'macos') {
        arch = 'universal';
    } else {
        arch = archSelect.value;
    }

    if (platform !== 'macos' && platform !== 'wasm' && !arch) {
        return null;
    }

    const isShared = sharedRadio.checked;
    let filename = `libmpegh-build-${platform}-${arch}`;

    if (isShared) {
        filename += '-shared';
    }

    return `downloads/${filename}.zip`;
}

function updateButtonState() {
    const url = getDownloadUrl();

    if (url) {
        downloadBtn.href = url;
        downloadBtn.disabled = false;
    } else {
        downloadBtn.removeAttribute('href');
        downloadBtn.disabled = true;
    }
}

function updateForm() {
    const platform = platformSelect.value;

    archContainer.classList.add('hidden');
    typeContainer.classList.add('hidden');

    if (!platform) {
        updateButtonState();
        return;
    }

    if (platform === 'wasm') {
        updateButtonState();
        return;
    }

    if (platform === 'macos') {
        typeContainer.classList.remove('hidden');
        updateButtonState();
        return;
    }

    archContainer.classList.remove('hidden');
    typeContainer.classList.remove('hidden');

    if (config[platform]) {
        archSelect.innerHTML = '';

        const hiddenOption = document.createElement('md-select-option');
        hiddenOption.value = "";
        hiddenOption.style.display = "none";
        archSelect.appendChild(hiddenOption);

        config[platform].forEach((arch) => {
            const opt = document.createElement('md-select-option');
            opt.value = arch;

            const headline = document.createElement('div');
            headline.slot = "headline";
            headline.textContent = arch;

            opt.appendChild(headline);
            archSelect.appendChild(opt);
        });

        archSelect.value = "";
    }

    updateButtonState();
}

window.addEventListener('load', () => {
    platformSelect.value = '';
    archSelect.innerHTML = '';
    const hiddenOption = document.createElement('md-select-option');
    hiddenOption.value = "";
    hiddenOption.style.display = "none";
    archSelect.appendChild(hiddenOption);

    updateButtonState();
});

platformSelect.addEventListener('input', updateForm);
archSelect.addEventListener('input', updateButtonState);
typeContainer.addEventListener('change', updateButtonState);
