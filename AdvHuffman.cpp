#include "AdvHuffman.h"
#include <cmath>
#include <QString>
#include <fstream>
#include <iostream>

AdvHuffman::AdvHuffman(){
    TabNode = new Node[513];
    TabNode[512].setAktif(true);
    TabKar = new int[256];
    for (int i = 0;i <= 255;i++){
        TabKar[i] = -1;
    }
    NYT = 512;
    NomorNode = new int[513];
    NomorNode[512] = 512;
    for (int i=0; i<512; i++)
        NomorNode[i] = -1;
}

/**
 * Convert a char into an array of bits
 */
char* charToArr(char c) {
    char* ans = new char[8];
    for (int i = 0; i < 8; i++) {
        ans[i] = (c << (7-i));
        ans[i] >>= 7;
        if (ans[i] == -1) {
            ans[i] = 1;
        }
    }
    return ans;
}

/**
 * Convert an array of bits into a char
 */
char arrToChar(char *arr) {
    int ans = 0;
    for (int i = 0; i < 8; i++) {
        ans += arr[i] * pow(float(2), i);
    }
    return char(ans);
}

QString getFileName(QString filePath) {
    const char *fPath = filePath.toStdString().c_str();
    QString fName;
    for (int i = filePath.size() - 1; i >= 0; i--) {
        if (fPath[i] != '/') {
            fName.push_front(fPath[i]);
        } else {
            break;
        }
    }
    return fName;
}

void AdvHuffman::Encoding(const char* inputFile, const char* outputFile, double *entrpy, double *bps){
    // Setting up files
    string dummyFile(outputFile);
    dummyFile += "Dummy";
    ifstream infile(inputFile, ios::in|ios::binary);
    ofstream ofile(dummyFile.c_str(), ios::out|ios::binary);

    // First process: converting the file into a text file which contains the "uncompressed"
    // string representation of bit code
    int totalBits = 0;
    char karakter;
    infile.get(karakter);
    unsigned char c = karakter;
    insertAfter(c);
    UpdateNomorNode();
    Update(TabKar[c],true);
    ofile << charToCode(c);
    totalBits += charToCode(c).length();

    // Next
    while (infile.get(karakter)) {
        unsigned char c = karakter;
        *bps += charToCode(c).length();
        if (TabKar[c] == -1){
            ofile << NYTcodeFromTree(512);
            totalBits += NYTcodeFromTree(512).length();
            insertAfter(c);
            ofile << charToCode(c);
            totalBits += charToCode(c).length();
            UpdateNomorNode();
            Update(TabNode[TabKar[c]].getParent(),true);
        } else {
            ofile << charCodeFromTree(512,c);
            totalBits += charCodeFromTree(512,c).length();
            Update(TabKar[c],false);
        }
    }
    infile.close();
    ofile.close();

    // Converting the resulting file with the string bit code representation into the
    // compressed form
    infile.open(dummyFile.c_str(), ios::in|ios::binary);
    ofile.open(outputFile, ios::out|ios::binary);
    char container[8];
    int idx = 0;

    ofile << getFileName(inputFile).toStdString();
    ofile << ' ';
    ofile << totalBits;
    ofile << ' ';
    char cc;
    while (infile.get(cc)) {
        char bit = cc - 48;
        container[idx] = bit;
        idx++;
        if (idx == 8) {
            idx = 0;
            ofile << arrToChar(container);
        }
    }
    // Putting dummy bits if the container is not empty
    if (idx != 0) {
        while (idx != 8) {
            container[idx] = 0;
            idx++;
        }
    }
    ofile << arrToChar(container);
    infile.close();
    ofile.close();

    // Remove dummy file
    remove(dummyFile.c_str());
}

void AdvHuffman::Decoding(const char* inputFile, QString outPath) {
    ifstream infile(inputFile, ios::in|ios::binary);
    string outputFileName;

    // Read the name of the "original" file
    infile >> outputFileName;

    // Create the bits string dummy file from the compressed file
    string dummyOut(inputFile);
    dummyOut += "Dummy_2";
    ofstream ofile(dummyOut.c_str(), ios::out|ios::binary);
    int count;
    infile >> count;
    infile.get(); // Skips a space
    char cc;
    while (infile.get(cc) && count > 0) {
        char *bitArr = charToArr(cc);
        for (int i = 0; i < 8; i++) {
            ofile << int(bitArr[i]);
            count--;
            if (count == 0) {
                break;
            }
        }
        delete[] bitArr;
    }
    infile.close();
    ofile.close();

    // Create the original file from the bits string
    outPath.append("//");
    outPath.append(outputFileName.c_str());
    cout << outPath.toStdString() << endl;
    infile.open(dummyOut.c_str(), ios::in|ios::binary);
    ofile.open(outPath.toStdString().c_str(), ios::out|ios::binary);
    char karakter;
    //First
    int ans = 0;
    for (int i = 7; i >= 0; i--) {
        infile.get(karakter);
        if (karakter == '1')
        ans += 1 * pow(float(2), i);
    }
    char k = (char) ans;
    unsigned char c = (unsigned char) ans;
    ofile << k;
    insertAfter(c);
    UpdateNomorNode();
    Update(TabKar[c],true);
    //Next
    do {
        int P = 512;
        while (!isLeaf(P)){
            infile.get(karakter);
            if (karakter == '0'){
                int temp = P;
                P = TabNode[temp].getLeft();
            } else {
                int temp = P;
                P = TabNode[temp].getRight();
            }
        }
        if (P == NYT){
            int ans = 0;
            for (int i = 7; i >= 0; i--) {
                infile.get(karakter);
                if (karakter == '1')
                    ans += 1 * pow(float(2), i);
            }
            char k = (char) ans;
            unsigned char c = (unsigned char) ans;
            if (!infile.eof()) {
                ofile << k;
            }
            insertAfter(c);
            UpdateNomorNode();
            Update(TabNode[TabKar[c]].getParent(),true);
        } else {
            if (!infile.eof()) {
                char k = (char) TabNode[P].getKar();
                ofile << k;
            }
            Update(P,false);
        }
    } while (!infile.eof());
    infile.close();
    ofile.close();

    // Remove dummy file 2
    remove(dummyOut.c_str());
}

void AdvHuffman::Display(int T){
    if (T == -1){
        cout << "()";
    } else {
        cout << "(";
        Display(TabNode[T].getLeft());
        cout << (TabNode[T].getKar());
        Display(TabNode[T].getRight());
        cout << ")";
    }
}

//METHOD UNTUK TABKAR
bool AdvHuffman::searchChar(unsigned char K)
/*Menghasilkan true ketika kar dberada pada pohon*/
{
    return ((TabKar[K] < 513) && (TabKar[K] >= 0));
}

int AdvHuffman::adressChar(unsigned char K)
/*Menghasilkan alamat kar pada pohon. -1 bila tidak ada*/
{
    return TabKar[K];
}

void AdvHuffman::setAdrressChar(unsigned char K, int Address)
/*Set array TabKar di indeks K dengann Address*/
{
    TabKar[K] = Address;
}

//METHOD UNTUK TABNODE
void AdvHuffman::insertAfter(unsigned char K)
/* Insert node dengan kar=K dan weight = 1 pada left NYK. Sedangkan right NYK diinsert node NYK baru*/
{
    int R = NYT;
    //Cari nomor node terbesar yang belum aktif
    while (TabNode[R].getAktif()){
        R--;
    }
    TabNode[NYT].setRight(R);
    TabKar[K] = R;
    TabNode[R].setAktif(true);
    TabNode[R].setKar(K);
    TabNode[R].setWeight(1);
    TabNode[R].setParent(NYT);
    //Cari nomor node terbesar yang belum aktif
    int oldNYT = NYT;
    while (TabNode[NYT].getAktif()){
        NYT--;
    }
    TabNode[oldNYT].setWeight(1);
    TabNode[oldNYT].setLeft(NYT);
    TabNode[NYT].setParent(oldNYT);
    TabNode[NYT].setAktif(true);

}

void AdvHuffman::switchNode(int A, int B)
/* Menukar 2 buah node pada pohon yang sama */
{
    int temp = A;
    int parentA= TabNode[A].getParent();
    int parentB= TabNode[B].getParent();
    if (isLeft(A) && (isRight(B))){
        TabNode[parentA].setLeft(B);
        TabNode[parentB].setRight(temp);
        TabNode[temp].setParent(parentB);
        TabNode[B].setParent(parentA);
    } else if (isLeft(A) && (isLeft(B))){
        TabNode[parentA].setLeft(B);
        TabNode[parentB].setLeft(temp);
        TabNode[temp].setParent(parentB);
        TabNode[B].setParent(parentA);
    } else if (isRight(A) && (isLeft(B))){
        TabNode[parentA].setRight(B);
        TabNode[parentB].setLeft(temp);
        TabNode[temp].setParent(parentB);
        TabNode[B].setParent(parentA);
    } else {
        TabNode[parentA].setRight(B);
        TabNode[parentB].setRight(temp);
        TabNode[temp].setParent(parentB);
        TabNode[B].setParent(parentA);
    }
}

char codeToChar(string s);
/* convert barisan bit ke char */

string AdvHuffman::charToCode(char c)
/* convert c ke barisan bit */
{
    string ans = "";
    for (int i = 0; i < 8; i++){
        char temp = (c << i);
        temp >>=7;
        if (temp == -1){
            ans += "1";
        } else {
            ans += "0";
        }
    }
    return ans;
}

string AdvHuffman::charCodeFromTree(int T, unsigned char kar)
/* menghasilkan barisan bit char dari pohon*/
{
    if (isLeaf(T))
        return "";
    else {
        if (searchCharTree(TabNode[T].getRight(),kar))
            return ("1" + charCodeFromTree(TabNode[T].getRight(),kar));
        else
            return ("0" + charCodeFromTree(TabNode[T].getLeft(),kar));
    }
}

string AdvHuffman::NYTcodeFromTree(int T)
/* menghasilkan barisan bit NYT dari pohon*/
{
    if (isLeaf(T))
        return "";
    else {
        if (searchNYTTree(TabNode[T].getRight()))
            return ("1" + NYTcodeFromTree(TabNode[T].getRight()));
        else
            return ("0" + NYTcodeFromTree(TabNode[T].getLeft()));
    }
}

bool AdvHuffman::searchCharTree(int T, unsigned char c)
/* menghasilkan true jika c berada di pohon*/
{
    if (T == -1)
        return false;
    else {
        return (((T == TabKar[c]) && ((T != NYT) || (T != 512))) || searchCharTree(TabNode[T].getLeft(),c) || searchCharTree(TabNode[T].getRight(),c));
    }

}

bool AdvHuffman::searchNYTTree(int T)
/* menghasilkan true jika NYT berada di pohon*/
{
    if (T == -1)
        return false;
    else {
        return ((T == NYT) || searchNYTTree(TabNode[T].getLeft()) || searchNYTTree(TabNode[T].getRight()));
    }
}

bool AdvHuffman::isLeaf(int T)
/* mengembalikan true jika T adalah daun */
{
    return ((TabNode[T].getLeft() == -1) && TabNode[T].getRight() == -1);
}

bool AdvHuffman::isLeft(int T)
/* mengembalikan true jika T merupakan anak kiri dari parent-nya*/
{
    return (TabNode[TabNode[T].getParent()].getLeft() == T);
}

bool AdvHuffman::isRight(int T)
/* mengembalikan true jika T merupakan anak kana dari parent-nya*/
{
    return (TabNode[TabNode[T].getParent()].getRight() == T);
}

//SUPPORTED METHOD FOR ADAPTIVE HUFFMAN

void AdvHuffman::Update(int T, bool baru)
/* Update pohon saat menginput node baru. T merupakan node baru yang diinput atau frequensi node lama yang ditambah*/
{
    int P;
    if (baru){
        P = TabNode[T].getParent();
        while (TabNode[P].getParent() != -1){
            int MaxNumber = MaxNumberNodeInBlock(P);
            if (MaxNumber != P){
                switchNode(P,MaxNumber);
                UpdateNomorNode();
            }
            int L = TabNode[P].getLeft();
            int R = TabNode[P].getRight();
            TabNode[P].setWeight(TabNode[L].getWeight() + TabNode[R].getWeight());
            P = TabNode[P].getParent();
        }
    } else {
        // Proses untuk mengolah T
        int MaxNumber = MaxNumberNodeInBlock(T);
        if ((MaxNumber != T) && (MaxNumber != TabNode[T].getParent())){
            switchNode(T,MaxNumber);
            UpdateNomorNode();
        }
        TabNode[T].setWeight(TabNode[T].getWeight()+1);
        P = TabNode[T].getParent();
        P = TabNode[T].getParent();
        while (TabNode[P].getParent() != -1){
            int MaxNumber = MaxNumberNodeInBlock(P);
            if (MaxNumber != P){
                switchNode(P,MaxNumber);
                UpdateNomorNode();
            }
            int L = TabNode[P].getLeft();
            int R = TabNode[P].getRight();
            TabNode[P].setWeight(TabNode[L].getWeight() + TabNode[R].getWeight());
            P = TabNode[P].getParent();
        }
    }
}

int AdvHuffman::MaxNumberNodeInBlock(int P)
/* Mencari nomor node maksimum yang memiliki weight yang sama dengan P */
{
    int S = 511;
    while (TabNode[NomorNode[S]].getWeight() != TabNode[P].getWeight()){
        S--;
    }
    return NomorNode[S];
}

void AdvHuffman::UpdateNomorNode()
/* Mengurutkan isi NomorNode */
{
    int D = DeepTree(512);
    int Start = 512;
    int i = 1;
    while (i <= D){
        setNomorNodeLevel(512, i, &Start);
        i++;
    }
}

//SUPPORTED METHOD FOR UPDATE NUMBER
int AdvHuffman::DeepTree(int T)
/* Menghasilkan nilai dari kedalaman sebuah pohon*/
{
    if (isLeaf(T))
        return 1;
    else {
        int Ldeep = DeepTree(TabNode[T].getLeft());
        int Rdeep = DeepTree(TabNode[T].getRight());
        if (Ldeep > Rdeep)
            return 1 + Ldeep;
        else
            return 1 + Rdeep;
    }
}

void AdvHuffman::setNomorNodeLevel(int T, int N, int *StartNumber)
/* Menginput alamat node ke NomorNode terurut membesar dari kiri ke kana*/
{
    if(N == 1){
        NomorNode[*StartNumber] = T;
        (*StartNumber)--;
    } else {
        if (!isLeaf(T)){
            setNomorNodeLevel(TabNode[T].getRight(),N-1,StartNumber);
            setNomorNodeLevel(TabNode[T].getLeft(),N-1,StartNumber);
        }
    }
}

void AdvHuffman::BarisBin(char* inputFile, char* outputFile) {
    ifstream infile(inputFile, ios::in|ios::binary);
    ofstream ofile(outputFile, ios::out|ios::binary);
    char c;
    int n = 0;
    while (infile.get(c)) {
        ofile <<  (int) c << " ";
    }
    infile.close();
    ofile.close();
}

double AdvHuffman::Entropy(){
    int total = 0;
    double ent = 0.0;

    for (int i = 0; i < 256; i++){
        if (TabKar[i] != -1){
            total += TabNode[TabKar[i]].getWeight();
        }
    }
    for (int i = 0; i < 256; i++){
        if (TabKar[i] != -1){
            ent += -1 * ((double)TabNode[TabKar[i]].getWeight()/(double)total) * log2((double)TabNode[TabKar[i]].getWeight()/(double)total);
        }
    }
    return ent;
}
