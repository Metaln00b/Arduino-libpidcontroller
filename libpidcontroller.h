/**

@file libpidcontroller.h

Regelschleife

w[k] -> Führungsgröße (SOLL-Wert)
e[k] -> Regelabweichung/Regeldifferenz
u[k] -> Stellgröße/Steuergröße
y[k] -> Regelgröße (IST-Wert)

               (-)           |--------|                |---------|
----w[k]-->-----+----e[k]--->| Regler |----u[k]------->| Strecke |----y[k]----x--->
                ^            |--------|                |---------|            |
                |                                                             v
                |            |---------|                                      |
                |<-----------| Messung |<-------------------<-----------------|
                             |---------|
*/


#ifndef libpidcontroller_h
#define libpidcontroller_h


/*******************************************************************************
 * Konstanten                                                                  *
 ******************************************************************************/

/* Definition des allgemeinen Datentyps */
typedef double pidc_num_t;


/*******************************************************************************
 * Datentypen                                                                  *
 ******************************************************************************/

/* Datentyp für PID-Regler */
typedef struct 
{
    pidc_num_t sample_rate_Hz;      /* Abtastrate/Abtastfrequenz    */

    pidc_num_t Kp;                  /* (P)roportionalkonstante      */
    pidc_num_t p;                   /* (P)roportional-Anteil        */

    pidc_num_t Ki;                  /* (I)ntegralkonstante          */
    pidc_num_t i;                   /* (I)ntegral-Anteil            */
    pidc_num_t i_max;               /* Max. I-Anteil (anti wind-up) */
    pidc_num_t i_min;               /* Min. I-Anteil (anti wind-up) */

    pidc_num_t Kd;                  /* (D)ifferentialkonstante      */
    pidc_num_t d;                   /* (D)ifferential-Anteil        */
    pidc_num_t previous_error;      /* Vor. Diff. zw. SOLL-IST      */

    pidc_num_t u_max;               /* Max. Stellgröße              */
    pidc_num_t u_min;               /* Min. Stellgröße              */
    pidc_num_t offset;              /* Offset Stellgröße            */
} pidc_t;


/*
 * Struktur für pidc_init()
 */
typedef struct {
    pidc_num_t sample_rate_Hz;

    pidc_num_t Kp;

    pidc_num_t Ki;
    pidc_num_t i_start;
    pidc_num_t i_max;
    pidc_num_t i_min;

    pidc_num_t Kd;

    pidc_num_t u_max;
    pidc_num_t u_min;
    pidc_num_t offset;
} pidc_init_args_t;

/**
 * Initalisiert den Regler
 * @param args: Struktur mit Parametern
 * @return Zeiger auf Objekt
 */
pidc_t * pidc_init (pidc_init_args_t args);

#if 0
pidc_t * pidc_init_base (pidc_init_args_t args);

#define pidc_init(...) pidc_init_base((pidc_init_args_t) {                     \
    .sample_rate_Hz = 1.0,                                                     \
    .Kp             = 0.0,                                                     \
    .Ki             = 0.0,                                                     \
    .i_start        = 0.0,                                                     \
    .i_min          = NAN,                                                     \
    .i_max          = NAN,                                                     \
    .Kd             = 0.0,                                                     \
    .u_min          = NAN,                                                     \
    .u_max          = NAN,                                                     \
    .offset         = 0,                                                       \
    __VA_ARGS__                                                                \
})
#endif


/**
 * Deinitialisiert den Regler
 * @param pidc: Zeiger auf Objekt
 */
void pidc_exit (pidc_t *pidc);

/**
 * Gibt die Stellgröße zurück
 * @param pidc: Zeiger auf Objekt
 * @param error: Differenz zw. SOLL- und IST-Wert
 * @return Stellgröße
 */
pidc_num_t pidc_calculate (pidc_t *pidc, pidc_num_t error);

/**
 * Setzt den Integralanteil und die vorherige Differenz zw. SOLL- und IST-Wert
 * des Regler zurück.
 * @param pidc: Zeiger auf Objekt
 */
void pidc_reset (pidc_t *pidc);
#endif