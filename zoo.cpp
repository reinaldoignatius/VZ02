/* Author    : Andika Kusuma / 13515033
 * File      : zoo.cpp
 */

#include "zoo.h"
#include "road.h"
#include "habitat.h"
#include "park.h"
#include "restaurant.h"
#include <time.h>
#include <list>
#include <set>
#include <stack>
#include <fstream>
#include <iostream>

using namespace std;

MakroGetterAnimal(string,GetId,in);
MakroGetterAnimal(int,GetNumber,in);
MakroGetterAnimal(float,GetWeight,in);
MakroGetterAnimal(float,GetEat,in);
MakroGetterAnimal(pair<int,int>,GetPos,in);
MakroGetterAnimal(char,GetType,in);
MakroGetterAnimal(char,GetLegend,in);
MakroGetterAnimal(set<char>,GetHabitat,in);
MakroGetterAnimal(set<string>,GetCompatible,in);

MakroGetterCell(char,GetInitSymbol,in);
MakroGetterCell(char,GetSymbol,in);
MakroGetterSekat(in,direction);

Zoo::Zoo(bool Auto, int w, int l): width(w), length(l) {
  cells = new Pointer* [width];
  for (int i = 0; i < width; ++i) {
    cells[i] = new Pointer [length];
  }
  string line;
  if (Auto) { // automatis ctor dari file eksternal
    ifstream ifile("map.txt");
    if (ifile.is_open()) {
      int i = 0;
      while (getline(ifile, line)) {
        for (int j = 0; j < length; ++j) {
          switch (line[j]) {
            case 'W': {
              Habitat w ('W');
              Habitat* pw = &w;
              cells[i][j].p = (Habitat*) pw;
              cells[i][j].id = "HA"; 
            }
            break;
            case 'A': {
              Habitat a ('A');
              Habitat* pa = &a;
              cells[i][j].p = (Habitat*) pa;
              cells[i][j].id = "HA"; 
            }  
            break;
            case 'L': {
              Habitat l ('L');
              Habitat* pl = &l;
              cells[i][j].p = (Habitat*) pl;
              cells[i][j].id = "HA"; 
            }  
            break;
            case 'X': {
              Road x ('X');
              Road* px = &x;
              cells[i][j].p = (Road*) px;
              cells[i][j].id = "RO"; 
            } 
            break; 
            case 'N': {
              Road n ('N');
              Road* pn = &n;
              cells[i][j].p = (Road*) pn;
              cells[i][j].id = "RO"; 
            }  
            break;
            case 'r': {
              Road r ('r');
              Road* pr = &r;
              cells[i][j].p = (Road*) pr;
              cells[i][j].id = "RO"; 
            }  
            break;
            case 'R': {
              Restaurant rt;
              Restaurant* prt = &rt;
              cells[i][j].p = (Restaurant*) prt;
              cells[i][j].id = "RE";
            }
            break;
            case 'P': {
              Park p;
              Park* pp = &p;
              cells[i][j].p = (Park*) pp;
              cells[i][j].id = "PA";
            }  
            break;
          }
        }
        ++i;
      }
    }
    ifile.close();
  }
  else { // membaca input dari user
    for (int i = 0; i < width; ++i) {
      getline(cin, line);
      for (int j = 0; j < length; ++j) {
        switch (line[j]) {
          case 'W': {
              Habitat w ('W');
              Habitat* pw = &w;
              cells[i][j].p = (Habitat*) pw;
              cells[i][j].id = "HA"; 
            }
            break;
            case 'A': {
              Habitat a ('A');
              Habitat* pa = &a;
              cells[i][j].p = (Habitat*) pa;
              cells[i][j].id = "HA"; 
            }  
            break;
            case 'L': {
              Habitat l ('L');
              Habitat* pl = &l;
              cells[i][j].p = (Habitat*) pl;
              cells[i][j].id = "HA"; 
            }  
            break;
            case 'X': {
              Road x ('X');
              Road* px = &x;
              cells[i][j].p = (Road*) px;
              cells[i][j].id = "RO"; 
            } 
            break; 
            case 'N': {
              Road n ('N');
              Road* pn = &n;
              cells[i][j].p = (Road*) pn;
              cells[i][j].id = "RO"; 
            }  
            break;
            case 'r': {
              Road r ('r');
              Road* pr = &r;
              cells[i][j].p = (Road*) pr;
              cells[i][j].id = "RO"; 
            }  
            break;
            case 'R': {
              Restaurant rt;
              Restaurant* prt = &rt;
              cells[i][j].p = (Restaurant*) prt;
              cells[i][j].id = "RE";
            }
            break;
            case 'P': {
              Park p;
              Park* pp = &p;
              cells[i][j].p = (Park*) pp;
              cells[i][j].id = "PA";
            }  
            break;
        }
      }
    }
  }
  // caging
  cage_map = new int* [w];
  for (int i = 0; i < w; i++) {
    cage_map[i] = new int[l];
    for (int j = 0; j < l; j++) {
      cage_map[i][j] = -99;
    }
  } // alokasi memori cage_map
  int counter = 1; // counter cage ke-i
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < l; j++) {
      if (cage_map[i][j] == -99) { // jika belum terisi
        char c = MakroGetterCell(GetSymbol,cells[i][j]);
        if (c != 'W' && c != 'A' && c != 'L') {
        // jika bukan merupakan habitat akan diisi  0
          cage_map[i][j] = 0;
        }
        else { // habitat
          int array_i[4], array_j[4]; //array menyimpan posisi yang akan dirubah
          for (int k = 0; k < 4; k++) {
            array_i[k] = i, array_j[k] = j; // inisialisasi
          }
          bool cek = true; // boolean apakah bisa membentuk cage atau tidak
          cage_map[i][j] = counter; // dimasukkan ke cage counter
          srand(time(NULL));
          pair<int,int> movable[12]; // array pair jalan yang mungkin
          int ii, jj, i_temp = i, j_temp = j, count = 0;
          for (int times = 0; times < 3; times++) {
          // cukup 3x karena akan dibuat cage dengan
          // jumlah cell 4 terlebih dahulu
            for (int k = 0; k < 4; k++) { // ada 4 sel bersebelahan
              bool dummy = false;
              // atas
              if (k == 0 && i_temp != 0) {
                ii = i_temp - 1, jj = j_temp, dummy = true;
              }
              // kiri
              else if (k == 1 && j_temp != 0) {
                ii = i_temp, jj = j_temp - 1, dummy = true;
              }
              // bawah
              else if (k == 2 && i_temp != w - 1) {
                ii = i_temp + 1, jj = j_temp, dummy = true;
              }
              // kanan
              else if (k == 3 && j_temp != l - 1) {
                ii = i_temp, jj = j_temp + 1, dummy = true;
              }
              if (dummy) { // jika ada sel disebelahnya
                if (MakroGetterCell(GetSymbol,cells[ii][jj]) == c &&
                    cage_map[ii][jj] == -99) {
                // jika sama habitatnya dan belum masuk ke cage
                  movable[count] = make_pair(ii,jj);
                  // menjadi salah satu pilihan
                  count++; // jumlah pilihan
                }
              }
            }
            if (count == 0) { // gaada pilihan, maka akan direturn salah
              cek = false;
              break;
            }
            int move = rand() % count; // random jalan
            i_temp = movable[move].first;
            j_temp = movable[move].second;
            array_i[times] = i_temp; // pengisian posisi yang sudah diubah
            array_j[times] = j_temp;
            movable[move] = make_pair(movable[count-1].first,
                                      movable[count-1].second);
            cage_map[i_temp][j_temp] = counter;
            count--;
            if (times == 2) counter++; 
            // jika sudah selesai dengan cage dengan 4 cell counter ditambah
          }
          if (!cek) { 
          // jumlah cell tidak sampai 4
          //semua yang sudah diisi dikembalikan ke-99
            for (int k = 0; k < 4; k++) {
              cage_map[array_i[k]][array_j[k]] = -99;
            }
          }
        }
      }
    }
  }
  cage_nb = counter-1;
  // mengurus habitat yang belum masuk ke cage
  int change = -1; // banyak perubahan yang dilakukan
  while (change != 0) { // jika sudah tidak ada perubahan berhenti
    change = 0; // inisialisasi
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < l; j++) {
        if (cage_map[i][j] == -99) { // masih belum terisi
          pair<int,int> movable[4]; // array jalan yang mungkin
          int count = 0, ii, jj;
          for (int k = 0; k < 4; k++) { 
          // ada 4 kemungkinan, atas, bawah, kiri, kanan
            bool dum = false; // boolean mungkin atau tidak
            if (k == 0 && i != 0) { // atas mungkin
              ii = i - 1, jj = j, dum = true;
            }
            else if (k == 1 && j != 0) { // kiri mungkin
              ii = i, jj = j - 1, dum = true;
            }
            else if (k == 2 && i != w - 1) { // bawah mungkin
              ii = i + 1, jj = j, dum = true;
            }
            else if (k == 3 && j != l - 1) { // kanan mungkin
              ii = i, jj = j + 1, dum = true;
            }
            if (dum) { // jika jalan mungkin
              if (MakroGetterCell(GetSymbol,cells[ii][jj]) == 
                  MakroGetterCell(GetSymbol,cells[i][j]) &&
                  cage_map[ii][jj] != -99) {
              // jika habitatnya sama, dan sudah masuk ke cage
                movable[count] = make_pair(ii,jj);
                count++;
              }
            }
          }
          if (count > 0) { // jika ada pilihan
            int move = rand() % count; // random
            ii = movable[move].first;
            jj = movable[move].second;
            cage_map[i][j] = cage_map[ii][jj];
            change++;
          }
        }
      }
    }
  }
  if (Auto) {
    // membaca animals dari animals.txt
    ifstream in("animals.txt");
    if (in.is_open()) {
      while (getline(in, line)) {
        string id = "";
        int px = 0;
        int py = 0;
        int j = 0;
        int weight = 0;
        int decimal_weight = 0;
        // membaca id
        while (line[j] != '|') {
          id += line[j];
          j++;
        }
        j++;
        // membaca px
        while (line[j] != '|') {
          px = 10*px + line[j] - '0';
          j++;
        }
        j++;
        //membaca py
        while (j < (int)line.length() && line[j] != '|') {
          py = 10*py + line[j] - '0';
          j++;
        }
        if (line[j] == '|') {
          j++;
          while (line[j] != '.' && j < (int)line.length()) {
            weight = 10*weight + line[j] - '0';
            j++;
          }
          if (line[j] == '.') {
            int multiplier = 1;
            while (j < (int)line.length()) {
              multiplier *= 0.1;
              decimal_weight += multiplier*line[j];
              j++;
            }
          }
          weight += decimal_weight;
        }
        if (id == "WF") {
          Wolf* pa;
          if (weight != 0) pa = new Wolf(weight,make_pair(py,px));
          else pa = new Wolf(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "LI") {
          Lion* pa;
          if (weight != 0) pa = new Lion(weight,make_pair(py,px));
          else pa = new Lion(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "ZBR") {
          Zebra* pa;
          if (weight != 0) pa = new Zebra(weight,make_pair(py,px));
          else pa = new Zebra(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "ELP") {
          Elephant* pa;
          if (weight != 0) pa = new Elephant(weight,make_pair(py,px));
          else pa = new Elephant(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "MCQ") {
          Macaque* pa;
          if (weight != 0) pa = new Macaque(weight,make_pair(py,px));
          else pa = new Macaque(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "HG") {
          Hog* pa;
          if (weight != 0) pa = new Hog(weight,make_pair(py,px));
          else pa = new Hog(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "EGL") {
          Eagle* pa;
          if (weight != 0) pa = new Eagle(weight,make_pair(py,px));
          else pa = new Eagle(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "OW") {
          Owl* pa;
          if (weight != 0) pa = new Owl(weight,make_pair(py,px));
          else pa = new Owl(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "HMB") {
          Hummingbird* pa;
          if (weight != 0) pa = new Hummingbird(weight,make_pair(py,px));
          else pa = new Hummingbird(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "CKT") {
          Cockatoo* pa;
          if (weight != 0) pa = new Cockatoo(weight,make_pair(py,px));
          else pa = new Cockatoo(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "RBN") {
          Robin* pa;
          if (weight != 0) pa = new Robin(weight,make_pair(py,px));
          else pa = new Robin(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "BT") {
          Bat* pa;
          if (weight != 0) pa = new Bat(weight,make_pair(py,px));
          else pa = new Bat(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "SHK") {
          Shark* pa;
          if (weight != 0) pa = new Shark(weight,make_pair(py,px));
          else pa = new Shark(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "MRE") {
          MorayEel* pa;
          if (weight != 0) pa = new MorayEel(weight,make_pair(py,px));
          else pa = new MorayEel(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "DGG") {
          Dugong* pa;
          if (weight != 0) pa = new Dugong(weight,make_pair(py,px));
          else pa = new Dugong(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "TRL") {
          Turtle* pa;
          if (weight != 0) pa = new Turtle(weight,make_pair(py,px));
          else pa = new Turtle(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "DLP") {
          Dolphin* pa;
          if (weight != 0) pa = new Dolphin(weight,make_pair(py,px));
          else pa = new Dolphin(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "WHL") {
          Whale* pa;
          if (weight != 0) pa = new Whale(weight,make_pair(py,px));
          else pa = new Whale(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "PNG") {
          Penguin* pa;
          if (weight != 0) pa = new Penguin(weight,make_pair(py,px));
          else pa = new Penguin(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "HPP") {
          Hippopotamus* pa;
          if (weight != 0) pa = new Hippopotamus(weight,make_pair(py,px));
          else pa = new Hippopotamus(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "PLC") {
          Pelican* pa;
          if (weight != 0) pa = new Pelican(weight,make_pair(py,px));
          else pa = new Pelican(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "GSE") {
          Goose* pa;
          if (weight != 0) pa = new Goose(weight,make_pair(py,px));
          else pa = new Goose(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "CRN") {
          Crane* pa;
          if (weight != 0) pa = new Crane(weight,make_pair(py,px));
          else pa = new Crane(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "DRL") {
          DracoLizard* pa;
          if (weight != 0) pa = new DracoLizard(weight,make_pair(py,px));
          else pa = new DracoLizard(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "CLG") {
          Colugo* pa;
          if (weight != 0) pa = new Colugo(weight,make_pair(py,px));
          else pa = new Colugo(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "SGL") {
          SugarGlider* pa;
          if (weight != 0) pa = new SugarGlider(weight,make_pair(py,px));
          else pa = new SugarGlider(make_pair(py,px));
          AddAnimal(pa);
        }
      }
    }
    in.close();
  }
  else{
    char option;
    do {
      cout << "Ingin menambah hewan lagi? (y/n)";
      cin >> option;
      if (option == 'Y' || option == 'y') {
        string id;
        char def_weight;
        int px;
        int py;
        int weight = -1;
        cout << "Input id hewan :";
        cin >> id;
        cout << "Input posisi x : (kolom ke-)";
        cin >> px;
        cout << "Input posisi y : (baris ke-)";
        cin >> py;
        cout << "Apakah ingin menggunakan berat default? (y/n)";
        cin >> def_weight;
        if (def_weight == 'Y' || def_weight == 'y') {
          do{
            cout << "Input berat : (>0)";
            cin >> weight;
          } while (weight <= 0);
        }
        if (id == "WF") {
          Wolf* pa;
          if (weight != -1) pa = new Wolf(weight, make_pair(py,px));
          else pa = new Wolf(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "LI") {
          Lion* pa;
          if (weight != -1) pa = new Lion(weight, make_pair(py,px));
          else pa = new Lion(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "ZBR") {
          Zebra* pa;
          if (weight != -1) pa = new Zebra(weight, make_pair(py,px));
          else pa = new Zebra(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "ELP") {
          Elephant* pa;
          if (weight != -1) pa = new Elephant(weight, make_pair(py,px));
          else pa = new Elephant(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "MCQ") {
          Macaque* pa;
          if (weight != -1) pa = new Macaque(weight, make_pair(py,px));
          else pa = new Macaque(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "HG") {
          Hog* pa;
          if (weight != -1) pa = new Hog(weight, make_pair(py,px));
          else pa = new Hog(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "EGL") {
          Eagle* pa;
          if (weight != -1) pa = new Eagle(weight, make_pair(py,px));
          else pa = new Eagle(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "OW") {
          Owl* pa;
          if (weight != -1) pa = new Owl(weight, make_pair(py,px));
          else pa = new Owl(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "HMB") {
          Hummingbird* pa;
          if (weight != -1) pa = new Hummingbird(weight, make_pair(py,px));
          else pa = new Hummingbird(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "CKT") {
          Cockatoo* pa;
          if (weight != -1) pa = new Cockatoo(weight, make_pair(py,px));
          else pa = new Cockatoo(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "RBN") {
          Robin* pa;
          if (weight != -1) pa = new Robin(weight, make_pair(py,px));
          else pa = new Robin(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "BT") {
          Bat* pa;
          if (weight != -1) pa = new Bat(weight, make_pair(py,px));
          else pa = new Bat(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "SHK") {
          Shark* pa;
          if (weight != -1) pa = new Shark(weight, make_pair(py,px));
          else pa = new Shark(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "MRE") {
          MorayEel* pa;
          if (weight != -1) pa = new MorayEel(weight, make_pair(py,px));
          else pa = new MorayEel(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "DGG") {
          Dugong* pa;
          if (weight != -1) pa = new Dugong(weight, make_pair(py,px));
          else pa = new Dugong(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "TRL") {
          Turtle* pa;
          if (weight != -1) pa = new Turtle(weight, make_pair(py,px));
          else pa = new Turtle(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "DLP") {
          Dolphin* pa;
          if (weight != -1) pa = new Dolphin(weight, make_pair(py,px));
          else pa = new Dolphin(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "WHL") {
          Whale* pa;
          if (weight != -1) pa = new Whale(weight, make_pair(py,px));
          else pa = new Whale(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "PNG") {
          Penguin* pa;
          if (weight != -1) pa = new Penguin(weight, make_pair(py,px));
          else pa = new Penguin(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "HPP") {
          Hippopotamus* pa;
          if (weight != -1) pa = new Hippopotamus(weight, make_pair(py,px));
          else pa = new Hippopotamus(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "PLC") {
          Pelican* pa;
          if (weight != -1) pa = new Pelican(weight, make_pair(py,px));
          else pa = new Pelican(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "GSE") {
          Goose* pa;
          if (weight != -1) pa = new Goose(weight, make_pair(py,px));
          else pa = new Goose(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "CRN") {
          Crane* pa;
          if (weight != -1) pa = new Crane(weight, make_pair(py,px));
          else pa = new Crane(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "DRL") {
          DracoLizard* pa;
          if (weight != -1) pa = new DracoLizard(weight, make_pair(py,px));
          else pa = new DracoLizard(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "CLG") {
          Colugo* pa;
          if (weight != -1) pa = new Colugo(weight, make_pair(py,px));
          else pa = new Colugo(make_pair(py,px));
          AddAnimal(pa);
        }
        else if (id == "SGL") {
          SugarGlider* pa;
          if (weight != -1) pa = new SugarGlider(weight, make_pair(py,px));
          else pa = new SugarGlider(make_pair(py,px));
          AddAnimal(pa);
        }
      }
    }while (option == 'Y' || option == 'y');
  }
}

Zoo::Zoo(const Zoo& z): width(z.width), length(z.length) {
  cells = new Pointer* [width];
  for (int i = 0; i < width; i++) cells[i] = new Pointer [length];
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < length; j++) {
      switch (MakroGetterCell(GetInitSymbol,(z.cells[i][j]))) {
        case 'W':
          cells[i][j] = new Habitat('W');
          break;
        case 'A':
          cells[i][j] = new Habitat('A');
          break;
        case 'L':
          cells[i][j] = new Habitat('L');
          break;
        case 'X':
          cells[i][j] = new Road('X');
          break;
        case 'N':
          cells[i][j] = new Road('N');
          break;
        case 'r':
          cells[i][j] = new Road('r');
          break;
        case 'R':
          cells[i][j] = new Restaurant();
          break;
        case 'P':
          cells[i][j] = new Park();
          break;
      }
    }
  }
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < length; j++) {
      cage_map[i][j] = z.cage_map[i][j];
    }
  }
  cage_nb = z.cage_nb;
}

Zoo::~Zoo() {
  for (int i = 0; i < width; i++) delete [] cage_map[i];
  delete [] cage_map;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < length; j++) {
      delete cells[i][j];
    }
    delete [] cells[i];
  }
  delete [] cells;
}
  
Zoo& Zoo::operator=(const Zoo& z) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < length; j++) {
      switch (MakroGetterCell(GetInitSymbol, z.cells[i][j])) {
        case 'W':
          cells[i][j] = new Habitat('W');
          break;
        case 'A':
          cells[i][j] = new Habitat('A');
          break;
        case 'L':
          cells[i][j] = new Habitat('L');
          break;
        case 'X':
          cells[i][j] = new Road('X');
          break;
        case 'N':
          cells[i][j] = new Road('N');
          break;
        case 'r':
          cells[i][j] = new Road('r');
          break;
        case 'R':
          cells[i][j] = new Restaurant();
          break;
        case 'P':
          cells[i][j] = new Park();
          break;
      }
    }
  }
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < length; j++) {
      cage_map[i][j] = z.cage_map[i][j];
    }
  }
  cage_nb = z.cage_nb;
  return *this;
}

void Zoo::Display(int x1, int y1, int x2, int y2) {
  for (int i = x1; i <= x2; ++i) {
    for (int j = y1; j <= y2; ++j) {
      MakroRender(cells[i][j]);
    }
    cout << endl;
  }
}

list<Pointer>::iterator Zoo::FindAnimal(pair<int,int> pos) {
  list<Pointer>::iterator it = animals.begin();
  list<Pointer>::iterator e = animals.end();
  --e;
  if (it != animals.end()) {
    while (MakroGetterAnimal(GetPos,(*it)) != pos && it != e) {
    ++it;
    }
    if (MakroGetterAnimal(GetPos,(*it)) == pos) {
      return it;
    } else {
      return animals.end();
    }
  } else {
    return it;
  }
  
}

void Zoo::AddAnimal(Pointer a) {
  int posx = MakroAnimalCell(GetPos,a).first;
  int posy = MakroAnimalCell(GetPos,a).second;
  if (posx < width && posy << length) {
    int cage = cage_map[posx][posy];
    // cek if habitat dlu
    set<char> hab = MakroGetterAnimal(GetHabitat,a);
    set<string> compability = MakroGetterAnimal(GetCompatible,a)();
    if (FindAnimal(make_pair(posx,posy)) == animals.end()) {
      if (hab.find(MakroGetterCell(GetSymbol,cells[posx][posy])) != hab.end()) {
        bool compatible = true; 
        // cek apakah ada hewan yang tidak kompatible dengan hewan a
        int count = 0; // count animal yang ada di cage yang sama
        for (list<Pointer>::const_iterator it = animals.begin();
             it != animals.end(); ++it) {
          if (cage == cage_map[MakroGetterAnimal(GetPos,(*it)).first]
                              [MakroGetterAnimal(GetPos,(*it)).second]) {
            count++;
            if (compability.find(MakroGetterAnimal(GetId,a)) ==
                compability.end()) {
              compatible = false;
            }
          }
        }
        int max = 0;
        for (int i = 0; i < width; i++) {
          for (int j = 0; j < length; j++) {
            if (cage_map[i][j] == cage) {
              max++;
            }
          }
        }
        if (0.3*max >= (count+1) && compatible) { // masih muat cagenya
          animals.push_back(a);
          MakroSetterCell(SetSymbol,cells[posx][posy],
                          MakroGetterAnimal(GetLegend,a));
        }
      }
    }
  }
}

void Zoo::DelAnimal(string _id, int _number) {
  list<Pointer>::iterator it = animals.begin();
  list<Pointer>::iterator e = animals.end();
  --e;
  while (MakroGetterAnimal(GetId,(*it)) != _id && 
         MakroGetterAnimal(GetNumber,(*it)) != _number && it != e) {
    ++it;
  }
  if (MakroGetterAnimal(GetId,(*it)) == _id && 
      MakroGetterAnimal(GetNumber,(*it)) == _number) {
    delete (*it);
    animals.erase(it);
  }
  int posx = MakroGetterAnimal(GetPos,(*it)).first;
  int posy = MakroGetterAnimal(GetPos,(*it)).second;
  MakroSetterCell(SetSymbol,cells[posx][posy],
                  MakroGetterCell(GetInitSymbol,cells[posx][posy]));
}

void Zoo::DelAnimal(int x, int y) {
  if (FindAnimal(make_pair(x,y)) != animals.end()) {
    DelAnimal(MakroGetterAnimal(GetId,(*FindAnimal(make_pair(x,y)))),
              MakroGetterAnimal(GetNumber,(*FindAnimal(make_pair(x,y)))));
  }
}

float Zoo::GetTotalMeat() const {
  float sum = 0;
  for (list<Pointer>::const_iterator it = animals.begin();
       it != animals.end();
       ++it) {
    if (MakroGetterAnimal(GetType,(*it)) == 'K') {
      sum += MakroGetterAnimal(GetWeight,(*it)) * 
             MakroGetterAnimal(GetEat,(*it));
    }
    else if (MakroGetterAnimal(GetType,(*it)) == 'O') {
      sum += 0.5 * MakroGetterAnimal(GetWeight,(*it)) * 
             MakroGetterAnimal(GetEat,(*it));
    }
  }
  return sum;
}

float Zoo::GetTotalVegetables() const {
  float sum = 0;
  for (list<Pointer>::const_iterator it = animals.begin();
       it != animals.end();
       ++it) {
    if (MakroGetterAnimal(GetType,(*it)) == 'H') {
      sum += MakroGetterAnimal(GetWeight,(*it)) * 
             MakroGetterAnimal(GetEat,(*it));
    }
    else if (MakroGetterAnimal(GetType,(*it)) == 'O') {
      sum += 0.5 * MakroGetterAnimal(GetWeight,(*it)) * 
             MakroGetterAnimal(GetEat,(*it));
    }
  }
  return sum;
}

void Zoo::MoveAnimal(pair<int, int> pos, int direction) {
  list<Pointer>::iterator it = FindAnimal(pos);
  if (it != animals.end()) {
    if (MakroGetSekat(cells[pos.first][pos.second],direction)) {
      bool valid = false;
      int i = pos.first, j = pos.second;
      switch(direction) {
        case 0:
          if (i-1 >= 0) {
            valid = true;
            --i;
          }
          break;
        case 1:
          if (j-1 >= 0) {
            valid = true;
            --j;
          }
          break;
        case 2:
          if (j+1 < length) {
            valid = true;
            ++j;
          }
          break;
        case 3:
          if (i+1 < width) {
            valid = true;
            ++i;
          }
          break;
      }
      if (valid) {
        if (FindAnimal(make_pair(i,j)) == animals.end()) {
          MakroMove((*it),direction);
          MakroSetterSymbol(SetSymbol,cells[pos.first][pos.second],
                            MakroGetterCell(GetInitSymbol,cells[pos.first]
                                                               [pos.second]));
          MakroSetterSymbol(SetSymbol,
                            cells[MakroGetterAnimal(GetPos,(*it)).first]
                                 [MakroGetterAnimal(GetPos,(*it)).second],
                            MakroGetterAnimal(GetLegend,(*it)));
        }
      }
    }
  }
}

void Zoo::MoveAnimal(string _id, int _number, int direction) {
  list<Pointer>::iterator it = animals.begin();
  list<Pointer>::iterator e = animals.end();
  --e;
  while (MakroGetterAnimal(GetId,(*it)) != _id && 
         MakroGetterAnimal(GetNumber,(*it)) != _number && it != e) {
    ++it;
  }
  if (MakroGetterAnimal(GetId,(*it)) == _id && 
      MakroGetterAnimal(GetNumber,(*it)) == _number) {
    MoveAnimal(MakroGetterAnimal(GetPos,(*it)), direction);
  }
}

void Zoo::MoveAllAnimal() {
  srand(time(NULL));
  for (list<Pointer>::iterator it = animals.begin();
       it != animals.end();
       ++it) {
    MoveAnimal(MakroGetterAnimal(GetPos,(*it)), (rand()%4));
  }
}

void Zoo::ToggleSekat(int i, int j, int direction) {
  if (i >= 0 && i < width && j >= 0 && j < length) {
    char c = MakroGetterCell(GetInitSymbol,cells[i][j]);
    if (c == 'W' || c == 'L' || c == 'A') {
      switch (direction) {
        case 0:
          if (i-1 >= 0) {
            if (cage_map[i-1][j] == cage_map[i][j]) {
              MakroToggleSekat(cells[i][j],0);
              MakroToggleSekat(cells[i-1][j],3);
            }
          }
          break;
        case 1:
          if (j-1 >= 0) {
            if (cage_map[i][j-1] == cage_map[i][j]) {
              MakroToggleSekat(cells[i][j],1);
              MakroToggleSekat(cells[i][j-1],2);
            }
          }
          break;
        case 2:
          if (j+1 < length) {
            if (cage_map[i][j+1] == cage_map[i][j]) {
              MakroToggleSekat(cells[i][j],2);
              MakroToggleSekat(cells[i][j+1],1);
            }
          }
          break;
        case 3:
          if (i+1 < width) {
            if (cage_map[i+1][j] == cage_map[i][j]) {
              MakroToggleSekat(cells[i][j],3);
              MakroToggleSekat(cells[i+1][j],0);
            }
          }
          break;
      }
    }
  }
}

void Zoo::ToggleAllSekat() {
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < length; ++j) {
      for (int k = 0; k < 2; ++k) {
        ToggleSekat(i, j, k);
      }
    }
  }
}

void Zoo::Tour() {
  set<pair<int,int>> entrance;
  bool vis[width][length];
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < length; ++j) {
      if (MakroGetterCell(GetSymbol,cells[i][j]) == 'N') {
        entrance.insert(make_pair(i, j));
      }
      vis[i][j] = false;
    }
  }
  srand(time(NULL));
  int selection = rand() % entrance.size();
  set<pair<int,int>>::iterator it = entrance.begin();
  for (int i = 0; i < selection; ++i) {
    ++it;
  }
  stack<pair<int,int>> dstack;
  list<int> route;
  dstack.push(*it);
  bool found = false;
  while (!found) {
    int i = dstack.top().first, j = dstack.top().second;
    vis[i][j] = true;
    if (MakroGetterCell(GetSymbol,cells[i][j]) == 'X') {
      found = true;
      route.push_back(4);
    } else {
      char c;
      set<int> choice;
      if (i-1 >= 0) {
        c = MakroGetterCell(GetSymbol,cells[i-1][j]);
        if (c == 'r' || c == 'X') {
          if (!vis[i-1][j]) {
            choice.insert(0);
          }
        }
      }
      if (j - 1 >= 0) {
        c = MakroGetterCell(GetSymbol,cells[i][j-1]);
        if (c == 'r' || c == 'X') {
          if (!vis[i][j-1]) {
            choice.insert(1);
          }
        }
      }
      if (j + 1 < length) {
        c = MakroGetterCell(GetSymbol,cells[i][j+1]);
        if (c == 'r' || c == 'X') {
          if (!vis[i][j+1]) {
            choice.insert(2);
          }
        }
      }
      if (i+1 < width) {
        c = MakroGetterCell(GetSymbol,cells[i+1][j]);
        if (c == 'r' || c == 'X') {
          if (!vis[i+1][j]) {
            choice.insert(3);
          }
        }
      }
      if (choice.size() > 0) {
        selection = rand() % choice.size();
        set<int>::iterator st = choice.begin();
        for (int k = 0; k < selection; ++k) {
          ++st;
        }
        route.push_back(*st);
        switch(*st) {
          case 0:
            dstack.push(make_pair(i - 1, j));
            break;
          case 1:
            dstack.push(make_pair(i, j - 1));
            break;
          case 2:
            dstack.push(make_pair(i, j + 1));
            break;
          case 3:
            dstack.push(make_pair(i + 1, j));
            break;
        }
      } else {
        dstack.pop();
        route.pop_back();
      }
    }
  }
  int i = it->first, j = it->second;
  bool VisCage[cage_nb];
  for (int i = 0; i < cage_nb; ++i) {
    VisCage[i] = false;
  }
  while (!route.empty()) {
    if (i - 1 >= 0) {
      char c = MakroGetterCell(GetInitSymbol,cells[i-1][j]);
      if (c == 'P' || c == 'R') {
        MakroInteractCell(cells[i-1][j]);
      } else if (c == 'W' || c == 'L' || c == 'A') {
        if (!VisCage[cage_map[i-1][j]]) {
          InteractCage(make_pair(i - 1,j), cage_map[i-1][j]);
          VisCage[cage_map[i-1][j]] = true;
        }
      }
    }
    if (j - 1 >= 0) {
      char c = MakroGetterCell(GetInitSymbol,cells[i][j-1]);
      if (c == 'P' || c == 'R') {
        MakroInteractCell(cells[i][j-1]);
      } else if (c == 'W' || c == 'L' || c == 'A') {
        if (!VisCage[cage_map[i][j-1]]) {
          InteractCage(make_pair(i,j - 1), cage_map[i][j-1]);
          VisCage[cage_map[i][j-1]] = true;
        }
      }
    }
    if (j + 1 < length) {
      char c = MakroGetterCell(GetInitSymbol,cells[i][j+1]);
      if (c == 'P' || c == 'R') {
        MakroInteractCell(cells[i][j+1]);
      } else if (c == 'W' || c == 'L' || c == 'A') {
        if (!VisCage[cage_map[i][j+1]]) {
          InteractCage(make_pair(i,j + 1), cage_map[i][j+1]);
          VisCage[cage_map[i][j+1]] = true;
        }
      }
    }
    if (i + 1 < width) {
      char c = MakroGetterCell(GetInitSymbol,cells[i+1][j]);
      if (c == 'P' || c == 'R') {
        MakroInteractCell(cells[i+1][j]);
      } else if (c == 'W' || c == 'L' || c == 'A') {
        if (!VisCage[cage_map[i+1][j]]) {
          InteractCage(make_pair(i + 1,j), cage_map[i+1][j]);
          VisCage[cage_map[i+1][j]] = true;
        }
      }
    }
    switch (route.front()) {
      case 0:
        --i;
        break;
      case 1:
        --j;
        break;
      case 2:
        ++j;
        break;
      case 3:
        ++i;
        break;
    }
    route.pop_front();
  }
}

void Zoo::InteractCage(pair<int,int> pos, int cage_number) {
  bool Vis[width][length];
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < length; ++j) {
      Vis[i][j] = false;
    }
  }
  queue<pair<int, int>> bqueue;
  bqueue.push(pos);
  Vis[pos.first][pos.second] = true;
  while (!bqueue.empty()) {
    int i = bqueue.front().first, j = bqueue.front().second;
    bqueue.pop();
    if (FindAnimal(make_pair(i,j)) != animals.end()) {
      MakroInteractAnimal((*FindAnimal(make_pair(i,j))));
    }  
    if (i-1 >= 0) {
      if (cage_map[i-1][j] == cage_number) {
        if (!Vis[i-1][j]) {
          bqueue.push(make_pair(i - 1, j));
          Vis[i-1][j] = true;
        }
      }
    }
    if (j - 1 >= 0) {
      if (cage_map[i][j-1] == cage_number) {
        if (!Vis[i][j-1]) {
          bqueue.push(make_pair(i,j - 1));
          Vis[i][j-1] = true;
        }
      }
    }
    if (j+1 < length) {
      if (cage_map[i][j+1] == cage_number) {
        if (!Vis[i][j+1]) {
          bqueue.push(make_pair(i,j + 1));
          Vis[i][j+1] = true;
        }
      }
    }
    if (i+1 < width) {
      if (cage_map[i+1][j] == cage_number) {
        if (!Vis[i+1][j]) {
          bqueue.push(make_pair(i + 1,j));
          Vis[i+1][j] = true;
        }
      }
    }  
  }
}
