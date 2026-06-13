# Using the custom WASM build

## Key features

Normally, the decoder writes the decoded PCM output to Emscripten's MEMFS. This can cause very large amounts of RAM to be used while decoding.

The streaming patch ([`patches/stream_output_wasm.patch`](patches/stream_output_wasm.patch)) alters this behavior:
1. It streams the decoded PCM data directly to the JS environment chunk by chunk
2. It sends progress updates via `postMessage` back to the main thread.

Logging behavior is not impacted by this (other than progress), so you must still use stdout/stderr capturing for that.

---

## 1. Handling PCM output (`self.writePcmChunk`)

To receive the decoded audio data, you must define a `writePcmChunk(pointer, size)` function on the global scope (`self`) of your Web Worker before the WASM module executes the decoding process.

The WASM binary will call `self.writePcmChunk(pointer, size)` for each decoded frame.

- `pointer`: A memory offset into the WASM heap where the PCM chunk is located.
- `size`: The length of the PCM chunk in bytes.

### Example 

```javascript
self.writePcmChunk = function(pointer, size) {
    const pcmData = Module.HEAP8.subarray(pointer, pointer + size);
    
    // Important: Copy the data if you plan to use it asynchronously or send it
    // to the main thread, as the WASM memory buffer will be reused/overwritten.
    const pcmCopy = new Int8Array(pcmData);
    
    // Process the data, or send it to the main thread
    self.postMessage({ 
        type: "pcm_data", 
        data: pcmCopy 
    }, [pcmCopy.buffer]); // Transferable for better performance
};
```

---

## 2. Progress events

The decoder emits progress events via `postMessage` as frames are decoded.

You can listen for these messages in your main thread to update your application's UI.

### Example

```javascript
const decoderWorker = new Worker('decoder_worker.js');

decoderWorker.onmessage = function(event) {
    const msg = event.data;
    
    if (msg.type === "progress") {
        console.log(`Successfully decoded frame: ${msg.frame}`);
        // Update your UI here
    } else if (msg.type === "pcm_data") {
        // Handle the incoming PCM audio chunks
        const audioBuffer = msg.data;
    }
};
```
