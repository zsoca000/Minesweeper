#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
void tablakiiras(int** palya, int** fedolap, int sor_index, int oszlop_index);
bool vege_van(int** palya, int** fedolap, int sor_index, int oszlop_index);
void ha_nincs_bomba_korulotte(int x, int y, int** palya, int** fedolap, bool** holjartam, int sor_index, int oszlop_index);
void megoldas_megmutatas(int** palya, int** fedolap, int sor_index, int oszlop_index);
int main()
{
    ifstream f("pelda.palya");
    int sor_index, oszlop_index, bombak_szama;
    f >> sor_index;
    f >> oszlop_index;
    f >> bombak_szama;
    int** palya;
    palya = new int* [sor_index + 2];
    for (int i = 0; i < sor_index + 2; i++) {
        palya[i] = new int[oszlop_index + 2];
    }
    for (int i = 0; i < sor_index + 2; i++) {
        for (int j = 0; j < oszlop_index + 2; j++) {
            if ((i == 0) || (j == 0) || (i == sor_index + 1) || (j == oszlop_index + 1)) {
                palya[i][j] = 10;
            }
            else {
                f >> palya[i][j];
            }
        }
    }

    for (int i = 1; i < sor_index + 1; i++) { //palya krealas
        for (int j = 1; j < oszlop_index + 1; j++) {
            if (palya[i][j] == 9) {
                if (palya[i - 1][j - 1] != 9) {
                    palya[i - 1][j - 1]++;
                }
                if (palya[i - 1][j] != 9) {
                    palya[i - 1][j]++;
                }
                if (palya[i - 1][j + 1] != 9) {
                    palya[i - 1][j + 1]++;
                }
                if (palya[i][j + 1] != 9) {
                    palya[i][j + 1]++;
                }
                if (palya[i + 1][j + 1] != 9) {
                    palya[i + 1][j + 1]++;
                }
                if (palya[i + 1][j] != 9) {
                    palya[i + 1][j]++;
                }
                if (palya[i + 1][j - 1] != 9) {
                    palya[i + 1][j - 1]++;
                }
                if (palya[i][j - 1] != 9) {
                    palya[i][j - 1]++;
                }
            }
        }
    }

    int** fedolap; //0-ha semmi, 1-ha azt hiszi nincs bomba, 2-ha tipp
    fedolap = new int* [sor_index + 2];
    for (int i = 0; i < sor_index + 2; i++) {
        fedolap[i] = new int[oszlop_index + 2];
    }
    bool** holjartam;
    holjartam = new bool* [sor_index + 2];
    for (int i = 0; i < sor_index + 2; i++) {
        holjartam[i] = new bool[oszlop_index + 2];
    }

    for (int i = 0; i < sor_index + 2; i++) {
        for (int j = 0; j < oszlop_index + 2; j++) {
            fedolap[i][j] = 0;
            holjartam[i][j] = false;
        }
    }
    int tipp_sor, tipp_oszlop;
    char bomba_zaszlo;

    do {
        /*for(int i=1; i<sor_index+1; i++){
            for(int j=1; j<oszlop_index+1; j++){
                cout<<palya[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;*/
        tablakiiras(palya, fedolap, sor_index, oszlop_index);
        cout << "X/P: "; cin >> bomba_zaszlo;
        cout << "Sor: "; cin >> tipp_sor;
        cout << "Oszlop: "; cin >> tipp_oszlop;
        if ((bomba_zaszlo == 'X') && (fedolap[tipp_sor][tipp_oszlop] != 2)) {
            fedolap[tipp_sor][tipp_oszlop] = 1;
            if (palya[tipp_sor][tipp_oszlop] == 0) {
                ha_nincs_bomba_korulotte(tipp_sor, tipp_oszlop, palya, fedolap, holjartam, sor_index, oszlop_index);
            }
        }
        else {
            if ((bomba_zaszlo == 'P') && (fedolap[tipp_sor][tipp_oszlop] == 0)) {
                fedolap[tipp_sor][tipp_oszlop] = 2;
            }
            else {
                if ((bomba_zaszlo == 'P') && (fedolap[tipp_sor][tipp_oszlop] == 2)) {
                    fedolap[tipp_sor][tipp_oszlop] = 0;
                }
            }
        }
        system("cls");
    } while (!vege_van(palya, fedolap, sor_index, oszlop_index));


    f.close();
    return 0;
}

void tablakiiras(int** palya, int** fedolap, int sor_index, int oszlop_index) {
    cout << "   ";
    for (int i = 1; i < oszlop_index + 1; i++) {
        cout << i << " ";
    }
    cout << endl;
    cout << endl;
    for (int i = 1; i < sor_index + 1; i++) {
        cout << i << "  ";
        for (int j = 1; j < oszlop_index + 1; j++) {
            if (fedolap[i][j] == 0) {
                cout << "O ";
            }
            else {
                if (fedolap[i][j] == 1) {
                    cout << palya[i][j] << " ";
                }
                else {
                    cout << "P ";
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool vege_van(int** palya, int** fedolap, int sor_index, int oszlop_index) {
    for (int i = 1; i < sor_index + 1; i++) {
        for (int j = 1; j < oszlop_index + 1; j++) {
            if ((palya[i][j] == 9) && (fedolap[i][j] == 1)) {
                megoldas_megmutatas(palya, fedolap, sor_index, oszlop_index);
                cout << endl << "     LOSE     " << endl;
                return 1;
            }
        }
    }
    for (int i = 1; i < sor_index + 1; i++) {
        for (int j = 1; j < oszlop_index + 1; j++) {
            if ((palya[i][j] != 9) && (fedolap[i][j] != 1)) {
                return 0;
            }
        }
    }
    cout << "WIN";
    //zaszlo rakas ahova meg nem rakott a jatekos
    return 1;
}

void ha_nincs_bomba_korulotte(int x, int y, int** palya, int** fedolap, bool** holjartam, int sor_index, int oszlop_index) {
    holjartam[x][y] = true;
    fedolap[x - 1][y - 1] = 1;
    fedolap[x - 1][y] = 1;
    fedolap[x - 1][y + 1] = 1;
    fedolap[x][y + 1] = 1;
    fedolap[x + 1][y + 1] = 1;
    fedolap[x + 1][y] = 1;
    fedolap[x + 1][y - 1] = 1;
    fedolap[x][y - 1] = 1;
    if ((palya[x - 1][y - 1] == 0) && (holjartam[x - 1][y - 1] == false)) {
        ha_nincs_bomba_korulotte(x - 1, y - 1, palya, fedolap, holjartam, sor_index, oszlop_index);
    }
    if ((palya[x - 1][y] == 0) && (holjartam[x - 1][y] == false)) {
        ha_nincs_bomba_korulotte(x - 1, y, palya, fedolap, holjartam, sor_index, oszlop_index);
    }
    if ((palya[x - 1][y + 1] == 0) && (holjartam[x - 1][y + 1] == false)) {
        ha_nincs_bomba_korulotte(x - 1, y + 1, palya, fedolap, holjartam, sor_index, oszlop_index);
    }
    if ((palya[x][y + 1] == 0) && (holjartam[x][y + 1] == false)) {
        ha_nincs_bomba_korulotte(x, y + 1, palya, fedolap, holjartam, sor_index, oszlop_index);
    }
    if ((palya[x + 1][y + 1] == 0) && (holjartam[x + 1][y + 1] == false)) {
        ha_nincs_bomba_korulotte(x + 1, y + 1, palya, fedolap, holjartam, sor_index, oszlop_index);
    }
    if ((palya[x + 1][y] == 0) && (holjartam[x + 1][y] == false)) {
        ha_nincs_bomba_korulotte(x + 1, y, palya, fedolap, holjartam, sor_index, oszlop_index);
    }
    if ((palya[x + 1][y - 1] == 0) && (holjartam[x + 1][y - 1] == false)) {
        ha_nincs_bomba_korulotte(x + 1, y - 1, palya, fedolap, holjartam, sor_index, oszlop_index);
    }
    if ((palya[x][y - 1] == 0) && (holjartam[x][y - 1] == false)) {
        ha_nincs_bomba_korulotte(x, y - 1, palya, fedolap, holjartam, sor_index, oszlop_index);
    }

}

void megoldas_megmutatas(int** palya, int** fedolap, int sor_index, int oszlop_index) {
    for (int i = 1; i < sor_index + 1; i++) {
        for (int j = 1; j < oszlop_index + 1; j++) {
            if (palya[i][j] == 9) {
                cout << "X ";
            }
            else {
                if (fedolap[i][j] == 2) {
                    cout << "R ";
                }
                else {
                    if (fedolap[i][j] == 1) {
                        cout << palya[i][j] << " ";
                    }
                    else {
                        cout << "O ";
                    }
                }
            }
        }
        cout << endl;
    }
}