#include <math.h>
#include <stdlib.h>
#include "lv2.h"
#include "log.h"

#define PLUGIN_URI "http://brnv.lv2/plugins/ir-cab-sim"

typedef enum {
    INPUT = 0,
    OUTPUT = 1,
    FILE_SIZE = 99,
    FILE_DATA = 100
} PortIndex;

typedef struct {
    const float *input;
    float *output;
    float *impulseResponse;
    float * buffer ;
    int buffer_size ;
} IRCabSim;

static LV2_Handle instantiate(
    const LV2_Descriptor *descriptor,
    double rate,
    const char *bundle_path,
    const LV2_Feature *const *features
) {
    IRCabSim *irCabSim = (IRCabSim*)malloc(sizeof(IRCabSim));

    return (LV2_Handle)irCabSim ;
}

static void connect_port(
    LV2_Handle instance,
    uint32_t port,
    void *data
) {
    IRCabSim *irCabSim = (IRCabSim*)instance;

    switch ((PortIndex)port) {
        case INPUT:
            irCabSim->input = (const float*)data;
            break;
        case OUTPUT:
            irCabSim->output = (float*)data;
            break;
        case FILE_SIZE:
            irCabSim->buffer_size = * (int *) data ;
            break ;
        case FILE_DATA:
            for (int i = 0 ; i < irCabSim->buffer_size ; i ++)
                irCabSim->buffer [i] = ((float *) data) [i] ;
            irCabSim->impulseResponse = irCabSim->buffer ;
            break ;
    }
}

static void activate(LV2_Handle instance) {
    IRCabSim *irCabSim = (IRCabSim*) instance;
}

static void run(LV2_Handle instance, uint32_t n_samples) {
    const IRCabSim *irCabSim = (const IRCabSim*)instance;

    const float *const input  = irCabSim->input;
    float *const output = irCabSim->output;
    float *impulseResponse = irCabSim->impulseResponse;

    /*http://ptolemy.eecs.berkeley.edu/eecs20/week12/Image53.gif*/

    int j;

    float resultSample;

    for (int n = 0; n < n_samples; n++)
    {
        resultSample = 0;

        for (int m = 0; m < sizeof(impulseResponse); m++)
        {
            j = n - m;
            if (j < 0)
            {
                break;
            }

            resultSample += impulseResponse[m] * input[j];
        }

        output[j] = resultSample;
    }
}

static void deactivate(LV2_Handle instance) {}

static void cleanup(LV2_Handle instance) {
    free(instance);
}

static const void *extension_data(const char *uri) {
    return NULL;
}

static const LV2_Descriptor descriptor = {
    PLUGIN_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index) {
    switch (index) {
        case 0:  return &descriptor;
        default: return NULL;
    }
}
