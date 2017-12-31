// vim syntax:cpp
/*
 * Copyright (C) 2017 Vincent Sallaberry
 * vrobobox <https://github.com/vsallaberry/robobox>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
// BOARD / PIN definitions.
#define PIN_LED     5   // Définition du 'pin' de sortie de la LED
#define PIN_PROBE   2   // Définition du 'pin' du capteur de mouvement
#define PIN_SPEAKER 6   // Définition du 'pin' de sortue du buzzer

#define PLAY_ALL    1   // Put 0 to stop current song as soon Probe becomes OFF.

// Music definitions
enum notes_e {
    SIL = 0,
    DO = 262, DOd = 277, REb = DOd,
    RE = 294, REd = 311, MIb = REd,
    MI = 330,
    FA = 349, MId = FA, FAb = MI, FAd = 370, SOLb = FAd,
    SOL = 392, SOLd = 415, LAb = SOLd,
    LA = 440, LAd = 466, SIb = LAd,
    SI = 494, DOb = SI, SId = DO
};
typedef struct {
    unsigned int     freq;
    unsigned long    duration;
} note_t;
// Rythme des note, valeurs en milisecondes
#define TEMPO   100              // pulsations/seconde
#define N       (60000 / TEMPO) // Noire
#define C       (N / 2)         // Croche
#define DC      (N / 4)         // Doube-Croche
#define NP      (C * 3)         // Noire Pointée
#define B       (N * 2)         // Blanche
#define R       (N * 4)         // Ronde
#define FIN     (0)
#define OCTAVE  1.0             // Octave pour le son

static const note_t    DO_MAJEUR[]  = { { DO/2, C }, { RE/2, C }, { MI/2, C }, { FA/2, C }, { SOL/2, C }, { LA/2, C}, { SI/2, C },
                                      { DO, C }, { RE, C }, { MI, C }, { FA, C }, { SOL, C }, { LA, C}, { SI, C },
                                      { DO*2, C }, { RE*2, C }, { MI*2, C }, { FA*2, C }, { SOL*2, C }, { LA*2, C}, { SI*2, C },
                                      { DO*3, B }, { SIL, FIN } };
static const note_t    LA_MINEUR[]  = { /*{ LA/3, C }, { SI/3, C }, { DO/2, C }, { RE/2, C }, { MI/2, C }, { FA/2, C }, { (SOLd)/2, C }, */
                                        { LA/2, C}, { SI/2, C },  { DO, C }, { RE, C }, { MI, C }, { FA, C }, { SOLd, C },
                                        { LA, C}, { SI, C }, { DO*2, C }, { RE*2, C }, { MI*2, C }, { FA*2, C }, { SOLd*2, C },
                                        { LA*2, C}, { SIL, FIN } };
static const note_t    TABAC[]      = { { DO,C}, { RE,C }, { MI,C }, { DO,C }, { RE,N }, { RE,C }, { MI,C }, { FA,N }, { FA,N }, { MI,N }, { MI,N },
                                        { DO,C}, { RE,C }, { MI,C }, { DO,C }, { RE,N }, { RE,C }, { MI,C }, { FA,N }, { SOL,N }, { DO,B },
                                        { SIL, FIN } };
static const note_t    BIP[]    = { { LA, C }, { SIL, C }, { SIL, FIN } };
static const note_t *  ALARM    = TABAC;

// Static global variables.
static unsigned int     s_note = 0;

// Static utility methods.
static void sound(unsigned long, unsigned int);

// Setup code, to run once.
void setup() {
    pinMode(PIN_LED, OUTPUT);       // le 'pin' de la LED est une sortie
    pinMode(PIN_PROBE, INPUT);      // le 'pin' du capteur est une entrée
    pinMode(PIN_SPEAKER, OUTPUT);   // le 'pin' du speaker est une sortie
}

// Main code, to run repeatedly.
void loop() {
    if ((PLAY_ALL && s_note > 0) || digitalRead(PIN_PROBE) == HIGH) {
        digitalWrite(PIN_LED, HIGH);
        sound(ALARM[s_note].duration - 50, ALARM[s_note].freq);
        delay(50);
        s_note++;
        if (ALARM[s_note].duration == FIN) {
            s_note = 0;
        }
    } else {
        digitalWrite(PIN_LED, LOW);
        sound(0, 0);
        delay(300);
        s_note = 0;
    }
}

/* *********************************************************************************** */

void sound(unsigned long duration, unsigned int freq) {
    if (freq == 0) {
        digitalWrite(PIN_SPEAKER, LOW);
        delay(duration);
        return ;
    }
    const unsigned int  period      = (1.0 / (freq * OCTAVE)) * 1000000;

    // convert input msec in usec
    duration *= 1000;
    for (unsigned long elapsed_us = 0; elapsed_us < duration; elapsed_us += period) {
        digitalWrite(PIN_SPEAKER, HIGH);
        delayMicroseconds((4 * period) / 10);
        digitalWrite(PIN_SPEAKER, LOW);
        delayMicroseconds((6 * period) / 10);
    }
}

