#include <libpidcontroller.h>

void setup() {
    // put your setup code here, to run once:

}

void loop() {
    pidc_init_args_t args = {
        1.0,        /* sample_rate_Hz */
        0.2,        /* Kp */
        0.1,        /* Ki */
        0.0,        /* i_start */
        -4095,      /* i_min */
        4095,       /* i_max */
        0.0,        /* Kd */
        -4095,      /* u_min */
        4095,       /* u_max */
        0,          /* offset */
    };
    pidc_t *pidc = pidc_init (args);

    pidc_num_t pid = 0.0;
    double soll = 1024;
    double ist = 2;
    double error = soll-ist;
    while (round(error) != 0)
    {
        pid = pidc_calculate(pidc, error);
        error = soll-pid;
        printf("(SOLL/IST): (%.2lf/%.2lf)    ERR: %.2lf\n", soll, pid, error);
        delay(1);
    }
    pidc_exit(pidc);
}