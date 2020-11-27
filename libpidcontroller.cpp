#include <Arduino.h>

#include "libpidcontroller.h"

#if 0
pidc_t * 
pidc_init_base (pidc_init_args_t args)
#endif

pidc_t * 
pidc_init (pidc_init_args_t args)
{
    pidc_t *pidc = (pidc_t*)calloc(1, sizeof(pidc_t));

    pidc->sample_rate_Hz = args.sample_rate_Hz;

    pidc->Kp        = args.Kp;

    pidc->Ki        = args.Ki;
    pidc->i         = args.i_start;
    pidc->i_max     = args.i_max;
    pidc->i_min     = args.i_min;

    pidc->Kd        = args.Kd;

    pidc->u_max     = args.u_max;
    pidc->u_min     = args.u_min;
    pidc->offset    = args.offset;

    return pidc;
}


void 
pidc_exit (pidc_t *pidc)
{
    free(pidc);
}


pidc_num_t 
pidc_calculate (pidc_t *pidc, pidc_num_t error)
{
    /* P controller */
    pidc->p = pidc->Kp * error;

    /* I controller */
    pidc->i += (pidc->Ki * error) / pidc->sample_rate_Hz;

    if (!isnan(pidc->i_max) && pidc->i > pidc->i_max)
    {
        pidc->i = pidc->i_max;
    }
    else if (!isnan(pidc->i_min) && pidc->i < pidc->i_min)
    {
        pidc->i = pidc->i_min;
    }

    /* D controller */
    pidc->d = ((error - pidc->previous_error) * pidc->Kd) * pidc->sample_rate_Hz;
    pidc->previous_error = error;

    /* Calculate result */
    pidc_num_t result = (pidc->p + pidc->i + pidc->d)+pidc->offset;

    if (!isnan(pidc->u_max) && result > pidc->u_max)
    {
        result = pidc->u_max;
    }
    else if (!isnan(pidc->u_min) && result < pidc->u_min)
    {
        result = pidc->u_min;
    }

    return result;
}


void 
pidc_reset (pidc_t *pidc)
{
    pidc->i = 0;
    pidc->previous_error = 0;
}