#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

void calcolaPercorso(double distanza, double vel_max, double accel, double decel, std::ofstream &file, double &tempo_totale, double &posizione_totale) {
    double tempo_accel = vel_max / accel;
    double distanza_accel = 0.5 * accel * tempo_accel * tempo_accel;

    double tempo_decel = vel_max / decel;
    double distanza_decel = 0.5 * decel * tempo_decel * tempo_decel;

    double distanza_costante = distanza - distanza_accel - distanza_decel;

    if (distanza_costante < 0) {
        tempo_accel = sqrt(distanza / (0.5 * (accel + decel)));
        tempo_decel = tempo_accel;
        distanza_accel = 0.5 * accel * tempo_accel * tempo_accel;
        distanza_decel = 0.5 * decel * tempo_decel * tempo_decel;
        distanza_costante = 0;
        vel_max = accel * tempo_accel;
    }

    double tempo_costante = distanza_costante / vel_max;

    double step_tempo = 0.1;
    for (double t = 0; t <= (tempo_accel + tempo_costante + tempo_decel); t += step_tempo) {
        double posizione = 0;
        double velocita = 0;

        if (t <= tempo_accel) {
            velocita = accel * t;
            posizione = 0.5 * accel * t * t;
        } else if (t <= tempo_accel + tempo_costante) {
            velocita = vel_max;
            posizione = distanza_accel + vel_max * (t - tempo_accel);
        } else if (t <= tempo_accel + tempo_costante + tempo_decel) {
            double t_decel = t - tempo_accel - tempo_costante;
            velocita = vel_max - decel * t_decel;
            posizione = distanza_accel + distanza_costante + 0.5 * decel * t_decel * t_decel;
        }

        file << std::fixed << std::setprecision(1) << tempo_totale + t << "\t" << posizione_totale + posizione << "\t" << velocita << std::endl;
    }
    tempo_totale = std::round((tempo_totale + tempo_accel + tempo_costante + tempo_decel) * 10.0) / 10.0;
    posizione_totale += distanza;
    file << std::endl;
}

int main() {
    std::ofstream file("percorso.txt");

    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file!" << std::endl;
        return 1;
    }

    file << "Time\tPosition\tSpeed" << std::endl;

    double tempo_totale = 0;
    double posizione_totale = 0;

    calcolaPercorso(40, 30.0 / 3.6, 2, 3, file, tempo_totale, posizione_totale);
    calcolaPercorso(180, 70.0 / 3.6, 2, 3, file, tempo_totale, posizione_totale);
    calcolaPercorso(110, 30.0 / 3.6, 2, 3, file, tempo_totale, posizione_totale);

    file.close();

    return 0;
}
