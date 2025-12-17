#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "impeghd_type_def.h"
#include "impeghd_memory_standards.h"
#include "impeghd_error_codes.h"
#include "impeghd_api.h"

void* wrapper_mem_alloc(UWORD32 size, UWORD32 alignment) { return malloc(size + alignment); }
void wrapper_mem_free(void* ptr) { free(ptr); }

typedef struct {
    ia_mpeghd_api_struct api;
    void* decoder_handle;
    
        int32_t bytes_valid; } DecoderContext;

DecoderContext* create_decoder(int sample_rate, int pcm_word_size) {
    DecoderContext* ctx = (DecoderContext*)malloc(sizeof(DecoderContext));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(DecoderContext));

    ctx->api.output_config.malloc_mpegh = wrapper_mem_alloc;
    ctx->api.output_config.free_mpegh   = wrapper_mem_free;

        ctx->api.input_config.ui_pcm_wd_sz = pcm_word_size;
    ctx->api.input_config.out_samp_freq = sample_rate;
    ctx->api.input_config.ui_cicp_layout_idx = 2;     ctx->api.input_config.ui_mhas_flag = 1;       
        if (ia_mpegh_dec_create(&ctx->api.input_config, &ctx->api.output_config) != IA_MPEGH_DEC_NO_ERROR) {
        free(ctx);
        return NULL;
    }
    
    ctx->decoder_handle = ctx->api.output_config.pv_ia_process_api_obj;
    ctx->bytes_valid = 0;
    return ctx;
}


uint8_t* get_input_buffer(DecoderContext* ctx) {
        return (uint8_t*)ctx->api.output_config.mem_info_table[IA_MEMTYPE_INPUT].mem_ptr;
}

int get_input_write_offset(DecoderContext* ctx) {
    return ctx->bytes_valid;
}

int get_input_capacity(DecoderContext* ctx) {
    return ctx->api.output_config.ui_inp_buf_size;
}

int process_packet(DecoderContext* ctx, int new_bytes_count) {
    if (!ctx) return -1;

        ctx->bytes_valid += new_bytes_count;
    ctx->api.input_config.num_inp_bytes = ctx->bytes_valid;

    IA_ERRORCODE err;

        if (!ctx->api.output_config.ui_init_done) {
        err = ia_mpegh_dec_init(ctx->decoder_handle, &ctx->api.input_config, &ctx->api.output_config);
        
                if (err == IA_MPEGH_DEC_INIT_NONFATAL_INSUFFICIENT_INPUT_BYTES) {
            return 1;         }
    } else {
        err = ia_mpegh_dec_execute(ctx->decoder_handle, &ctx->api.input_config, &ctx->api.output_config);
        
                if (err == IA_MPEGH_DEC_EXE_NONFATAL_INSUFFICIENT_INPUT_BYTES) {
                     }
    }

    if (err != IA_MPEGH_DEC_NO_ERROR && 
        err != IA_MPEGH_DEC_INIT_NONFATAL_INSUFFICIENT_INPUT_BYTES &&
        err != IA_MPEGH_DEC_EXE_NONFATAL_INSUFFICIENT_INPUT_BYTES) {
        return (int)err;     }

            int bytes_consumed = ctx->api.output_config.i_bytes_consumed;
    int bytes_remaining = ctx->bytes_valid - bytes_consumed;

    if (bytes_remaining > 0 && bytes_consumed > 0) {
        uint8_t* base = get_input_buffer(ctx);
        memmove(base, base + bytes_consumed, bytes_remaining);
    }
    
        ctx->bytes_valid = bytes_remaining;

        return 0;
}

uint8_t* get_output_buffer(DecoderContext* ctx) {
        return (uint8_t*)ctx->api.output_config.mem_info_table[IA_MEMTYPE_OUTPUT].mem_ptr;
}

int get_output_size(DecoderContext* ctx) {
    return ctx->api.output_config.num_out_bytes;
}

void destroy_decoder(DecoderContext* ctx) {
    if (ctx) {
        ia_mpegh_dec_delete(&ctx->api.output_config);
        free(ctx);
    }
}